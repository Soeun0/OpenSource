#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CLIENTS 5
#define MESSAGE_SIZE 256

pthread_mutex_t mutex;
pthread_cond_t cond;

char message[MESSAGE_SIZE];
int message_ready = 0;

void *client_thread(void *arg) {
    int client_id = *(int *)arg;
    while (1) {
        pthread_mutex_lock(&mutex);

        while (!message_ready) {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("[Client %d] Received message: %s\n", client_id, message);

        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void *server_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        printf("[Server] Enter a message to broadcast: ");
        fgets(message, MESSAGE_SIZE, stdin);
        message[strcspn(message, "\n")] = 0;
        message_ready = 1;

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t server;
    pthread_t clients[MAX_CLIENTS];
    int client_ids[MAX_CLIENTS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&server, NULL, server_thread, NULL);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_ids[i] = i;
        pthread_create(&clients[i], NULL, client_thread, &client_ids[i]);
    }

    pthread_join(server, NULL);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(clients[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}