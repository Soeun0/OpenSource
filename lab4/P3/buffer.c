#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE]; // 제한 버퍼
int count = 0;           // 현재 버퍼의 아이템 개수
int in = 0;              // 생산자가 아이템을 넣는 위치
int out = 0;             // 소비자가 아이템을 꺼내는 위치
pthread_mutex_t mutex;   // 버퍼 보호용 뮤텍스
sem_t empty;             // 버퍼의 빈 공간을 나타내는 세마포어
sem_t full;              // 버퍼의 채워진 공간을 나타내는 세마포어
void *producer(void *arg) {
    int producer_id = *(int *)arg;
    while (1) {
        int item = rand() % 100 + 1; // 랜덤한 정수를 생산
        sem_wait(&empty);           // 빈 공간 확보 대기
        pthread_mutex_lock(&mutex); // 버퍼 접근 잠금
        // 버퍼에 아이템 추가
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        printf("[Producer %d] Produced: %d | Buffer Size: %d\n", producer_id, item, count);
        pthread_mutex_unlock(&mutex); // 버퍼 접근 잠금 해제
        sem_post(&full);              // 채워진 공간 증가
        sleep(rand() % 2 + 1); // 생산 간격
    }
    return NULL;
}
void *consumer(void *arg) {
    int consumer_id = *(int *)arg;
    while (1) {
        sem_wait(&full);             // 채워진 공간 확보 대기
        pthread_mutex_lock(&mutex); // 버퍼 접근 잠금
        // 버퍼에서 아이템 가져오기
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        printf("[Consumer %d] Consumed: %d | Buffer Size: %d\n", consumer_id, item, count);
        pthread_mutex_unlock(&mutex); // 버퍼 접근 잠금 해제
        sem_post(&empty);             // 빈 공간 증가
        sleep(rand() % 3 + 1); // 소비 간격
    }
    return NULL;
}
int main() {
    pthread_t producers[3];
    pthread_t consumers[2];
    int producer_ids[3] = {0, 1, 2};
    int consumer_ids[2] = {0, 1};
    // 뮤텍스와 세마포어 초기화
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    // 생산자 쓰레드 생성
    for (int i = 0; i < 3; i++) {
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }
    // 소비자 쓰레드 생성
    for (int i = 0; i < 2; i++) {
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }
    // 쓰레드 종료를 막기 위한 조치 (무한 대기)
    for (int i = 0; i < 3; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(consumers[i], NULL);
    }
    // 뮤텍스와 세마포어 삭제
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}
