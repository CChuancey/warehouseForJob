#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

/**
 * @brief 删除输入文件的第十行，默认文件大于十行
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage ...\n");
        exit(EXIT_FAILURE);
    }

    FILE* fp1 = fopen(argv[1], "r");
    // 注意：这里必须要以r+的方式打开,rw的方式无法写入文件！！！
    FILE* fp2 = fopen(argv[1], "r+");

    if (!fp1 || !fp2) {
        fprintf(stderr, "fopen failed");
        exit(EXIT_FAILURE);
    }

    size_t sumSize = 0;
    // fd1 读，fd2 写
    size_t num = BUFSIZ;
    char* buf = (char*)malloc(num);
    const int kLines = 9;
    for (int i = 0; i < kLines; i++) {
        ssize_t len1 = getline(&buf, &num, fp1);
        ssize_t len2 = getline(&buf, &num, fp2);
        sumSize += len1;
    }
    ssize_t line10 = getline(&buf, &num, fp1);

    while (fgets(buf, BUFSIZ, fp1)) {
        sumSize += strlen(buf);
        fputs(buf, fp2);
        fflush(fp2);
    }
    truncate(argv[1], sumSize - line10);

    fclose(fp2);
    fclose(fp1);
    free(buf);

    return 0;
}