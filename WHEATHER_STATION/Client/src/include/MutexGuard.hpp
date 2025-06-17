#ifndef BOSCH_PROJECT_MUTEXGUARD_H
#define BOSCH_PROJECT_MUTEXGUARD_H

#include <mutex>

class MutexGuard {
public:
    explicit MutexGuard(std::mutex&);
    ~MutexGuard();
private:
    std::mutex& mLock;
};


#endif //BOSCH_PROJECT_MUTEXGUARD_H
