#include "mutex_lock.h"

#include <pthread.h>

#include <cstdio>
#include <cstring>
#include <exception>

MutexLock::MutexLock() {
    int err = pthread_mutex_init(&mMutex, nullptr);
    if (err) {
        fprintf(stderr, "pthread_mutex_init():%s\n", strerror(err));
        std::terminate();
    }
}

MutexLock::~MutexLock() { pthread_mutex_destroy(&mMutex); }

void MutexLock::lock() { pthread_mutex_lock(&mMutex); }

void MutexLock::unlock() { pthread_mutex_unlock(&mMutex); }