#ifndef CLIENT_DATA_HPP
#define CLIENT_DATA_HPP

#pragma pack(push, 1)
struct ClientData {
    double latitude;
    double longitude;
    short aqi;
    short temperature;
};
#pragma pack(pop)

#endif
