//
// Created by hristogarkov on 4/22/24.
//

#include <string>
#include <iostream>
#include <sys/socket.h>

#include "RemoveClient.hpp"


RemoveClient::RemoveClient(const int socket, bool& terminateFlag):mClientSocket{socket}, mToTerminate{terminateFlag} {}

void RemoveClient::checkForDisconnect() {
    std::string command;
    while(true){
        std::cin >> command;
        send(mClientSocket, command.c_str(), command.length() + 1 , 0);
        if(command == "exit"){
            mToTerminate = true;
            break;
        }
    }
}