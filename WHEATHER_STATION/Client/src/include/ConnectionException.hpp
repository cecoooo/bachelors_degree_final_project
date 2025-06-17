#ifndef BOSCH_PROJECT_CONNECTIONEXCEPTION_H
#define BOSCH_PROJECT_CONNECTIONEXCEPTION_H

#include <iostream>
#include <stdexcept>
#include <string>

class ConnectionException: public std::exception{
public:
    ConnectionException(const char* message);
    const char* what() const noexcept override;
private:
    std::string mMessage;
};


#endif //BOSCH_PROJECT_CONNECTIONEXCEPTION_H
