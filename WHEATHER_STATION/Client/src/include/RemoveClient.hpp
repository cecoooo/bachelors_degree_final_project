#ifndef BOSCH_PROJECT_REMOVECLIENT_H
#define BOSCH_PROJECT_REMOVECLIENT_H



class RemoveClient {
public:
    void checkForDisconnect();
    RemoveClient(const int, bool&);
private:
    const int mClientSocket;
    bool& mToTerminate;
};


#endif //BOSCH_PROJECT_REMOVECLIENT_H
