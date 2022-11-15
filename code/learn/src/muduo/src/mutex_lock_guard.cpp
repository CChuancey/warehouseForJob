#include "mutex_lock_guard.h"

#include "mutex_lock.h"

MutexLockGurad::MutexLockGurad(MutexLock& mutex) : mMutex(mutex) {
    // 创建时即上锁
    mMutex.lock();
}

MutexLockGurad::~MutexLockGurad() { mMutex.unlock(); }