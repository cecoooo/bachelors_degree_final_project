#include "FileOpeningException.hpp"

FileOpeningException::FileOpeningException(const char* message): mMessage{message}{}

const char* FileOpeningException::what() const noexcept {
    return mMessage.c_str();
}