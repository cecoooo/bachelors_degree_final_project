#include "ServerConnection.hpp"




int main()
{
    ServerConnection s1("192.168.56.1", "../src/measurements.txt");
    s1.connectToServer();
    return 0;
}