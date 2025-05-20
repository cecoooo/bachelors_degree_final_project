#include  "ADC.hpp"

#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <thread>

#include "adafruit/bbio.h"
#include "adafruit/bbio/error.h"

namespace adafruit {
namespace bbio {

extern "C" {
	#include "event_gpio.h"
	#include "c_adc.h"
}
}
}

extern "C" {
	#include "common.h"
}

	
ADC::ADC()
{
	
}

ADC::~ADC()
{
	// adc_cleanup();
}

bool ADC::init()
{
	using adafruit::bbio::lib_options;
    using adafruit::bbio::Gpio;
	using namespace adafruit::bbio;
	
	adafruit::bbio::init(lib_options(LOG_DEBUG, nullptr, LOG_PERROR));
	adafruit::bbio::BBIO_err rc = adc_setup();
	m_isInit = (rc == BBIO_OK);
	return m_isInit;
}

bool ADC::getAIN(Analog_IN pin, float& val)
{
	using namespace adafruit::bbio;
	if(!m_isInit) return false;
	if(pin >= Analog_IN::AIN_END) return false;
	
	unsigned int ain = static_cast<unsigned int>(pin);
	adafruit::bbio::BBIO_err rc = read_value(ain, &val);
	return (rc == BBIO_OK);
}