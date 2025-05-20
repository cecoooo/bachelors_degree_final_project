//
// Created by User on 02/05/2024.
//

#ifndef BOSCH_PROJECT_SOCKETBINDINGEXCEPTION_H
#define BOSCH_PROJECT_SOCKETBINDINGEXCEPTION_H

#include <stdexcept>
#include <iostream>
#include <string>

class SocketBindingException: public std::exception {
    std::string m_message;
public:
    SocketBindingException(const char*);
    const char* what() const noexcept override;
};


#endif //BOSCH_PROJECT_SOCKETBINDINGEXCEPTION_H
