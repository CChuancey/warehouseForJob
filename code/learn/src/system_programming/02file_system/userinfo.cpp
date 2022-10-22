#include <pwd.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage...\n");
        exit(EXIT_FAILURE);
    }

    struct passwd* pd = getpwnam(argv[1]);

    // 在/etc/passwd中看不到实际的密码，加密后的密码存放在/etc/shadow中
    printf("passwd: %s\n", pd->pw_passwd);

    return 0;
}