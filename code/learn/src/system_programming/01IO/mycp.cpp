#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage...\n");
        exit(EXIT_FAILURE);
    }

    int sfd = open(argv[1], O_RDONLY);
    int dfd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY);
    if (sfd < 0 || dfd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (true) {
        char buf[BUFSIZ];
        ssize_t len = read(sfd, buf, BUFSIZ);
        if (len < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (len == 0) {
            break;
        }

        // 坚持写，一直写完读到的所有字节
        ssize_t wlen = 0;
        while (wlen < len) {
            ssize_t ret = write(dfd, buf + wlen, len);
            if (ret < 0) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            wlen += ret;
            len -= ret;
        }
    }

    // 打开的资源要释放!
    close(sfd);
    close(dfd);

    return 0;
}