//
// Created by User on 21/04/2024.
//

#ifndef BOSCH_PROJECT_REQUESTTOCLIENT_H
#define BOSCH_PROJECT_REQUESTTOCLIENT_H

#include <map>
#include "ClientContainer.hpp"
#include "CustomLogGuard.hpp"
#include "Printer.hpp"


class RequestToClient {
private:
    ClientContainer& m_container;
    void setSamplePeriod(ClientConnection *);
    void sentTypeOfCommand(int, ClientConnection*);
    void setTreshold(ClientConnection *connection);
    std::map<int, char> m_commandMapper{{0, '0'},{1, '1'},{2, '2'},{3, '3'}};
    CustomLogGuard& m_downLock;
    ClientConnection* getCurrentConnection();
    int typeCommandToClient();
    void seeListOfAllClients();
    void downloadLogs(ClientConnection*);
    Printer m_printer;
public:
    RequestToClient(ClientContainer&, CustomLogGuard&);
    void waitForRequest();
};


#endif //BOSCH_PROJECT_REQUESTTOCLIENT_H
