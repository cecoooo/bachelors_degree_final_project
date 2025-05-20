#ifndef AQILIB_H__
#define AQILIB_H__
namespace AQI {

typedef struct AQIData {
	double latitude;
	double longtitude;
    short aqiValue;
} AQIData;

void aqiDetection_init();
AQIData triggerPollutionEvent();

};
#endif