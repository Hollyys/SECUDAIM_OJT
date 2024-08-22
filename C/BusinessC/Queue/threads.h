// threads.h
#ifndef THREADS_H
#define THREADS_H

#include "queue.h"
#include "parson.h"

// typedef struct {
// 	char *data;
//    QueueType *Q;
// }ThreadArgs;

void* enqueue_function(void *arg);

#endif // THREADS_H
