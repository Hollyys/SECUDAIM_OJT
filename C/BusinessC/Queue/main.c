#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"
#include "threads.h"

int main()
{
	QueueType Q;
	init(&Q);

	pthread_t threads[THREAD_NUM];
	struct ThreadArgs args[THREAD_NUM];

	for (int i = 0; i < THREAD_NUM; i++)
	{
		args->Q = &Q;
		args->data = "911 GT3";

		if (pthread_create(&threads[i], NULL, enqueue_function, (void *)&args[i])) {
    		fprintf(stderr, "Error creating thread\n");
        	exit(1);
		}
    }

	for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(threads[i], NULL);
    }


	display(&Q);

	return 0;
}
