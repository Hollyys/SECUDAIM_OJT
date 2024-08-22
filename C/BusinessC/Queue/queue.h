// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "threads.h"

#define SIZE 100

typedef char* element;

typedef struct
{
	element data[SIZE];
	int rear, front;
	pthread_mutex_t lock;
}QueueType;

void init(QueueType *Q);
int is_empty(QueueType *Q);
int is_full(QueueType *Q);
void enqueue(QueueType *Q, element e);
element dequeue(QueueType *Q);
void display(QueueType *Q);

#endif // QUEUE_H
