#include "ConnectionException.hpp"


ConnectionException::ConnectionException(const char* message): mMessage{message}{}

const char* ConnectionException::what() const noexcept {
    return mMessage.c_str();
}