// threads.h
#ifndef THREADS_H
#define THREADS_H

#include "queue.h"

#define THREAD_NUM 1

struct ThreadArgs {
    char *data;
    QueueType *Q;
};

void* enqueue_function(void *arg);

#endif // THREADS_H
