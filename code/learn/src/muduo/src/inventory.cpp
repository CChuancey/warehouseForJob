
#include "inventory.h"

#include <unistd.h>

#include <cstdio>

#include "mutex_lock_guard.h"

void Inventory::add(Request* req) {
    MutexLockGurad lockGuard(mMutex);
    mRequestSet.insert(req);
}

void Inventory::remove(Request* req) {
    MutexLockGurad lockGuard(mMutex);
    mRequestSet.erase(req);
}

void Inventory::printAll() const {
    MutexLockGurad lockGuard(mMutex);
    sleep(2);
    for (const auto* item : mRequestSet) {
        item->print();
    }
    // printf("print all!\n");
}
