#include <glob.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int errfunc(const char *errorpath, int errorno) {
    return fprintf(stderr, "error path:%s\n error reason: %s\n", errorpath,
                   strerror(errorno));
}

static bool isValid(const char *path) {
    // 得到filenmae ==> strrchr
    // puts(path);
    const char *fileName = strrchr(path, '/');
    if (!fileName) {
        fprintf(stderr, "path format error!\n");
        exit(EXIT_FAILURE);
    }
    // printf("dir : %s\n", fileName + 1);
    return !(strcmp(".", fileName + 1) == 0 || strcmp("..", fileName + 1) == 0);
}

static int64_t getSize(const char *path) {
    // puts(path);
    int64_t sum = 0;

    glob_t globres;
    char pattern[BUFSIZ];
    sprintf(pattern, "%s/*", path);
    int ret = glob(pattern, GLOB_NOSORT, errfunc, &globres);
    if (ret) {
        exit(EXIT_FAILURE);
    }
    // 将隐藏文件也计入大小，pattern 为xx/*时不会匹配xx目录下的隐藏文件
    sprintf(pattern, "%s/.*", path);
    ret = glob(pattern, GLOB_NOSORT | GLOB_APPEND, errfunc, &globres);
    if (ret) {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < globres.gl_pathc; i++) {
        struct stat st;
        if (lstat(globres.gl_pathv[i], &st) < 0) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        // 普通文件
        if (S_ISREG(st.st_mode)) {
            sum += st.st_blocks;
            // printf("files: %s\n", globres.gl_pathv[i]);
        } else if (S_ISDIR(st.st_mode) &&
                   isValid(globres.gl_pathv[i])) {  // 目录文件
            // printf("path: %s\n", globres.gl_pathv[i]);
            sum += getSize(globres.gl_pathv[i]);
        }
    }

    // 不要忘记释放glob申请的内存
    globfree(&globres);
    return sum;
}

// 统计给定目录的大小
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage ...\n");
        exit(EXIT_FAILURE);
    }

    printf("path:%s,size:%ldKB\n", argv[1], getSize(argv[1]) / 2);

    return 0;
}