//
// Created by User on 02/05/2024.
//

#include "CustomMutex.hpp"

CustomMutex::CustomMutex(std::mutex& m): m_mtx{m} {
    this->m_mtx.lock();
}

CustomMutex::~CustomMutex() {
    this->m_mtx.unlock();
}