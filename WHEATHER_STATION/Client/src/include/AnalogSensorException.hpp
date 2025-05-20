//
// Created by hristogarkov on 5/2/24.
//

#ifndef BOSCH_PROJECT_ANALOGSENSOREXCEPTION_H
#define BOSCH_PROJECT_ANALOGSENSOREXCEPTION_H

#include <iostream>
#include <stdexcept>
#include <string>

class AnalogSensorException: public std::exception{
public:
    AnalogSensorException(const char* message);
    const char* what() const noexcept override;
private:
    std::string mMessage;
};


#endif //BOSCH_PROJECT_ANALOGSENSOREXCEPTION_H
