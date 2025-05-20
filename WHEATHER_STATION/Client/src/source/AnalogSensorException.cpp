//
// Created by hristogarkov on 5/2/24.
//

#include "AnalogSensorException.hpp"

AnalogSensorException::AnalogSensorException(const char* message): mMessage{message}{}

const char* AnalogSensorException::what() const noexcept {
    return mMessage.c_str();
}