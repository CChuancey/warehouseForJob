#ifndef CODE_LEARN_SRC_MUDUO_INC_REQUEST
#define CODE_LEARN_SRC_MUDUO_INC_REQUEST

#include "mutex_lock_guard.h"

class Request {
public:
    void process();

    ~Request() __attribute__((noinline));

    void print() const __attribute__((noinline));

private:
    mutable MutexLock mMutex;
};  // namespace Request

#endif  // CODE_LEARN_SRC_MUDUO_INC_REQUEST
