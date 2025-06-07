#include "ClientConnection.hpp"
#include <cstring>
#include <winsock2.h>

#define BUFFER_SIZE 64
#define STRUCTURE_SIZE 20
#define FLAGS 0
#define RESPONSE_SIZE 1

ClientConnection::ClientConnection(SOCKET socket)
    : m_socket(socket), m_connected(true) {
    m_filename = "../client_data/" + std::to_string(socket) + "_ClientFile.txt";
}

ClientConnection::~ClientConnection() {
    close();
}

bool ClientConnection::isConnected() const {
    return m_connected;
}

SOCKET ClientConnection::socket() const {
    return m_socket;
}

void ClientConnection::sendCommand(char commandId, const QByteArray& payload) {
    send(m_socket, &commandId, 1, 0);
    if (!payload.isEmpty()) {
        if ((commandId == '0' || commandId == '1') && payload.size() == sizeof(unsigned short)) {
            unsigned short value;
            std::memcpy(&value, payload.data(), sizeof(unsigned short));
            unsigned short netValue = htons(value);
            send(m_socket, reinterpret_cast<const char*>(&netValue), sizeof(unsigned short), 0);
        } else {
            send(m_socket, payload.data(), payload.size(), 0);
        }
    }
}

void ClientConnection::close() {
    if (m_connected) {
        closesocket(m_socket);
        m_connected = false;
        if (m_outputFile.is_open()) 
            m_outputFile.close();
        std::remove(m_filename.c_str());
    }
}

void ClientConnection::saveMeasurement(const ClientData& data, bool isInitialData) {
    std::lock_guard<std::mutex> lock(m_fileMutex);
    m_outputFile.open(m_filename, std::ios::out | std::ios::app);
    if (!m_outputFile.is_open()) return;
    if(isInitialData) m_outputFile << "first measurement" << std::endl;
    m_outputFile << data.latitude << "/" << data.longitude << "/"
                 << data.aqi << "/" << data.temperature << std::endl;
    if(isInitialData) m_outputFile << "=================" << std::endl;
    m_outputFile.close();
    // FOR DB

    if(!g_dbManager.insertNotification(static_cast<int>(m_socket), data.latitude, data.longitude, data.temperature, data.aqi))
    {
        std::cout << "failure while saving notification" << std::endl;
    }
}

void ClientConnection::sentDataQualityResponse(int bytesReceived){
    char response[RESPONSE_SIZE];
    if (bytesReceived < STRUCTURE_SIZE) {
        response[0] = '1';
    } else {
        response[0] = '0';
    }
    send(m_socket, response, sizeof(response), FLAGS);
}

ReceiveResult ClientConnection::receiveData() {
    ReceiveResult result;
    char buffer[BUFFER_SIZE] = {};

    int ret = recv(m_socket, buffer, sizeof(buffer), FLAGS);


    if (ret <= 0) {
        result.exitRequested = true;
        return result;
    }

    std::string message(buffer, 0, ret);

    if (message == "exit") {
        // char dummy[4];
        // recv(m_socket, dummy, 4, 0);  // consume "exit"
        result.exitRequested = true;
        return result;
    }
    if (message == "0") {
        return result;
    }
    if (message == "1") {
        // char dummy[1];
        // recv(m_socket, dummy, 1, 0);
        result.exitRequested = true;
        return result;
    }
    if (ret == STRUCTURE_SIZE) {
        ClientData data;
        std::memcpy(&data, buffer, STRUCTURE_SIZE);
        saveMeasurement(data);
        return result;
    }

    {
        std::lock_guard<std::mutex> lock(m_fileMutex);
        m_outputFile.open(m_filename, std::ios::out | std::ios::app);
        if (!m_outputFile.is_open()) 
        {
            result.exitRequested = true;
            return result;
        }

        m_outputFile << "Requested data:" << std::endl;
        m_outputFile << std::string(buffer, 0, ret) << std::endl;
        send(m_socket, buffer, sizeof(buffer), 0);
        
        
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesReceived = recv(m_socket, buffer, sizeof(buffer), 0);
            if (bytesReceived <= 0) break;

            if (std::string(buffer, 0, bytesReceived) == "end") break;

            m_outputFile << std::string(buffer, 0, bytesReceived) << std::endl;

            if(!g_dbManager.insertRequestedRecording(static_cast<int>(m_socket), std::string(buffer, 0, bytesReceived)))
            {
                std::cout << "failure while saving requested data" << std::endl;
            }
            
            send(m_socket, buffer, sizeof(buffer), 0);
            // FOR DB
        }

        m_outputFile.close();
    }

    return result;
}