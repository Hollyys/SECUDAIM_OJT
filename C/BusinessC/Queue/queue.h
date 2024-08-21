// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#define SIZE 100

typedef char* element;

typedef struct
{
	element data[SIZE];
	int rear, front;
}QueueType;

void init(QueueType *Q);
int is_empty(QueueType *Q);
int is_full(QueueType *Q);
void enqueue(QueueType *Q, element e);
element dequeue(QueueType *Q);
void display(QueueType *Q);

#endif // QUEUE_H
