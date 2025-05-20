//
// Created by User on 02/05/2024.
//

#include "SocketBindingException.hpp"

SocketBindingException::SocketBindingException(const char* mess): m_message{mess} {}

const char *SocketBindingException::what() const noexcept {
    return m_message.c_str();
}