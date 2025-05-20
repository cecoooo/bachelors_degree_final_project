//
// Created by User on 03/05/2024.
//

#ifndef BOSCH_PROJECT_FILEOPENINGEXCEPTION_H
#define BOSCH_PROJECT_FILEOPENINGEXCEPTION_H


#include <iostream>
#include <stdexcept>
#include <string>

class FileOpeningException: public std::exception{
public:
    FileOpeningException(const char* message);
    const char* what() const noexcept override;
private:
    std::string m_message;
};


#endif //BOSCH_PROJECT_FILEOPENINGEXCEPTION_H
