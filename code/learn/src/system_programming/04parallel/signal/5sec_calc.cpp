#include <unistd.h>

#include <csignal>
#include <cstdint>
#include <cstdio>
#include <ctime>

namespace m1 {
void method() {
    const int kInterval = 5;

    time_t to = time(nullptr) + kInterval;
    int64_t val = 0;
    while (time(nullptr) < to) {
        val++;
    }
    printf("%ld\n", val);
}
};  // namespace m1

namespace m2 {
bool flag = true;
void sighandler(int s) { flag = false; }

void calc() {
    int64_t val = 0;
    while (flag) {
        val++;
    }
    printf("%ld\n", val);
}
};  // namespace m2

void method2() {}

int main() {
    // method1
    {
        // m1::method();
    }

    // method2
    {
        const int kInterval = 5;
        alarm(kInterval);
        signal(SIGALRM, m2::sighandler);
        m2::calc();
    }

    return 0;
}