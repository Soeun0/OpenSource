#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // fork(), sleep()
#include <sys/types.h> // pid_t
#include <sys/wait.h> // wait()

int main(void) {
    pid_t pid;

    // 자식 프로세스 생성
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    } 
    else if (pid == 0) { // 자식 프로세스
        exit(0); // 자식 프로세스는 즉시 종료
    }
    
    // 부모 프로세스
    sleep(4); // 4초 대기
    printf("현재 좀비 프로세스 상태:\n");
    system("ps -o pid,ppid,state,tty,command"); // 프로세스 상태 출력

    // 부모 프로세스가 자식 프로세스를 기다리는 부분 (주석 처리)
    // wait(NULL); // 좀비 프로세스가 생성되지 않도록 자식 프로세스를 기다림

    exit(0);
}
