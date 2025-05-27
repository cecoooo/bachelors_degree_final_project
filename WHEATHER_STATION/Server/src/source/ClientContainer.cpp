#include "ClientContainer.hpp"

void ClientContainer::add(const std::shared_ptr<ClientConnection>& cl) {
    CustomMutex mutex(m_containerMutex);
    m_clients.push_back(cl);
}

void ClientContainer::showClients() {
    CustomMutex mutex(m_containerMutex);
    for (const auto& client : m_clients) {
        if (client) {
            std::cout << client->getClientSocket() << std::endl;
        }
    }
}

int ClientContainer::size() {
    CustomMutex mutex(m_containerMutex);
    return static_cast<int>(m_clients.size());
}

std::vector<std::shared_ptr<ClientConnection>> ClientContainer::getClients() {
    CustomMutex mutex(m_containerMutex);
    return m_clients; // copy is safe
}

void ClientContainer::removeClient(const std::shared_ptr<ClientConnection>& clientConnection) {
    CustomMutex mutex(m_containerMutex);
    auto it = std::find(m_clients.begin(), m_clients.end(), clientConnection);
    if (it != m_clients.end()) {
        m_clients.erase(it);
    }
}

void ClientContainer::removeAllClients() {
    CustomMutex mutex(m_containerMutex);
    for (auto& client : m_clients) {
        if (client)
            client->closeConnection(); // 🚨 force socket to close
    }
    m_clients.clear(); // destroy shared_ptrs
}
