#include "ADC.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include "ADC.hpp"


int main(int /*argc*/, char** /*argv*/)
{
    using namespace std::chrono_literals;
    ADC analog_wr;
	bool isInit = analog_wr.init();
	
	if(isInit)
	{
		bool status{true};
		for (int i = 0; i < 110; ++i) {
			float value;
			std::this_thread::sleep_for(200ms);
			status = analog_wr.getAIN(Analog_IN::AIN1, value);
			if(status)
				std::cout << "Value of pin P9_40 (beaglebone analog pin 1) is " << value << std::endl;
			else
				std::cout << "Failed to retrieve value from pin P9_40 (beaglebone analog pin 1) " << std::endl;
		}
	}
	
    return 0;
}
