//
// Created by hristogarkov on 5/1/24.
//
#include <ctime>
#include <chrono>

#include "TimeProvider.hpp"

std::string TimeProvider::getTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    return std::ctime(&currentTime);
}