#include "queue.h"

void init(QueueType *Q)
{
  	Q->rear = Q->front = -1;
	pthread_mutex_init(&(Q->lock), NULL);
}

int is_empty(QueueType *Q)
{
	return Q->front == Q->rear;
}

int is_full(QueueType *Q)
{
	return Q->rear == SIZE - 1;
}

void enqueue(QueueType *Q, element e)
{
	pthread_mutex_lock(&(Q->lock));
	if(is_full(Q))
	{
		printf("Overflow\n");
	}
	else
	{
		Q->rear++;
		Q->data[Q->rear] = strdup(e);
		printf("%s Enqueued.: %s\n", e, Q->data[Q->rear]);
		printf("\n");
	}
	pthread_mutex_unlock(&(Q->lock));
}

element dequeue(QueueType *Q)
{
	pthread_mutex_lock(&(Q->lock));
	if(is_empty(Q))
	{
		printf("Empty\n");
		return 0;
	}
	else
	{
		Q->front++;
		return Q->data[Q->front];
	}
	pthread_mutex_unlock(&(Q->lock));
}

void display(QueueType *Q)
{
	printf("Front Pos: %d\nRear Pos: %d\n", Q->front, Q->rear);
	for(int i = Q->front+1; i <= Q->rear; i++)
	{
		printf("[%s] ", Q->data[i]);
	}
	printf("\n");
}
