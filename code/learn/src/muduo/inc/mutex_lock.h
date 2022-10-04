#ifndef MUTEX_LOCK_HPP
#define MUTEX_LOCK_HPP

#include <pthread.h>

class MutexLock {
public:
    explicit MutexLock(pthread_mutex_t&);

    void lock();
    void unlock();

private:
    pthread_mutex_t& mMutex;
};

#endif
