#ifndef BOSCH_PROJECT_CLIENTCONNECTION_H
#define BOSCH_PROJECT_CLIENTCONNECTION_H

#include <winsock.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <cstdio>

#include "ClientData.hpp"
#include "FileOpeningException.hpp"
#include "Printer.hpp"
#include "CustomMutex.hpp"

#define BUFFER_SIZE 64
#define FLAGS 0
#define RESPONSE_SIZE 1
#define SAMPLE_PERIOD_SIZE 2
#define STRUCTURE_SIZE 20
#define DATA_DIR_PATH "../../client_data/"

class ClientConnection {
private:
    SOCKET m_clientSocket;
    void sentDataQualityResponse(int);
    std::ofstream m_fileWriteStructure;
    std::string m_fileName;
    std::mutex m_mtx;
    Printer m_printer;
    bool checkIsFileOpen();
public:
    ClientConnection(SOCKET);
    ~ClientConnection();
    int getSocketNumber();
    void sentUnsignedShort(unsigned short);
    void getClientData();
    SOCKET getClientSocket();
    void downloadLog();
    void saveNotificationData(const char buff[]);
};


#endif
