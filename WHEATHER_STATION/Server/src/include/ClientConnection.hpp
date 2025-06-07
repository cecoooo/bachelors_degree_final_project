#pragma once

#include <winsock2.h>
#include <string>
#include <fstream>
#include <atomic>
#include <mutex>
// detede #include <iostream> after all tests
#include <iostream>
#include <QByteArray>

#include "DatabaseGlobal.hpp"


#pragma pack(push, 1)
struct ClientData {
    double latitude;
    double longitude;
    short aqi;
    short temperature;
};
#pragma pack(pop)

struct ReceiveResult {
    bool exitRequested = false;
};

class ClientConnection {
public:
    explicit ClientConnection(SOCKET socket);
    ~ClientConnection();

    bool isConnected() const;
    SOCKET socket() const;

    ReceiveResult receiveData();
    void sendCommand(char commandId, const QByteArray& payload);
    void close();

    void saveMeasurement(const ClientData& data, bool isInitialData = false);
    void sentDataQualityResponse(int bytesReceived);

private:
    SOCKET m_socket;
    std::atomic<bool> m_connected;
    std::ofstream m_outputFile;
    std::string m_filename;
    std::mutex m_fileMutex;
};

