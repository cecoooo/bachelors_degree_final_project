//
// Created by User on 02/05/2024.
//

#ifndef BOSCH_PROJECT_WINSOCKINITIALIZINGEXCEPTION_H
#define BOSCH_PROJECT_WINSOCKINITIALIZINGEXCEPTION_H

#include <stdexcept>
#include <iostream>
#include <string>

class WinsockInitializingException: public std::exception{
private:
    std::string m_message;
public:
    WinsockInitializingException(const char*);
    const char* what() const noexcept override;
};


#endif //BOSCH_PROJECT_WINSOCKINITIALIZINGEXCEPTION_H
