#include "Server.hpp"

int main() {
    ClientContainer clientContainer;
    Server server;
    server.start(clientContainer);
    return 0;
}
