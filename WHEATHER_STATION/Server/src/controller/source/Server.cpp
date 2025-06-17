#include "Server.hpp"
#include <QDebug>
#include <stdexcept>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

Server::Server(QObject* parent) : QObject(parent), m_running(false), m_serverSocket(INVALID_SOCKET) {
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) throw std::runtime_error("WSAStartup failed");

    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET) throw std::runtime_error("Socket creation failed");

    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(54000);
    m_serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, reinterpret_cast<sockaddr*>(&m_serverAddr), sizeof(m_serverAddr)) == SOCKET_ERROR)
        throw std::runtime_error("Bind failed");

    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
        throw std::runtime_error("Listen failed");

    // FOR DB
    g_dbManager.connect("WeatherStationDSN", "sa", "NewStrongPassword123!");
}

Server::~Server() {
    shutdown();
}

bool Server::start() {
    m_running = true;
    m_acceptThread = std::thread(&Server::acceptConnections, this);
    return true;
}

void Server::acceptConnections() {
    while (m_running) {
        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(m_serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientSize);
        if (clientSocket == INVALID_SOCKET) continue;

        auto client = std::make_shared<ClientConnection>(clientSocket);
        int socketId = static_cast<int>(clientSocket);

        // Step 1: Send default sample time (2000 ms)
        unsigned short defaultSampleTime = 2000;
        QByteArray payload(reinterpret_cast<const char*>(&defaultSampleTime), sizeof(defaultSampleTime));
        unsigned short value;
        std::memcpy(&value, payload.data(), sizeof(unsigned short));
        unsigned short netValue = htons(value);
        send(clientSocket, reinterpret_cast<const char*>(&netValue), sizeof(unsigned short), 0);

        // Step 2: Receive first measurement
        char buffer[20];
        memset(buffer, 0, sizeof(buffer));

        int ret = recv(clientSocket, buffer, sizeof(buffer), 0);
        ClientData data;
        std::memcpy(&data, buffer, sizeof(buffer));

        // FOR DB
        if(!g_dbManager.insertClient(socketId, std::to_string(socketId) + "_ClientFile.txt"))
        {
            std::cout << "failure while saving client" << std::endl;
        }

        client->saveMeasurement(data, true);

        // Step 3: sent OK responce
        client->sentDataQualityResponse(ret);

        // Add to map
        {
            std::lock_guard<std::mutex> lock(m_clientsMutex);
            m_clients[socketId] = client;
        }

        // start listenig
        emit clientConnected(socketId);
        std::thread(&Server::handleClient, this, client).detach();
    }
}

void Server::handleClient(std::shared_ptr<ClientConnection> client) {
    int socketId = static_cast<int>(client->socket());

    while (client->isConnected() && m_running) {
        auto result = client->receiveData();
        if (result.exitRequested) break;
    }

    {
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        m_clients.erase(socketId);
    }

    emit clientDisconnected(socketId);
}

void Server::sendCommandToClient(int socketId, char commandId, const QByteArray& payload) {
    std::lock_guard<std::mutex> lock(m_clientsMutex);
    if (m_clients.find(socketId) != m_clients.end()) {
        m_clients[socketId]->sendCommand(commandId, payload);
    }
}

void Server::shutdown() {
    m_running = false;

    {
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        for (auto& [id, client] : m_clients) {
            client->close();
        }
        m_clients.clear();
    }

    closesocket(m_serverSocket);
    if (m_acceptThread.joinable())
        m_acceptThread.join();
    
    emit serverStopped();
    WSACleanup();
}