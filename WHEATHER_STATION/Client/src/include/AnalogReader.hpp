#ifndef BOSCH_PROJECT_ANALOGREADER_H
#define BOSCH_PROJECT_ANALOGREADER_H
#include "ADC.hpp"

class AnalogReader {
public:
    short readTemperature();
private:
    ADC mAnalogReader;
    short voltageToNumber(const float value);
};


#endif //BOSCH_PROJECT_ANALOGREADER_H
