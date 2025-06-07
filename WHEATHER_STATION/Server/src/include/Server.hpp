#pragma once

#include <QObject>
#include <thread>
#include <map>
#include <atomic>
#include <winsock2.h>
#include <memory>

#include "ClientConnection.hpp"

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject* parent = nullptr);
    ~Server();

    bool start();
    void shutdown();
    void sendCommandToClient(int socketId, char commandId, const QByteArray& payload);

signals:
    void clientConnected(int socketId);
    void clientDisconnected(int socketId);
    void serverStopped();

private:
    void acceptConnections();
    void handleClient(std::shared_ptr<ClientConnection> client);

    std::thread m_acceptThread;
    std::atomic<bool> m_running;
    SOCKET m_serverSocket;
    sockaddr_in m_serverAddr;

    std::mutex m_clientsMutex;
    std::map<int, std::shared_ptr<ClientConnection>> m_clients;
};