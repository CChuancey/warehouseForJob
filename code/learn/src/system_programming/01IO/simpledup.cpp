#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>

#define FNAME "/tmp/out"

int main() {
    const int kMode = 0600;
    int fd = open(FNAME, O_CREAT | O_TRUNC | O_WRONLY, kMode);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 下面两行不严谨，涉及到并发时可能会出错
    // close(STDOUT_FILENO);
    // dup(fd);

    // 应换成dup2
    dup2(fd, STDOUT_FILENO);

    printf("const char *__restrict format, ...\n");

    return 0;
}