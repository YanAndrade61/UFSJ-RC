#include "main.h"

void push_task(struct shared_data *datash, int connfd) {
    pthread_mutex_lock(&(datash->mutex));

    if (datash->tail == MAXTHREAD - 1) {
        printf("Fila de tarefas cheia. Descartando conexão.\n");
        close(connfd);
    } else {
        if (datash->head == -1)
            datash->head = 0;
        datash->tail++;
        datash->tasks[datash->tail] = connfd;
        printf("Conexão enfileirada para processamento.\n");
    }
    pthread_mutex_unlock(&(datash->mutex));
}

int pop_task(struct shared_data *datash) {
    pthread_mutex_lock(&(datash->mutex));

    int task;

    if (datash->head == -1) {
        task = -1;
    } else {
        task = datash->tasks[datash->head];
        datash->head++;

        if (datash->head > datash->tail)
            datash->head = datash->tail = -1;
    }

    pthread_mutex_unlock(&(datash->mutex));

    return task;
}


void* thread_function(void* arg) {
    struct shared_data *datash = (struct shared_data *)arg;
    while (1) {
        int task = pop_task(datash);
        if (task != -1){
            web_server(task);
        }
    }
}
