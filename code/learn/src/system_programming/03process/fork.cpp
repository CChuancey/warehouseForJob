#include <unistd.h>

#include <csignal>
#include <cstdio>
#include <cstdlib>

int main() {
    printf("Begin\n");

    // fork（）
    // 前必须要执行fflush(NULL)刷新先前打开的所有流，否则因为缓冲的原因，
    // Begin会被拷贝到子进程的缓冲区进而出现打印两次Begin的情况
    // 测试：./fork1 > /tmp/out
    fflush(nullptr);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork()");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // child
        printf("child[%lld] is working!\n", static_cast<long long>(getpid()));
    } else {
        printf("Parent[%lld] is working!\n", static_cast<long long>(getpid()));
    }
    printf("[%lld] end!\n", static_cast<long long>(getpid()));

    return 0;
}