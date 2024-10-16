#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"
#include "threads.h"
#include "parson.h"

QueueType Q;

int main()
{
	init(&Q);

	JSON_Value *root_value = json_parse_file("input.json");
	if (root_value == NULL) {
    	fprintf(stderr, "Error parsing JSON file\n");
    	return 1;
	}

	JSON_Object *root_object = json_value_get_object(root_value);
	JSON_Array *strings_array = json_object_get_array(root_object, "strings");

	pthread_t threads[THREAD_NUM];

	// ENQUEUE TREAD
	for (int i = 0; i < THREAD_NUM; i++)
	{

		if (pthread_create(&threads[i], NULL, enqueue_function, (void *)strings_array)) {
    		fprintf(stderr, "Error creating thread\n");
        	exit(1);
		}
    }

	for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

	display(&Q);

	// DEQUEUE THREAD
	for (int i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_create(&threads[i], NULL, dequeue_function, NULL)) {
			fprintf(stderr, "Error creating thread\n");
			exit(1);
		}
	}
	for (int i = 0; i < THREAD_NUM; i++)
	{
		pthread_join(threads[i], NULL);
	}

	json_value_free(root_value);


	return 0;
}
