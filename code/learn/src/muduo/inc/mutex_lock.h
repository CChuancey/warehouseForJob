#ifndef MUTEX_LOCK_HPP
#define MUTEX_LOCK_HPP

#include <pthread.h>

// 充当mutex的作用，提供lock和unlock接口
class MutexLock {
public:
    MutexLock();
    ~MutexLock();

    void lock();
    void unlock();

private:
    pthread_mutex_t mMutex;
};

#endif
