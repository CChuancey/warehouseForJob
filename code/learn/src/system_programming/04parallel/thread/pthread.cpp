#include <pthread.h>

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// 栈清理函数，相当于进程的atexit函数，但是给予了更多的机动性
void cleanup(void* arg) { printf("cleanup():%s\n", static_cast<char*>(arg)); }

void* routine(void* arg) {
    puts("thread!");
    // 线程退出应使用专用的函数

    // push pop 必须成对出现
    pthread_cleanup_push(cleanup, (void*)("1"));
    pthread_cleanup_push(cleanup, (void*)("2"));
    pthread_cleanup_push(cleanup, (void*)("3"));
    pthread_cleanup_push(cleanup, (void*)("4"));

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(1);

    pthread_exit(nullptr);
    return arg;
}

int main() {
    puts("Begin");

    pthread_t tid;
    // 返回值并不是线程的id
    int ret = pthread_create(&tid, nullptr, routine, nullptr);
    if (ret < 0) {
        fprintf(stderr, "error:%s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    // 不join可能routine并没有被调度到就结束了程序，就没有routine线程的事了，原因是return时会回收当前进程的资源（包括routine线程的资源）！！！
    pthread_join(tid, nullptr);
    // pthread_detach(tid);

    puts("End!");
    return 0;
}