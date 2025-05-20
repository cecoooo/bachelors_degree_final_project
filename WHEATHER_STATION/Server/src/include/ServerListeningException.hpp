//
// Created by User on 02/05/2024.
//

#ifndef BOSCH_PROJECT_SERVERLISTENINGEXCEPTION_H
#define BOSCH_PROJECT_SERVERLISTENINGEXCEPTION_H

#include <stdexcept>
#include <iostream>
#include <string>

class ServerListeningException: public std::exception {
private:
    std::string m_message;
public:
    ServerListeningException(const char* message);
    const char* what() const noexcept override;
};


#endif //BOSCH_PROJECT_SERVERLISTENINGEXCEPTION_H
