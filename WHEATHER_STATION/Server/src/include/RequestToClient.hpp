#ifndef BOSCH_PROJECT_REQUESTTOCLIENT_H
#define BOSCH_PROJECT_REQUESTTOCLIENT_H

#include <map>
#include <memory>
#include "ClientContainer.hpp"
#include "CustomLogGuard.hpp"
#include "Printer.hpp"

class RequestToClient {
private:
    ClientContainer& m_container;
    void setSamplePeriod(const std::shared_ptr<ClientConnection>&);
    void sentTypeOfCommand(int, const std::shared_ptr<ClientConnection>&);
    void setTreshold(const std::shared_ptr<ClientConnection>&);
    std::map<int, char> m_commandMapper{{0, '0'}, {1, '1'}, {2, '2'}, {3, '3'}};
    CustomLogGuard& m_downLock;
    std::shared_ptr<ClientConnection> getCurrentConnection();
    int typeCommandToClient();
    void seeListOfAllClients();
    void downloadLogs(const std::shared_ptr<ClientConnection>&);
    Printer m_printer;
    bool m_shutdownRequested{false};
public:
    RequestToClient(ClientContainer&, CustomLogGuard&);
    bool shouldShutdown() const;
    void waitForRequest();
};

#endif // BOSCH_PROJECT_REQUESTTOCLIENT_H
