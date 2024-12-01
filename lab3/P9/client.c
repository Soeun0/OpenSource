#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_SIZE 1024

struct msg_buffer {
    long msg_type;
    char text[MSG_SIZE];
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("사용법: %s [송신 유형] [수신 유형]\n", argv[0]);
        return 1;
    }

    long send_type = atol(argv[1]);
    long receive_type = atol(argv[2]);

    key_t key = ftok("chat_key", 65);
    int msg_id = msgget(key, 0666 | IPC_CREAT);
    struct msg_buffer msg;
    char message[MSG_SIZE];

    printf("채팅을 시작합니다. 종료하려면 'exit'을 입력하세요.\n");

    while (1) {
        // 메시지 전송
        printf("나> ");
        fflush(stdout);
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0'; // 개행 문자 제거

        if (strcmp(message, "exit") == 0) {
            printf("채팅을 종료합니다.\n");
            break;
        }

        msg.msg_type = send_type;
        strncpy(msg.text, message, sizeof(msg.text));
        msgsnd(msg_id, &msg, sizeof(msg.text), 0);

        // 메시지 수신
        msgrcv(msg_id, &msg, sizeof(msg.text), receive_type, 0);
        printf("상대방> %s\n", msg.text);
    }

    if (send_type == 1) {
        msgctl(msg_id, IPC_RMID, NULL); // 메시지 큐 삭제 (P1이 종료 시)
    }

    return 0;
}
