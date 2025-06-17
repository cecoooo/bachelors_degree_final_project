#include "AnalogSensorException.hpp"

AnalogSensorException::AnalogSensorException(const char* message): mMessage{message}{}

const char* AnalogSensorException::what() const noexcept {
    return mMessage.c_str();
}