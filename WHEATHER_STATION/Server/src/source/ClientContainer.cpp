//
// Created by User on 21/04/2024.
//

#include "ClientContainer.hpp"

void ClientContainer::add(ClientConnection *cl)
{
    CustomMutex mutex(m_containerMutex);
    m_clients.push_back(cl);
}

void ClientContainer::showClients()
{
    CustomMutex mutex(m_containerMutex);
    int size = m_clients.size();
    for (int i = 0; i < size; ++i)
    {
        std::cout << m_clients[i]->getClientSocket() << std::endl;
    }
}

int ClientContainer::size() {
    CustomMutex mutex(m_containerMutex);
    int sizeOfVectorOfClients = m_clients.size();
    return sizeOfVectorOfClients;
}

std::vector<ClientConnection*> ClientContainer::getClients() {
    CustomMutex mutex(m_containerMutex);
    std::vector<ClientConnection*> &res = m_clients;
    return res;
}

void ClientContainer::removeClient(ClientConnection* clientConnection) {
    CustomMutex mutex(m_containerMutex);
    auto it = std::find(m_clients.begin(), m_clients.end(), clientConnection);
    if (it != m_clients.end()) {
        m_clients.erase(it, it+1);
    }
    delete clientConnection;
    clientConnection = nullptr;
}

void ClientContainer::removeAllClients(){
    CustomMutex mutex(m_containerMutex);
    while (m_clients.size() != 0){
        m_containerMutex.unlock();
        removeClient(m_clients[m_clients.size()-1]);
    }
}