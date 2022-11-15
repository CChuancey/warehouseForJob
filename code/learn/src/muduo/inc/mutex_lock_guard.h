#ifndef CODE_LEARN_SRC_MUDUO_INC_MUTEX_LOCK_GUARD
#define CODE_LEARN_SRC_MUDUO_INC_MUTEX_LOCK_GUARD

#include "mutex_lock.h"
class MutexLockGurad {
public:
    explicit MutexLockGurad(MutexLock&);
    ~MutexLockGurad();

private:
    MutexLock& mMutex;
};

#endif  // CODE_LEARN_SRC_MUDUO_INC_MUTEX_LOCK_GUARD
