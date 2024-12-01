#include <stdio.h>
#include <sys/ipc.h>
int main() {
    key_t key = ftok("chat_key", 65);
    printf("공통 키 생성 완료: %d\n", key);
    return 0;
}
