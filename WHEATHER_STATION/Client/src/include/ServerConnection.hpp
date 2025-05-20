//
// Created by hristogarkov on 4/19/24.
//

#ifndef BOSCH_ACADEMY_SERVERCONNECTION_H
#define BOSCH_ACADEMY_SERVERCONNECTION_H

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <mutex>
#include <condition_variable>

#include "DataManager.hpp"




class ServerConnection {
public:
    ServerConnection(const std::string&, const std::string&);
    ~ServerConnection();
    void connectToServer();
private:
    bool toTerminate;
    int mClientSocket;
    std::string mServerIp;
    sockaddr_in mServerAddr;
    DataManager *mDataManager;
    void recvSampleTime();
    void registerClient();
    void checkRegistration();
    void handleConnection();
    void recvThreshold();
    int recvSingleByte();
    void sendResponseOnCommand(int);
    void clientInitialization();
};


#endif //BOSCH_ACADEMY_SERVERCONNECTION_H