//
// Created by User on 02/05/2024.
//

#include "ServerListeningException.hpp"

ServerListeningException::ServerListeningException(const char * mess): m_message{mess} {}

const char *ServerListeningException::what() const noexcept {
    return m_message.c_str();
}