#include <glob.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

// 目录解析函数，不包括隐藏文件
const char* pattern = "/etc/*.conf";

static int errfunction(const char* errorpath, int errorno) {
    fprintf(stderr, "error reson : %s, error path: %s\n", strerror(errorno),
            errorpath);
    return 0;
}

int main() {
    glob_t globRes;
    int ret = glob(pattern, GLOB_NOSORT, errfunction, &globRes);

    if (ret) {
        perror("glob");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < globRes.gl_pathc; i++) {
        puts(globRes.gl_pathv[i]);
    }

    // 记得销毁空间
    globfree(&globRes);

    return 0;
}