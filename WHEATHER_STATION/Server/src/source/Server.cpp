#include "Server.hpp"

int Server::initializeWinsock() {
    m_ver = MAKEWORD(2, 2);
    if (WSAStartup(m_ver, &m_wsData) != SUCCESS_CODE) {
        try {
            throw WinsockInitializingException(m_printer.cantInitializeWinsockMessage());
        } catch (WinsockInitializingException& ex) {
            return ERROR_CODE;
        }
    }
    return SUCCESS_CODE;
}

int Server::createSocket() {
    m_serverSocket = socket(AF_INET, SOCK_STREAM, SOCKET_DEFAULT_PROTOCOL);
    if (m_serverSocket == INVALID_SOCKET) {
        try {
            throw SocketCreatingException(m_printer.cantCreateSocketMessage());
        } catch (SocketCreatingException& ex) {
            WSACleanup();
            return ERROR_CODE;
        }
    }
    return SUCCESS_CODE;
}

int Server::bindTheSocket() {
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(SERVER_PORT);
    m_serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == SOCKET_ERROR) {
        try {
            throw SocketBindingException(m_printer.cantBindToIPPortMessage());
        } catch (SocketBindingException& ex) {
            closesocket(m_serverSocket);
            WSACleanup();
            return ERROR_CODE;
        }
    }
    return SUCCESS_CODE;
}

int Server::listeningForConnections() {
    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        try {
            throw ServerListeningException(m_printer.cantListenMessage());
        } catch (ServerListeningException& ex) {
            closesocket(m_serverSocket);
            WSACleanup();
            return ERROR_CODE;
        }
    }

    m_printer.serverListeningOnPortMessage(SERVER_PORT);
    return SUCCESS_CODE;
}

void Server::waitForMessageFromClient(ClientContainer& clients, CustomLogGuard& downLock, std::shared_ptr<ClientConnection> clientConnection) {
    Printer m_printer;

    while (true) {
        char buff[STRUCTURE_SIZE] = {};
        int bytesReceived = recv(clientConnection->getClientSocket(), buff, sizeof(buff), FLAGS);

        if (bytesReceived <= 0) {
            clients.removeClient(clientConnection);
            downLock.g_cv.notify_all();
            return;
        }

        std::string message(buff, 0, bytesReceived);

        if (message == "exit") {
            clients.removeClient(clientConnection);
            downLock.g_cv.notify_all();
            return;
        }

        if (message == "0") {
            downLock.g_cv.notify_all();
            std::unique_lock<std::mutex> ul(downLock.g_mutex);
            downLock.g_cv.wait(ul, [toChange = downLock.g_ready]() { return toChange == true; });
            m_printer.commandIsDoneMessage();
            downLock.g_ready = !downLock.g_ready;
            continue;
        }

        if (message == "1") {
            try {
                throw ErrorInClientException(m_printer.errorInClientMessage());
            } catch (ErrorInClientException& ex) {
                clients.removeClient(clientConnection);
                downLock.g_cv.notify_all();
                return;
            }
        }

        if (bytesReceived == STRUCTURE_SIZE) {
            clientConnection->saveNotificationData(buff);
        }
    }
}

void Server::handleClient(SOCKET clientSocket, ClientContainer& clients, CustomLogGuard& downLock) {
    auto clientConnection = std::make_shared<ClientConnection>(clientSocket);
    clients.add(clientConnection);
    clientConnection->sentUnsignedShort(DEFAULT_SAMPLE_TIME);
    clientConnection->getClientData();
    Server::waitForMessageFromClient(clients, downLock, clientConnection);
}

void Server::acceptConnections(ClientContainer& clientContainer) {
    while (!m_shouldStop) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(m_serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            if (!m_shouldStop) {
                try {
                    throw ClientConnectingException(m_printer.problemWithClientConnectingMessage());
                } catch (...) {
                    // log if needed
                }
            }
            break;
        }

        m_threads.emplace_back(&Server::handleClient, clientSocket, std::ref(clientContainer), std::ref(m_DownLock));
    }
}

void Server::start(ClientContainer& clientContainer) {
    if (initializeWinsock() == ERROR_CODE) return;
    if (createSocket() == ERROR_CODE) return;
    if (bindTheSocket() == ERROR_CODE) return;
    if (listeningForConnections() == ERROR_CODE) return;

    // Create request handler object
    RequestToClient requestToClient(clientContainer, m_DownLock);

    // Start the request-handling thread
    m_requestThread = std::thread(&RequestToClient::waitForRequest, &requestToClient);

    // Start accepting connections in a separate thread
    std::thread acceptThread(&Server::acceptConnections, this, std::ref(clientContainer));

    // Wait for the request thread to finish
    if (m_requestThread.joinable())
        m_requestThread.join();

    // Check if shutdown was requested
    if (requestToClient.shouldShutdown()) {
        requestShutdown(); // closes socket, triggers accept to exit
    }

    // Wait for accept thread to finish
    if (acceptThread.joinable())
        acceptThread.join();

    // Join all client threads
    for (auto& t : m_threads)
        if (t.joinable())
            t.join();
}



Server::~Server() {
    if (m_requestThread.joinable())
        m_requestThread.join();

    for (auto& t : m_threads)
        if (t.joinable())
            t.join();

    closesocket(m_serverSocket);
    WSACleanup();
}

void Server::requestShutdown() {
    m_shouldStop = true;
    closesocket(m_serverSocket); // interrupts blocking accept()
}