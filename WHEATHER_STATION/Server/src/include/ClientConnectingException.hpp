//
// Created by User on 02/05/2024.
//

#ifndef BOSCH_PROJECT_CLIENTCONNECTINGEXCEPTION_H
#define BOSCH_PROJECT_CLIENTCONNECTINGEXCEPTION_H

#include <stdexcept>
#include <iostream>
#include <string>

class ClientConnectingException: public std::exception {
private:
    std::string message;
public:
    ClientConnectingException(const char* message);
    const char* what() const noexcept override;
};


#endif //BOSCH_PROJECT_CLIENTCONNECTINGEXCEPTION_H
