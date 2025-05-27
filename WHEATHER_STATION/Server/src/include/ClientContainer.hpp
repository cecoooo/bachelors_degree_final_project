#ifndef BOSCH_PROJECT_CLIENTCONTAINER_H
#define BOSCH_PROJECT_CLIENTCONTAINER_H

#include <vector>
#include <memory>
#include <mutex>
#include <algorithm>
#include <iostream>
#include "ClientConnection.hpp"
#include "CustomMutex.hpp"

class ClientContainer {
private:
    std::vector<std::shared_ptr<ClientConnection>> m_clients;
    std::mutex m_containerMutex;

public:
    void add(const std::shared_ptr<ClientConnection>& cl);
    void showClients();
    int size();
    std::vector<std::shared_ptr<ClientConnection>> getClients();
    void removeClient(const std::shared_ptr<ClientConnection>& clientConnection);
    void removeAllClients();
};

#endif // BOSCH_PROJECT_CLIENTCONTAINER_H
