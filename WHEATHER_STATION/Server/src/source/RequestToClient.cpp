#include "RequestToClient.hpp"
#include <limits>
#include <ios>
#include <algorithm>

#define FLAGS 0
#define SOCKET_DEFAULT_PROTOCOL 0
#define TRESHOLD_FLOOR 500
#define EXIT_CODE 0

RequestToClient::RequestToClient(ClientContainer& clientContainer, CustomLogGuard& lock)
    : m_container{clientContainer}, m_downLock{lock} {}

void RequestToClient::sentTypeOfCommand(int num, const std::shared_ptr<ClientConnection>& connection) {
    char buff[1];
    buff[0] = m_commandMapper[num];
    send(connection->getClientSocket(), buff, sizeof(buff), FLAGS);
}

void clearInputBuffer(){
    Printer printer;
    printer.notANumberTryAgain();
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void RequestToClient::setSamplePeriod(const std::shared_ptr<ClientConnection>& connection) {
    unsigned short samplePeriod;
    while (true) {
        m_printer.enterNumberBetween1And65535Message();
        if ((std::cin >> samplePeriod))
            break;
        clearInputBuffer();
    }
    connection->sentUnsignedShort(samplePeriod);
}

void RequestToClient::setTreshold(const std::shared_ptr<ClientConnection>& connection) {
    unsigned short treshold;
    while (true) {
        m_printer.enterNumberBetween0And500Message();
        if ((std::cin >> treshold)) {
            if (treshold > TRESHOLD_FLOOR)
                continue;
            break;
        }
        clearInputBuffer();
    }
    connection->sentUnsignedShort(treshold);
}

void RequestToClient::seeListOfAllClients() {
    int initialInput;

    while (true) {
        m_printer.seeListOfAllClientsMessage();
        if ((std::cin >> initialInput)) {
            break;
        }
        clearInputBuffer();
    }

    if (initialInput == 0) {
        char res;
        m_printer.areYouSureYouWantToStopTheServerMessage();
        while (true) {
            std::cin >> res;
            if (res == 'y') {
                m_printer.serverSwitchedOffMessage();
                m_container.removeAllClients();
                m_shutdownRequested = true;  // ✅ mark request to shut down
                return;
            } else if (res == 'n') {
                break;
            } else {
                m_printer.commandUnrecognizedMessage();
            }
        }
    }

    if (m_container.size() != 0)
        m_container.showClients();
    else
        m_printer.noConnectedClientsAtTheMoment();
}


std::shared_ptr<ClientConnection> RequestToClient::getCurrentConnection() {
    int socketNumber;
    while (true) {
        m_printer.enterNumberOfClientYouWantToCommunicateMessage();
        if ((std::cin >> socketNumber)) {
            break;
        }
        clearInputBuffer();
    }

    auto clients = m_container.getClients(); // copy is safe
    for (const auto& client : clients) {
        if (client && client->getSocketNumber() == socketNumber)
            return client;
    }

    return nullptr;
}

int RequestToClient::typeCommandToClient() {
    int commandToClient;
    while (true) {
        m_printer.whatDoYouWantToDoMessage();
        if ((std::cin >> commandToClient)) {
            if (commandToClient < 0 || commandToClient > 3){
                m_printer.invalidCommandOrLostConnectionMessage();
                continue;
            }
            break;
        }
        clearInputBuffer();
    }
    return commandToClient;
}

void RequestToClient::downloadLogs(const std::shared_ptr<ClientConnection>& connection) {
    connection->downloadLog();
}

void RequestToClient::waitForRequest() {
    while (!m_shutdownRequested) {
        seeListOfAllClients();
        if (m_shutdownRequested || m_container.size() == 0)
            continue;

        auto connection = getCurrentConnection();
        if (!connection) {
            m_printer.clientWithThisNumberDoesNotExistsInTheListMessage();
            continue;
        }

        int commandToClient = typeCommandToClient();
        sentTypeOfCommand(commandToClient, connection);

        std::unique_lock<std::mutex> ul(m_downLock.g_mutex);
        m_downLock.g_cv.wait(ul, [toChange = m_downLock.g_ready]() { return toChange == false; });

        switch (commandToClient) {
            case 0: setSamplePeriod(connection); break;
            case 1: setTreshold(connection); break;
            case 2: break;
            case 3: downloadLogs(connection); break;
        }

        m_downLock.g_ready = !m_downLock.g_ready;
        ul.unlock();
        m_downLock.g_cv.notify_all();
    }
}

bool RequestToClient::shouldShutdown() const {
    return m_shutdownRequested;
}

