#include "ClientConnection.hpp"

ClientConnection::ClientConnection(SOCKET cs){
    m_clientSocket = cs;
    m_fileName = DATA_DIR_PATH + std::to_string(this->getSocketNumber()) + "_ClientFile.txt";
}

ClientConnection::~ClientConnection() {
    std::remove(m_fileName.c_str());
    closesocket(m_clientSocket);
    //WSACleanup();
}

void ClientConnection::getClientData() {
    // create buffer for data
    char buffer[STRUCTURE_SIZE];
    memset(buffer, 0, sizeof(buffer));

    // receive data
    int bytesReceived = recv(m_clientSocket, buffer, sizeof(buffer), FLAGS);

    //sent 0 for OK, 1 for error
    sentDataQualityResponse(bytesReceived);

    if(bytesReceived < STRUCTURE_SIZE)
        return;

    CustomMutex ctsMtx(m_mtx);
    ClientData cd;
    memcpy(&cd, buffer, sizeof(buffer));

    m_fileWriteStructure.open(m_fileName,std::ios_base::app);
    if(!checkIsFileOpen())
        return;
    m_fileWriteStructure << "starting data:" << std::endl;
    m_fileWriteStructure << cd.latitude << "/";
    m_fileWriteStructure << cd.longitude << "/";
    m_fileWriteStructure << cd.AQI << "/";
    m_fileWriteStructure << cd.temperature << std::endl;
    m_fileWriteStructure.close();
}

void ClientConnection::sentUnsignedShort(unsigned short period) {
    char samplePeriod[SAMPLE_PERIOD_SIZE];
    unsigned short sp = period;
    unsigned short sp1 = htons(sp);
    memcpy(samplePeriod, &sp1, sizeof(short));
    send(m_clientSocket, samplePeriod, sizeof(short), FLAGS);
}

void ClientConnection::sentDataQualityResponse(int bytesReceived){
    char response[RESPONSE_SIZE];
    if (bytesReceived < STRUCTURE_SIZE) {
        response[0] = '1';
    } else {
        response[0] = '0';
    }
    send(m_clientSocket, response, sizeof(response), FLAGS);
}

SOCKET ClientConnection::getClientSocket() {
    return m_clientSocket;
}

int ClientConnection::getSocketNumber() {
    return static_cast<int>(m_clientSocket);
}

void ClientConnection::downloadLog(){
    char buffer[BUFFER_SIZE] = {0};
    CustomMutex ctsMtx(m_mtx);
    m_fileWriteStructure.open(m_fileName, std::ios_base::app);
    if(!checkIsFileOpen())
        return;
    m_fileWriteStructure << "Requested data:" << std::endl;
    while(true){
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(m_clientSocket, buffer, sizeof(buffer), FLAGS);
        if (bytesReceived <= 0) {
            break;
        }
        if(std::string(buffer, 0, bytesReceived) == "end"){
            break;
        }
        m_fileWriteStructure << std::string(buffer, 0, bytesReceived) << std::endl;
        send(m_clientSocket, buffer, sizeof(buffer), FLAGS);
    }
    m_fileWriteStructure.close();
}

void ClientConnection::saveNotificationData(const char buff[]) {
    CustomMutex ctsMtx(m_mtx);
    ClientData cd;
    memcpy(&cd, buff, sizeof(cd));
    m_fileWriteStructure.open(m_fileName, std::ios_base::app);
    if(!checkIsFileOpen())
        return;
    m_fileWriteStructure << "Notification data:" << std::endl;
    m_fileWriteStructure << cd.latitude << "/";
    m_fileWriteStructure << cd.longitude << "/";
    m_fileWriteStructure << cd.AQI << "/";
    m_fileWriteStructure << cd.temperature << std::endl;
    m_fileWriteStructure.close();
}

bool ClientConnection::checkIsFileOpen() {
    if (!m_fileWriteStructure.is_open()) {
        try {
            throw FileOpeningException(m_printer.cantOpenFile());
        }catch (FileOpeningException& ex){
            return false;
        }
    }
    return true;
}