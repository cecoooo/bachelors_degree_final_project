//
// Created by User on 02/05/2024.
//

#ifndef BOSCH_PROJECT_CUSTOMMUTEX_H
#define BOSCH_PROJECT_CUSTOMMUTEX_H

#include <mutex>

class CustomMutex {
private:
    std::mutex& m_mtx;
public:
    CustomMutex(std::mutex&);
    ~CustomMutex();
};


#endif //BOSCH_PROJECT_CUSTOMMUTEX_H
