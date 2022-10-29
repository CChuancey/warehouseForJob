#include <unistd.h>

#include <csignal>
#include <cstdint>
#include <cstdio>
#include <iostream>

void status(int s) {
    std::cout << "Hello" << std::endl;
    std::cout << s << std::endl;
}

int main() {
    signal(SIGINT, SIG_IGN);
    for (int i = 0; i < INT16_MAX; i++) {
        write(STDOUT_FILENO, "*", 1);
        sleep(1);
    }
    return 0;
}