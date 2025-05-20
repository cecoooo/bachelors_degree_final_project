//
// Created by hristogarkov on 4/22/24.
//

#include "AnalogReader.hpp"
#include "AnalogSensorException.hpp"

short AnalogReader::readTemperature() {
    bool isInit = mAnalogReader.init();
    float value;
    if(isInit){
        bool status{true};
        status = mAnalogReader.getAIN(Analog_IN::AIN1, value);
        short temperature = voltageToNumber(value);
        if(status)
            return temperature;
        else
            throw AnalogSensorException("Couldn't read analog input!");
    }
}

short AnalogReader::voltageToNumber(const float value) {
    float voltage = value * 1.8 / 4096.0;
    return (voltage - 0.5) / 0.01;
}