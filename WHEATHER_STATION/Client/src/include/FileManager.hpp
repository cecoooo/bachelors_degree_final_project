//
// Created by hristogarkov on 4/21/24.
//

#ifndef BOSCH_PROJECT_FILEMANAGER_H
#define BOSCH_PROJECT_FILEMANAGER_H

#include "ClientData.hpp"
#include "TimeProvider.hpp"

#include <string>
#include <mutex>

class FileManager {
public:
    void sendTerminationSignal();
    FileManager(const int, const std::string&);
    void writeData(const ClientData&);
    void clearFile();
    void sendFile();
private:
    std::mutex mFileMutex;
    const int mClientSocket;
    TimeProvider mTimeProvider;
    std::string mFileName;
};


#endif //BOSCH_PROJECT_FILEMANAGER_H
