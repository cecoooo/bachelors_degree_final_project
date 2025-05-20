#include "Server.hpp"

int Server::initializeWinsock()
{
    m_ver = MAKEWORD(2, 2);
    if (WSAStartup(m_ver, &m_wsData) != SUCCESS_CODE)
    {
        try{
            throw WinsockInitializingException(m_printer.cantInitializeWinsockMessage());
        }catch (WinsockInitializingException& ex) {
            return ERROR_CODE;
        }
    }
    return SUCCESS_CODE;
}

int Server::createSocket()
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, SOCKET_DEFAULT_PROTOCOL);
    if (m_serverSocket == INVALID_SOCKET)
    {
        try{
            throw SocketCreatingException(m_printer.cantCreateSocketMessage());
        }catch (SocketCreatingException& ex){
            WSACleanup();
            return ERROR_CODE;
        }
    }
    return SUCCESS_CODE;
}

int Server::bindTheSocket()
{
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(SERVER_PORT);
    m_serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, (sockaddr *)&m_serverAddr, sizeof(m_serverAddr)) ==
        SOCKET_ERROR)
    {
        try{
            throw SocketBindingException(m_printer.cantBindToIPPortMessage());
        }catch(SocketBindingException& ex){
            closesocket(m_serverSocket);
            WSACleanup();
            return ERROR_CODE;
        }
    }
    return SUCCESS_CODE;
}

int Server::listeningForConnections()
{
    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        try{
            throw ServerListeningException(m_printer.cantListenMessage());
        }catch(ServerListeningException& ex){
            closesocket(m_serverSocket);
            WSACleanup();
            return ERROR_CODE;
        }
    }

    m_printer.serverListeningOnPortMessage(SERVER_PORT);
    return SUCCESS_CODE;
}

void waitForMessageFromClient(ClientContainer &clients, CustomLogGuard& downLock, ClientConnection *clientConnection){
    Printer m_printer;
    while (true)
    {
        char buff[STRUCTURE_SIZE];

        memset(buff, 0, sizeof(buff));
        int bytesReceived = recv(clientConnection->getClientSocket(), buff, sizeof(buff), FLAGS);

        if(std::string(buff, 0, bytesReceived) == "exit"){
            clients.removeClient(clientConnection);
            downLock.g_cv.notify_all();
        }
        if(std::string(buff, 0, bytesReceived) == "0"){
            downLock.g_cv.notify_all();
            std::unique_lock<std::mutex> ul(downLock.g_mutex);
            downLock.g_cv.wait(ul, [toChange=downLock.g_ready](){return toChange == true;});
            m_printer.commandIsDoneMessage();
            downLock.g_ready = !downLock.g_ready;
            continue;
        }
        if(std::string(buff, 0, bytesReceived) == "1"){
            try{
                throw ErrorInClientException(m_printer.errorInClientMessage());
            }catch(ErrorInClientException& ex){
                clients.removeClient(clientConnection);
                downLock.g_cv.notify_all();
            }
        }
        if(bytesReceived == STRUCTURE_SIZE){
            clientConnection->saveNotificationData(buff);
        }
    }
}

void handleClient(SOCKET clientSocket, ClientContainer &clients, CustomLogGuard& downLock)
{
    ClientConnection *clientConnection = new ClientConnection(clientSocket);
    clients.add(clientConnection);
    clientConnection->sentUnsignedShort(DEFAULT_SAMPLE_TIME);
    clientConnection->getClientData();
    waitForMessageFromClient(clients, downLock, clientConnection);
}

void Server::acceptConnections(ClientContainer &clientContainer)
{
    while (true)
    {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket =
                accept(m_serverSocket, (sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET)
        {
            try{
                throw ClientConnectingException(m_printer.problemWithClientConnectingMessage());
            }catch(ClientConnectingException& ex){
                closesocket(m_serverSocket);
                WSACleanup();
                return;
            }
        }

        // receive and send message
        m_threads.emplace_back(handleClient, clientSocket, std::ref(clientContainer), std::ref(m_DownLock));
        m_threads.back().detach();
    }
}

void Server::start(ClientContainer& clientContainer){
    if(initializeWinsock() == ERROR_CODE)
        return;
    if (createSocket() == ERROR_CODE)
        return;
    if (bindTheSocket() == ERROR_CODE)
        return;
    if (listeningForConnections() == ERROR_CODE)
        return;
    std::thread requestThread(&RequestToClient::waitForRequest, RequestToClient(std::ref(clientContainer), std::ref(m_DownLock)));
    requestThread.detach();
    acceptConnections(clientContainer);
}

Server::~Server()
{
    int size = m_threads.size();
    for (int i = 0; i < size; ++i)
        m_threads[i].join();

    closesocket(m_serverSocket);
    WSACleanup();
}