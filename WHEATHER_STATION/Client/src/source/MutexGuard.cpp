//
// Created by hristogarkov on 5/2/24.
//

#include "MutexGuard.hpp"

MutexGuard::MutexGuard(std::mutex &lock): mLock{lock} {
    mLock.lock();
}

MutexGuard::~MutexGuard() {
    mLock.unlock();
}