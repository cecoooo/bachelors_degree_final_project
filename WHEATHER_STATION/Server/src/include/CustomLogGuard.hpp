//
// Created by PC on 4/30/2024.
//

#ifndef BOSCH_PROJECT_MASTER_DEMOGUARD_H
#define BOSCH_PROJECT_MASTER_DEMOGUARD_H

#include <mutex>
#include <condition_variable>


class CustomLogGuard{
public:
    std::mutex g_mutex;
    std::condition_variable g_cv;
    bool g_ready{false};
};

#endif //BOSCH_PROJECT_MASTER_DEMOGUARD_H
