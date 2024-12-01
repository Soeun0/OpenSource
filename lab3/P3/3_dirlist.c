#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void list_directory(const char *path);

int main(int argc, char *argv[]) {
    const char *start_path = "."; // 기본 경로는 현재 디렉토리

    // 명령행 인자로 디렉토리 경로를 제공받으면 그 경로로 시작
    if (argc > 1) {
        start_path = argv[1];
    }

    // 디렉토리 나열 시작
    list_directory(start_path);

    return 0;
}

void list_directory(const char *path) {
    DIR *dir;
    struct dirent *entry;

    // 디렉토리를 열기
    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        return;
    }

    printf("\n%s:\n", path); // 현재 디렉토리 경로 출력

    while ((entry = readdir(dir)) != NULL) {
        // 숨김 파일 및 현재/상위 디렉토리 제외
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        printf("%s  ", entry->d_name); // 파일/디렉토리 이름 출력
    }
    printf("\n");

    // 다시 디렉토리 스트림을 읽으며 하위 디렉토리 탐색
    rewinddir(dir);
    while ((entry = readdir(dir)) != NULL) {
        char sub_path[1024];

        // 숨김 파일 및 현재/상위 디렉토리 제외
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // 하위 디렉토리인지 확인
        snprintf(sub_path, sizeof(sub_path), "%s/%s", path, entry->d_name);
        struct stat statbuf;

        if (stat(sub_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            // 재귀적으로 하위 디렉토리 탐색
            list_directory(sub_path);
        }
    }

    closedir(dir);
}
