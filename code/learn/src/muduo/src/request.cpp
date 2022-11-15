#include "request.h"

#include <unistd.h>

#include "inventory.h"

Inventory gInventory;

void Request::process()  //__attribute__((noinline));
{
    MutexLockGurad lockGuard(mMutex);
    gInventory.add(this);
}

Request::~Request() {
    MutexLockGurad lockGuard(mMutex);
    gInventory.remove(this);
}

void Request::print() const {
    MutexLockGurad lockGuard(mMutex);
    // print sth
}
