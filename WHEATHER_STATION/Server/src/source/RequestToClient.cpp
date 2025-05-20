//
// Created by User on 21/04/2024.
//

#include "RequestToClient.hpp"
#include <limits>
#include <ios>

#define FLAGS 0
#define SOCKET_DEFAULT_PROTOCOL 0
#define TRESHOLD_FLOOR 500
#define EXIT_CODE 0

RequestToClient::RequestToClient(ClientContainer &clientContainer, CustomLogGuard& lock): m_container{clientContainer}
        , m_downLock{lock} {}

void RequestToClient::sentTypeOfCommand(int num, ClientConnection *connection){
    char buff[1];
    buff[0] = m_commandMapper[num];
    send(connection->getClientSocket(), buff, sizeof(buff), FLAGS);
}

void RequestToClient::setSamplePeriod(ClientConnection *connection){
    unsigned short samplePeriod;
    while(true) {
        m_printer.enterNumberBetween1And65535Message();
        if((std::cin >> samplePeriod))
            break;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    connection->sentUnsignedShort(samplePeriod);
}

void RequestToClient::setTreshold(ClientConnection *connection){
    unsigned short treshold;
    while(true) {
        m_printer.enterNumberBetween0And500Message();
        if((std::cin >> treshold)) {
           if(treshold > TRESHOLD_FLOOR)
               continue;
           break;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    connection->sentUnsignedShort(treshold);
}

void RequestToClient::seeListOfAllClients(){
    int initialInput;
    m_printer.seeListOfAllClientsMessage();
    std::cin >> initialInput;
    if(initialInput == 0){
        char res;
        m_printer.areYouSureYouWantToStopTheServerMessage();
        while(true) {
            std::cin >> res;
            if (res == 'y'){
                m_printer.serverSwitchedOffMessage();
                m_container.removeAllClients();
                exit(EXIT_CODE);
            }
            else if (res == 'n')
                break;
            else
                m_printer.commandUnrecognizedMessage();
        }
    }
    if(m_container.size() != 0)
        m_container.showClients();
    else
        m_printer.noConnectedClientsAtTheMoment();
}

ClientConnection* RequestToClient::getCurrentConnection(){
    int socketNumber;
    ClientConnection* connection = nullptr;
    m_printer.enterNumberOfClientYouWantToCommunicateMessage();
    std::cin >> socketNumber;
    int size = m_container.size();
    for (int i = 0; i < size; ++i) {
        if(socketNumber == m_container.getClients()[i]->getSocketNumber())
            connection = m_container.getClients()[i];
    }
    return connection;
}

int RequestToClient::typeCommandToClient(){
    int commandToClient;
    m_printer.whatDoYouWantToDoMessage();
    std::cin >> commandToClient;
    return commandToClient;
}

void RequestToClient::downloadLogs(ClientConnection* connection){
    connection->downloadLog();
}

void RequestToClient::waitForRequest() {
    while(true){
        seeListOfAllClients();
        if(m_container.size() == 0)
            continue;
        ClientConnection* connection = getCurrentConnection();
        if(connection == nullptr){
            m_printer.clientWithThisNumberDoesNotExistsInTheListMessage();
            continue;
        }
        int commandToClient = typeCommandToClient();
        sentTypeOfCommand(commandToClient, connection);
        std::unique_lock <std::mutex> ul(m_downLock.g_mutex);
        m_downLock.g_cv.wait(ul, [toChange=m_downLock.g_ready](){return toChange == false;});
        switch (commandToClient) {
            case 0: setSamplePeriod(connection); break;
            case 1: setTreshold(connection); break;
            case 2: break;
            case 3: downloadLogs(connection); break;
            default: m_printer.invalidCommandOrLostConnectionMessage(); break;
        }
        m_downLock.g_ready = !m_downLock.g_ready;
        ul.unlock();
        m_downLock.g_cv.notify_all();
    }
}