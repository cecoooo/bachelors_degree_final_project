#pragma once

#ifndef ADC_H
#define ADC_H

enum class Analog_IN : int
{
	AIN0 = 0,
	AIN1 = 1,
	AIN2 = 2,
	AIN3 = 3,
	AIN4 = 4,
	AIN5 = 5,
	AIN6 = 6,
	AIN_END = 7
};

class ADC
{
public:
	ADC();
	~ADC();
	
	bool init();
	
	bool getAIN(Analog_IN pin, float& val);
private:
	bool m_isInit{false};
};

#endif