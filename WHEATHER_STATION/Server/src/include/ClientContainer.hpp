//
// Created by User on 21/04/2024.
//

#ifndef BOSCH_PROJECT_CLIENTCONTAINER_H
#define BOSCH_PROJECT_CLIENTCONTAINER_H

#include <vector>
#include "ClientConnection.hpp"
#include <iostream>
#include <mutex>
#include <algorithm>
#include "CustomMutex.hpp"

class ClientContainer {
private:
    std::vector<ClientConnection*> m_clients;
    std::mutex m_containerMutex;
public:
    void add(ClientConnection *cl);
    void showClients();
    int size();
    std::vector<ClientConnection*> getClients();
    void removeClient(ClientConnection*);
    void removeAllClients();
};


#endif //BOSCH_PROJECT_CLIENTCONTAINER_H
