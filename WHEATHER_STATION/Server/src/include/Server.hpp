#ifndef UNTITLED_SERVER_H
#define UNTITLED_SERVER_H

#pragma comment(lib, "ws2_32.lib")

#include <winsock.h>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "ClientConnection.hpp"
#include "ClientContainer.hpp"
#include "RequestToClient.hpp"
#include "CustomLogGuard.hpp"
#include "Printer.hpp"
#include "WinsockInitializingException.hpp"
#include "SocketCreatingException.hpp"
#include "SocketBindingException.hpp"
#include "ServerListeningException.hpp"
#include "ErrorInClientException.hpp"
#include "ClientConnectingException.hpp"

#define ERROR_CODE 1
#define SUCCESS_CODE 0
#define SOCKET_DEFAULT_PROTOCOL 0
#define SERVER_PORT 54000
#define FLAGS 0
#define DEFAULT_SAMPLE_TIME 2000
#define STRUCTURE_SIZE 20

class Server {
private:
    WSADATA m_wsData;
    WORD m_ver;
    SOCKET m_serverSocket;
    sockaddr_in m_serverAddr;
    std::vector<std::thread> m_threads;
    int initializeWinsock();
    int createSocket();
    int bindTheSocket();
    int listeningForConnections();
    void acceptConnections(ClientContainer& clientContainer);
    void listenForRequest(ClientContainer& clientContainer);
    unsigned short SAMPLE_PERIOD{32000};
    void waitForMessageFromClient(ClientContainer&, CustomLogGuard&, ClientConnection*);
    CustomLogGuard m_DownLock;
    Printer m_printer;
public:
    ~Server();
    void start(ClientContainer& clientContainer);
};


#endif //UNTITLED_SERVER_H
