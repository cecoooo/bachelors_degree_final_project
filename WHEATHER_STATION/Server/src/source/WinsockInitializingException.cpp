//
// Created by User on 02/05/2024.
//

#include "WinsockInitializingException.hpp"

WinsockInitializingException::WinsockInitializingException(const char* mess): m_message{mess} {}

const char *WinsockInitializingException::what() const noexcept {
    return m_message.c_str();
}