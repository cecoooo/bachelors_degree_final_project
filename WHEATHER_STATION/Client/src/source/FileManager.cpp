#include "FileManager.hpp"
#include "MutexGuard.hpp"
#include "FileOpeningException.hpp"

#include <sys/socket.h>
#include <iostream>
#include <fstream>


#define BUFFER_SIZE 64




FileManager::FileManager(const int socket, const std::string& fileName):mClientSocket{socket}, mTimeProvider{}, mFileName{fileName} {}


void FileManager::writeData(const ClientData &data) {
    MutexGuard lock(mFileMutex);
    std::ofstream outputFile;
    outputFile.open (mFileName, std::ios_base::app);
    std::string time = mTimeProvider.getTime();
    if(outputFile.is_open()){
        outputFile << data.latitude << "/" << data.longitude << "/" << data.AQI << "/" << data.temperature << " " << time;
    }else{
        throw FileOpeningException("Couldn't write data!");
    }
    outputFile.close();
}

void FileManager::clearFile() {
    MutexGuard lock(mFileMutex);
    std::ofstream outputFile;
    outputFile.open(mFileName, std::ofstream::trunc);
    outputFile.close();
}

void FileManager::sendFile() {
    char buffer[BUFFER_SIZE];

    MutexGuard lock(mFileMutex);
    std::ifstream myfile;
    myfile.open(mFileName);
    std::string line;
    if (!myfile.is_open()) {
        throw FileOpeningException("Can't transfer data!");
    }
    while ( myfile ) {
        std::getline (myfile, line);
        send(mClientSocket, line.c_str(), line.length() + 1, 0);
        recv(mClientSocket, buffer, sizeof(buffer), 0);
    }

    sendTerminationSignal();

    myfile.close();
}

void FileManager::sendTerminationSignal() {
    std::string terminationSignal = "end";
    send(mClientSocket, terminationSignal.c_str(), terminationSignal.length() + 1, 0);
}