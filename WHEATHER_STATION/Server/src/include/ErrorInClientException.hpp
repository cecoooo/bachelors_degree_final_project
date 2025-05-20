//
// Created by User on 02/05/2024.
//

#ifndef BOSCH_PROJECT_ERRORINCLIENTEXCEPTION_H
#define BOSCH_PROJECT_ERRORINCLIENTEXCEPTION_H

#include <stdexcept>
#include <iostream>
#include <string>

class ErrorInClientException: public std::exception {
private:
    std::string m_message;
public:
    ErrorInClientException(const char* message);
    const char* what() const noexcept override;
};


#endif //BOSCH_PROJECT_ERRORINCLIENTEXCEPTION_H
