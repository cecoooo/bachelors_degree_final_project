#include <cstring>
#include <chrono>
#include <thread>
#include <iostream>

#include "DataManager.hpp"
#include "ClientData.hpp"
#include "AnalogSensorException.hpp"
#include "MutexGuard.hpp"
#include "FileOpeningException.hpp"

#define SIZE_OF_CLIENT_DATA 20

// beaglebone
// DataManager::DataManager(int socket, const std::string& fileName):mClientSocket(socket), mFileManager{socket, fileName}, mTerminate{false}, mTempReader{}{}
//PC
DataManager::DataManager(int socket, const std::string& fileName):mClientSocket(socket), mFileManager{socket, fileName}, mTerminate{false}{}

void DataManager::readData() {
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(mSampleTime));

        ClientData cd = generateData();
        try{
            mFileManager.writeData(cd);
        }catch(FileOpeningException& ex){
            std::cout << "Data wasn't written in the file" << std::endl;
        }
        if(isNotificationNeeded(cd.AQI)){
            sendData(cd);
        }
        if(mTerminate){
            break;
        }
    }
}

void DataManager::getRegistrationData(ClientData& toFill) {
    // Remove the aqiDetection_init() to run it on a PC.
    //AQI::aqiDetection_init();
    toFill = generateData();
    mLastAqiValue = toFill.AQI;
}

void DataManager::setSamplePeriod(unsigned short sample_period) {
    MutexGuard lock(mDataMutex);
    this->mSampleTime = sample_period;
}

void DataManager::setThreshold(unsigned short threshold){
    MutexGuard lock(mDataMutex);
    this->mThreshold = threshold;
}

void DataManager::clearData() {
    mFileManager.clearFile();
}

void DataManager::terminate() {
    mTerminate = true;
}

void DataManager::sendToServer() {
    MutexGuard lock(mDataMutex);
    try{
        mFileManager.sendFile();
    } catch(FileOpeningException& ex){
        std::string fileProblem = "Couldn't access the log file!";
        send(mClientSocket, fileProblem.c_str(), fileProblem.length() + 1, 0);
        mFileManager.sendTerminationSignal();
    }
}

bool DataManager::isNotificationNeeded(unsigned short newAqiValue){
    int AqiDelta = std::abs((newAqiValue - mLastAqiValue));
    mLastAqiValue = newAqiValue;
    if(AqiDelta > 250 || newAqiValue >= mThreshold){
        return true;
    }return false;
}

ClientData DataManager::generateData() {
    //beaglebone
    // AQI::AQIData aqiData;
    // aqiData = AQI::triggerPollutionEvent();
    // ClientData cd;
    // cd.latitude = aqiData.latitude;
    // cd.longitude = aqiData.longtitude;
    // cd.AQI = aqiData.aqiValue;
    // try{
    //     cd.temperature = mTempReader.readTemperature();
    // }catch (AnalogSensorException& ex){
    //     cd.temperature = -273;
    // }
    // return cd;
    // PC
   ClientData cd;
   cd.latitude = 212;
   cd.longitude = 234;
   cd.AQI = 30;
   try{
       cd.temperature = 23;
   }catch (AnalogSensorException& ex){
       cd.temperature = -273;
   }
   return cd;
}

void DataManager::sendData(const struct ClientData &cd) {
    char buffer[SIZE_OF_CLIENT_DATA];
    memcpy(buffer, &cd, sizeof(buffer));
    send(mClientSocket, buffer, sizeof(buffer) , 0);
}