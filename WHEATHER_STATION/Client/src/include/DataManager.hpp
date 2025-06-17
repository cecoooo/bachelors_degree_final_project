#ifndef BOSCH_ACADEMY_DATAREADER_H
#define BOSCH_ACADEMY_DATAREADER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mutex>
#include <cstring>

#include "ClientData.hpp"
#include "FileManager.hpp"
//Remove the code after "beaglebone" to run it on a PC
//beaglebone
// #include "../../include/aqi_detect.h"
// #include "AnalogReader.hpp"



class DataManager {
public:
    DataManager(int, const std::string&);
    void readData();
    void getRegistrationData(struct ClientData&);
    void setSamplePeriod(unsigned short);
    void clearData();
    void setThreshold(unsigned short);
    void terminate();
    void sendToServer();
private:
    FileManager mFileManager;
    unsigned short mThreshold;
    int mClientSocket;
    std::mutex mDataMutex;
    unsigned short mSampleTime;
    bool mTerminate;
    unsigned short mLastAqiValue;
    bool isNotificationNeeded(unsigned short);
    struct ClientData generateData();
    void sendData(const struct ClientData&);
    //Remove the code after "beaglebone" to run it on a PC
    //beaglebone
    //AnalogReader mTempReader;
};
#endif //BOSCH_ACADEMY_DATAREADER_H