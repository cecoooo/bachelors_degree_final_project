//
// Created by hristogarkov on 4/19/24.
//

#include <thread>

#include "ServerConnection.hpp"
#include "ClientData.hpp"
#include "ConnectionException.hpp"
#include "RemoveClient.hpp"

#define SIZE_OF_CLIENT_DATA 20
#define SIZE_OF_RESPONSE 1
#define CHAR_TO_INT_DISPARITY 48
#define INITIAL_THRESHOLD 50    
#define PORT_HOST 54000

ServerConnection::ServerConnection(const std::string& serverIp, const std::string& fileName): mServerIp{serverIp}, mServerAddr{}, toTerminate{false}{
    mClientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mClientSocket == -1) {
        throw ConnectionException("Can't create socket!");
    }
    mServerAddr.sin_family = AF_INET;
    mServerAddr.sin_port = htons(PORT_HOST);
    mDataManager = new DataManager(mClientSocket, fileName);
}

void ServerConnection::connectToServer() {
    inet_pton(AF_INET, mServerIp.c_str(), &mServerAddr.sin_addr);

    if (connect(mClientSocket, (sockaddr*)&mServerAddr, sizeof(mServerAddr)) == -1) {
        throw ConnectionException("Can't connect to server");
    }else{
        std::cout << "Connected to server" << std::endl;
    }
    handleConnection();
}

void ServerConnection::handleConnection() {
    try{
        clientInitialization();
    }catch (ConnectionException& ex){
        std::cout << "Server registration failed - try again!" << std::endl;
        return;
    }

    while(true){
        int command =  recvSingleByte();
        sendResponseOnCommand(command);

        if(toTerminate){
            mDataManager->terminate();
            break;
        }
        switch (command) {
            case 0:
                recvSampleTime();
                break;
            case 1:
                recvThreshold();
                break;
            case 2:
                mDataManager->clearData();
                break;
            case 3:
                mDataManager->sendToServer();
                break;
        }
    }

}

void ServerConnection::recvSampleTime() {
    unsigned short receivedShort;
    recv(mClientSocket, &receivedShort, sizeof(receivedShort), 0);
    unsigned short samplePeriod = ntohs(receivedShort);
    mDataManager->setSamplePeriod(samplePeriod);
}

void ServerConnection::registerClient() {
    ClientData registrationData;
    mDataManager->getRegistrationData(registrationData);

    char buffer[SIZE_OF_CLIENT_DATA];
    memcpy(buffer, &registrationData, SIZE_OF_CLIENT_DATA);
    send(mClientSocket, buffer, SIZE_OF_CLIENT_DATA, 0);
}

void ServerConnection::checkRegistration() {
    int responseInt = recvSingleByte();
    if(responseInt == 0){
        std::cout << "OK" << std::endl;
    }else{
        throw ConnectionException("Client was not registered!");
    }
}

void ServerConnection::recvThreshold() {
    unsigned short receivedShort;
    recv(mClientSocket, &receivedShort, sizeof(receivedShort), 0);
    unsigned short threshold = ntohs(receivedShort);
    mDataManager->setThreshold(threshold);
}

int ServerConnection::recvSingleByte() {
    char response[SIZE_OF_RESPONSE];
    recv(mClientSocket, response, SIZE_OF_RESPONSE, 0);
    return response[0] - CHAR_TO_INT_DISPARITY;
}

void ServerConnection::sendResponseOnCommand(int command) {
    char checkSignal[SIZE_OF_RESPONSE];
    if(command >= 0 && command <= 3){
        checkSignal[0] = '0';
    }else{
        checkSignal[0] = '1';
        toTerminate = true;
    }

    send(mClientSocket, checkSignal, sizeof(checkSignal), 0);
}

void ServerConnection::clientInitialization() {
    mDataManager->clearData();
    mDataManager->setThreshold(INITIAL_THRESHOLD);
    recvSampleTime();

    std::thread dataThread(&DataManager::readData, mDataManager);
    dataThread.detach();

    registerClient();
    checkRegistration();

    std::thread closeClient(&RemoveClient::checkForDisconnect, RemoveClient(mClientSocket, std::ref(toTerminate)));
    closeClient.detach();
}

ServerConnection::~ServerConnection(){
    close(mClientSocket);
    if(mDataManager != nullptr){
        delete mDataManager;
    }
}

