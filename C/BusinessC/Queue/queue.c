#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void init(QueueType *Q)
{
  Q->rear = Q->front = -1;
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
	if(is_full(Q))
	{
		printf("Overflow\n");
	}
	else
	{
		Q->rear++;
		Q->data[Q->rear] = strdup(e);
	}
}

element dequeue(QueueType *Q)
{
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
