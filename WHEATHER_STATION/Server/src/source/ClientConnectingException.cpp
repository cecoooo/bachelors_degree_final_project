//
// Created by User on 02/05/2024.
//

#include "ClientConnectingException.hpp"

ClientConnectingException::ClientConnectingException(const char * mess): message{mess} {}

const char *ClientConnectingException::what() const noexcept {
    return message.c_str();
}