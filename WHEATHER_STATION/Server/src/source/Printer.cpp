//
// Created by User on 02/05/2024.
//

#include "Printer.hpp"

const char* Printer::cantInitializeWinsockMessage(){
    return "Can't initialize Winsock! Quitting";
}

const char* Printer::cantCreateSocketMessage(){
    return "Can't create socket! Quitting";
}

const char* Printer::cantBindToIPPortMessage(){
    return "Can't bind to IP/port! Quitting";
}

const char* Printer::cantListenMessage(){
    return "Can't listen! Quitting";
}

const char* Printer::cantOpenFile(){
    return "Can't open the file.";
}

void Printer::serverListeningOnPortMessage(int port){
    std::cout << "Server listening on port " << port << std::endl;
}

void Printer::commandIsDoneMessage(){
    std::cout << "\ncommand is done!" << std::endl;
}

const char* Printer::errorInClientMessage(){
    return "\nerror in Client";
}

const char* Printer::problemWithClientConnectingMessage(){
    return "Problem with client connecting! Quitting";
}

void Printer::enterNumberBetween1And65535Message(){
    std::cout << "Enter number between 1 and 65 535" << std::endl;
}

void Printer::enterNumberBetween0And500Message(){
    std::cout << "Enter number between 0 and 500" << std::endl;
}

void Printer::seeListOfAllClientsMessage(){
    std::cout << "Refresh list of all clients. (PRESS ANY INTEGER, EXCEPT 0).\n0 - STOP THE SERVER.";
}

void Printer::areYouSureYouWantToStopTheServerMessage(){
    std::cout << "Are you sure you want to stop the server? y/n" << std::endl;
}

void Printer::serverSwitchedOffMessage(){
    std::cout << "SERVER SWITCHED OFF" << std::endl;
}

void Printer::commandUnrecognizedMessage(){
    std::cout << "command unrecognized." << std::endl;
}

void Printer::enterNumberOfClientYouWantToCommunicateMessage(){
    std::cout << "Enter number of client you want to communicate:";
}

void Printer::whatDoYouWantToDoMessage(){
    std::cout << "What do you want to do?" << std::endl << "0 = Set sample period\n"
                                                           "1 = Set threshold\n"
                                                           "2 = Reset log\n"
                                                           "3 = Download log" << std::endl;
}

void Printer::clientWithThisNumberDoesNotExistsInTheListMessage(){
    std::cout << "Client with this number does not exists in the list." << std::endl;
}

void Printer::invalidCommandOrLostConnectionMessage(){
    std::cout << "Invalid command or lost connection." << std::endl;
}

void Printer::noConnectedClientsAtTheMoment() {
    std::cout << "No connected clients at the moment." << std::endl;
}