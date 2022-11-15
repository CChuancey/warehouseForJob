#ifndef CODE_LEARN_SRC_MUDUO_INC_THREAD
#define CODE_LEARN_SRC_MUDUO_INC_THREAD

#include <pthread.h>

using callbk = void* (*)(void*);

class Thread {
public:
    explicit Thread(callbk);
    explicit Thread(callbk, void*);

    void join() const;
    void detach() const;
    void run() const;

private:
    pthread_t mTid;
    callbk mFunc;
    void* mArg;
};

#endif  // CODE_LEARN_SRC_MUDUO_INC_THREAD
