//
// Created by User on 03/05/2024.
//

#include "FileOpeningException.hpp"

FileOpeningException::FileOpeningException(const char* message): m_message{message}{}

const char* FileOpeningException::what() const noexcept {
    return m_message.c_str();
}