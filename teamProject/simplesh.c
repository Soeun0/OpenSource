#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

// 함수 선언
void cmd_ls(char **argv);
void cmd_pwd();
void cmd_cd(char **argv);
void cmd_mkdir(char **argv);
void cmd_rmdir(char **argv);
void cmd_ln(char **argv);
void cmd_cp(char **argv);
void cmd_rm(char **argv);
void cmd_mv(char **argv);
void cmd_cat(char **argv);

int getargs(char *cmd, char **argv);
void execute_command(char **argv);

int main() {
    char buf[256];
    char *argv[50];
    int narg;

    while (1) {
        printf("shell> ");
        fflush(stdout);

        // 사용자 입력 받기
        if (fgets(buf, sizeof(buf), stdin) == NULL) break;
        buf[strcspn(buf, "\n")] = '\0'; // 개행 문자 제거

        // "exit" 명령어 처리
        if (strcmp(buf, "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        narg = getargs(buf, argv);
        if (narg == 0) continue; // 빈 입력 처리

        execute_command(argv);
    }
    return 0;
}

int getargs(char *cmd, char **argv) {
    int narg = 0;
    while (*cmd) {
        if (*cmd == ' ' || *cmd == '\t') {
            *cmd++ = '\0';
        } else {
            argv[narg++] = cmd++;
            while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t') {
                cmd++;
            }
        }
    }
    argv[narg] = NULL;
    return narg;
}

void execute_command(char **argv) {
    if (strcmp(argv[0], "ls") == 0) {
        cmd_ls(argv);
    } else if (strcmp(argv[0], "pwd") == 0) {
        cmd_pwd();
    } else if (strcmp(argv[0], "cd") == 0) {
        cmd_cd(argv);
    } else if (strcmp(argv[0], "mkdir") == 0) {
        cmd_mkdir(argv);
    } else if (strcmp(argv[0], "rmdir") == 0) {
        cmd_rmdir(argv);
    } else if (strcmp(argv[0], "ln") == 0) {
        cmd_ln(argv);
    } else if (strcmp(argv[0], "cp") == 0) {
        cmd_cp(argv);
    } else if (strcmp(argv[0], "rm") == 0) {
        cmd_rm(argv);
    } else if (strcmp(argv[0], "mv") == 0) {
        cmd_mv(argv);
    } else if (strcmp(argv[0], "cat") == 0) {
        cmd_cat(argv);
    } else {
        printf("Command not found: %s\n", argv[0]);
    }
}

// ls: 디렉토리 내용 나열
void cmd_ls(char **argv) {
    DIR *dir;
    struct dirent *entry;
    char *path = argv[1] ? argv[1] : ".";

    if ((dir = opendir(path)) == NULL) {
        perror("ls");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s  ", entry->d_name);
    }
    printf("\n");
    closedir(dir);
}

// pwd: 현재 디렉토리 출력
void cmd_pwd() {
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}

// cd: 디렉토리 변경
void cmd_cd(char **argv) {
    if (argv[1]) {
        if (chdir(argv[1]) != 0) {
            perror("cd");
        }
    } else {
        printf("Usage: cd <directory>\n");
    }
}

// mkdir: 디렉토리 생성
void cmd_mkdir(char **argv) {
    if (argv[1]) {
        if (mkdir(argv[1], 0755) != 0) {
            perror("mkdir");
        }
    } else {
        printf("Usage: mkdir <directory>\n");
    }
}

// rmdir: 디렉토리 삭제
void cmd_rmdir(char **argv) {
    if (argv[1]) {
        if (rmdir(argv[1]) != 0) {
            perror("rmdir");
        }
    } else {
        printf("Usage: rmdir <directory>\n");
    }
}

// ln: 하드 링크 생성
void cmd_ln(char **argv) {
    if (argv[1] && argv[2]) {
        if (link(argv[1], argv[2]) != 0) {
            perror("ln");
        }
    } else {
        printf("Usage: ln <source> <destination>\n");
    }
}

// cp: 파일 복사
void cmd_cp(char **argv) {
    if (argv[1] && argv[2]) {
        int src = open(argv[1], O_RDONLY);
        if (src < 0) {
            perror("cp");
            return;
        }
        int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dest < 0) {
            perror("cp");
            close(src);
            return;
        }

        char buf[1024];
        ssize_t bytes;
        while ((bytes = read(src, buf, sizeof(buf))) > 0) {
            write(dest, buf, bytes);
        }

        close(src);
        close(dest);
    } else {
        printf("Usage: cp <source> <destination>\n");
    }
}

// rm: 파일 삭제
void cmd_rm(char **argv) {
    if (argv[1]) {
        if (remove(argv[1]) != 0) {
            perror("rm");
        }
    } else {
        printf("Usage: rm <file>\n");
    }
}

// mv: 파일 이동/이름 변경
void cmd_mv(char **argv) {
    if (argv[1] && argv[2]) {
        struct stat statbuf;

        // destination이 디렉토리인지 확인
        if (stat(argv[2], &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            // destination이 디렉토리일 경우 source 파일을 해당 디렉토리로 이동
            char dest_path[1024];
            snprintf(dest_path, sizeof(dest_path), "%s/%s", argv[2], argv[1]);

            if (rename(argv[1], dest_path) != 0) {
                perror("mv");
            } else {
                printf("Moved '%s' to '%s'\n", argv[1], dest_path);
            }
        } else {
            // destination이 디렉토리가 아니면 단순히 파일 이름 변경
            if (rename(argv[1], argv[2]) != 0) {
                perror("mv");
            } else {
                printf("Moved '%s' to '%s'\n", argv[1], argv[2]);
            }
        }
    } else {
        printf("Usage: mv <source> <destination>\n");
    }
}

// cat: 파일 내용 출력
void cmd_cat(char **argv) {
    if (argv[1]) {
        FILE *file = fopen(argv[1], "r");
        if (file) {
            char line[256];
            while (fgets(line, sizeof(line), file)) {
                printf("%s", line);
            }
            fclose(file);
        } else {
            perror("cat");
        }
    } else {
        printf("Usage: cat <file>\n");
    }
}
