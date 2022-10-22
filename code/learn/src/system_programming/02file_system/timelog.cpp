#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>

const char* logfile = "/tmp/log";

/**
 * @brief 要求能够续写之前的log，不会覆盖！
 *
 * @return int
 */
int main() {
    // 有则打开追加，无则创建
    FILE* fp = fopen(logfile, "a+");
    if (!fp) {
        fprintf(stderr, "open file filed!\n");
        exit(EXIT_FAILURE);
    }
    int lines = 0;
    char linebuff[BUFSIZ];
    while (fgets(linebuff, BUFSIZ, fp)) {
        lines++;
    }

    while (true) {
        time_t stamp = time(nullptr);
        struct tm* sstime = localtime(&stamp);
        if (!sstime) {
            fprintf(stderr, "get local time failed!\n");
            exit(EXIT_FAILURE);
        }

#if 0
        // 使用普通的字符串格式化写入log
        fprintf(fp, "%d %d-%d-%d %d:%d:%02d\n", ++lines, sstime->tm_year,
                sstime->tm_mon, sstime->tm_yday, sstime->tm_hour,
                sstime->tm_min, sstime->tm_sec);
#endif

        // 使用strformat构造格式化时间字符串
        const char* timeFormat = "%Y-%m-%d %H:%M:%S";
        char timeFormated[BUFSIZ];
        if (strftime(timeFormated, BUFSIZ, timeFormat, sstime) == 0) {
            fprintf(stderr, "strftime failede!\n");
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "%d %s\n", ++lines, timeFormated);

        fflush(fp);
        sleep(1);
    }

    fclose(fp);

    return 0;
}