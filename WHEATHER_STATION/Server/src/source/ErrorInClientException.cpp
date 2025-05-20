//
// Created by User on 02/05/2024.
//

#include "ErrorInClientException.hpp"

ErrorInClientException::ErrorInClientException(const char * mess): m_message{mess} {}

const char *ErrorInClientException::what() const noexcept {
return m_message.c_str();
}