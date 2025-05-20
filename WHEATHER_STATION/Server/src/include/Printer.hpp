//
// Created by User on 02/05/2024.
//

#ifndef BOSCH_PROJECT_PRINTER_H
#define BOSCH_PROJECT_PRINTER_H
#include <iostream>


class Printer {
public:
    const char* cantInitializeWinsockMessage();
    const char* cantCreateSocketMessage();
    const char* cantBindToIPPortMessage();
    const char*  cantListenMessage();
    const char* cantOpenFile();
    void serverListeningOnPortMessage(int);
    void commandIsDoneMessage();
    const char* errorInClientMessage();
    const char* problemWithClientConnectingMessage();
    void enterNumberBetween1And65535Message();
    void enterNumberBetween0And500Message();
    void seeListOfAllClientsMessage();
    void areYouSureYouWantToStopTheServerMessage();
    void serverSwitchedOffMessage();
    void commandUnrecognizedMessage();
    void enterNumberOfClientYouWantToCommunicateMessage();
    void whatDoYouWantToDoMessage();
    void clientWithThisNumberDoesNotExistsInTheListMessage();
    void invalidCommandOrLostConnectionMessage();
    void noConnectedClientsAtTheMoment();
};


#endif //BOSCH_PROJECT_PRINTER_H
