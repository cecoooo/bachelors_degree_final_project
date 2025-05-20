//
// Created by User on 02/05/2024.
//

#ifndef BOSCH_PROJECT_SOCKETCREATINGEXCEPTION_H
#define BOSCH_PROJECT_SOCKETCREATINGEXCEPTION_H

#include <stdexcept>
#include <iostream>
#include <string>

class SocketCreatingException: public std::exception {
private:
    std::string m_message;
public:
    SocketCreatingException(const char* message);
    const char* what() const noexcept override;
};


#endif //BOSCH_PROJECT_SOCKETCREATINGEXCEPTION_H
