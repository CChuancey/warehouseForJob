#ifndef CODE_LEARN_SRC_MUDUO_SRC_INVENTORY
#define CODE_LEARN_SRC_MUDUO_SRC_INVENTORY
#include <set>

#include "request.h"

class Inventory {
public:
    void add(Request* req);

    void remove(Request* req);

    void printAll() const;

private:
    mutable MutexLock mMutex;
    std::set<Request*> mRequestSet;
};

#endif  // CODE_LEARN_SRC_MUDUO_SRC_INVENTORY
