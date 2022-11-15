#include <unistd.h>

#include <cstdio>
#include <set>

#include "inventory.h"
#include "mutex_lock.h"
#include "mutex_lock_guard.h"
#include "request.h"
#include "thread.h"

void* threadFunc(void* /*arg*/) {
    // printf("thread func working!\n");
    auto* req = new Request;
    req->process();
    // printf("thread func work over!\n");
    sleep(2);
    delete req;
    return nullptr;
}

extern Inventory gInventory;

int main() {
    Thread thread(threadFunc);

    // usleep(500 * 1000);
    sleep(1);
    gInventory.printAll();

    thread.join();
    return 0;
}
