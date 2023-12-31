#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

#define MAXTHREAD 5
#define MAXTASK 1024

struct shared_data {
    int tasks[MAXTASK];
    int head, tail;

    pthread_mutex_t mutex;
};

void        push_task(struct shared_data *datash, int connfd);
int          pop_task(struct shared_data *datash);
void* thread_function(void *datash);

#endif