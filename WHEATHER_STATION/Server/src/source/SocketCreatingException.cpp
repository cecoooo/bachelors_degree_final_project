//
// Created by User on 02/05/2024.
//

#include "SocketCreatingException.hpp"

SocketCreatingException::SocketCreatingException(const char * mess): m_message{mess} {}

const char *SocketCreatingException::what() const noexcept {
    return m_message.c_str();
}