#include <pthread.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

const char* kFileName = "/tmp/out";

// 静态初始化mutex互斥量
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void* add(void* arg) {
    FILE* fp = fopen(kFileName, "r+");
    if (!fp) {
        perror("fopen()");
        exit(EXIT_FAILURE);
    }

    char buf[BUFSIZ];
    pthread_mutex_lock(&mutex);
    fgets(buf, BUFSIZ, fp);
    fseek(fp, 0, SEEK_SET);
    int v = atoi(buf) + 1;
    // sleep 可以调试多线程的资源竞争
    // sleep(1);
    fprintf(fp, "%d\n", v);
    fflush(fp);
    pthread_mutex_unlock(&mutex);

    pthread_exit(arg);
    return arg;
}

int main() {
    const int kThreadNum = 8;
    pthread_t tid[kThreadNum];
    for (int i = 0; i < kThreadNum; i++) {
        int ret = pthread_create(tid + i, nullptr, add, nullptr);
        if (ret > 0) {
            fprintf(stderr, "pthread_create():%s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    for (pthread_t i : tid) {
        pthread_join(i, nullptr);
    }

    // 销毁互斥量
    pthread_mutex_destroy(&mutex);
    return 0;
}