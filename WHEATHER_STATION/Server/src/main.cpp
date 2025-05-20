#include "Server.hpp"

int main() {
    ClientContainer clientContainer;
    Server *server = new Server();
    server->start(std::ref(clientContainer));
    delete server;
    return 0;
}
