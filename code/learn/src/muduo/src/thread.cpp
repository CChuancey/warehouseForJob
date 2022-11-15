#include "thread.h"

#include <pthread.h>

static void* exec(void* arg) {
    auto* thread = reinterpret_cast<Thread*>(arg);
    thread->run();
    return nullptr;
}

Thread::Thread(callbk func) : mFunc(func) {
    pthread_create(&mTid, nullptr, exec, this);
}

void Thread::join() const { pthread_join(mTid, nullptr); }

void Thread::detach() const { pthread_detach(mTid); }

void Thread::run() const { mFunc(mArg); }