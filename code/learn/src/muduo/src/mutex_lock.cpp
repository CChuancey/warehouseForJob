#include "mutex_lock.h"

#include <pthread.h>

MutexLock::MutexLock(pthread_mutex_t& mutex) : mMutex(mutex) {}

void MutexLock::lock() {}

void MutexLock::unlock() {}