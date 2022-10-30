#include <unistd.h>

#include <csignal>
#include <cstdio>

// ps axf 查看进程号，使用kill -44 pid 给进程发送实时信号
#define MYSIG SIGRTMIN + 10

void sighandler(int s) {
    if (s != SIGINT) {
        return;
    }
    write(STDOUT_FILENO, "!", 1);
}

int main() {
    signal(MYSIG, sighandler);

    const int kNum = 10;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    // 为了确保程序结束前后的信号阻塞状态一致，需要
    // 在修改阻塞信号时保存状态。
    sigset_t preset;
    sigprocmask(SIG_UNBLOCK, &set, &preset);

    // 写法1
    // {
    //     for (;;) {
    //         sigprocmask(SIG_BLOCK, &set, nullptr);
    //         for (int i = 0; i < kNum; i++) {
    //             write(STDOUT_FILENO, "*", 1);
    //             sleep(1);
    //         }
    //         sigprocmask(SIG_UNBLOCK, &set, nullptr);
    //         write(STDOUT_FILENO, "\n", 1);
    //     }
    // }

    // 写法二
    {
        sigset_t oldSet;
        for (;;) {
            sigprocmask(SIG_BLOCK, &set, &oldSet);
            for (int i = 0; i < kNum; i++) {
                write(STDOUT_FILENO, "*", 1);
                sleep(1);
            }
            // 实时信号不会丢失，不需要sigsuspend
            sigprocmask(SIG_SETMASK, &oldSet, nullptr);
            // 只靠pause无法屏蔽for循环中的信号
            // pause();
            // sigsuspend(&oldSet);

            write(STDOUT_FILENO, "\n", 1);
        }
    }

    // 恢复之前的状态
    sigprocmask(SIG_SETMASK, &preset, nullptr);
    return 0;
}