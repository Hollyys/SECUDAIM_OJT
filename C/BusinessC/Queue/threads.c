#include "queue.h"

extern QueueType Q;

void* enqueue_function(void *arg){
		JSON_Array *strings = (JSON_Array*)arg;

		for(int i=0; i<json_array_get_count(strings); i++)
		{
			const char* string = json_array_get_string(strings, i);
			enqueue(&Q, (element)string);
			printf("Enqueued: %s\n", (element)string);
		}
		
		return NULL;
}

void* dequeue_function(void *arg){
	printf("\n** Dequeued List **\n");
	while(!is_empty(&Q)){
		char *output = dequeue(&Q);
		printf(" - %s\n", output);
		free(output);
	}
	printf("\n******************\n");

	return NULL;
}
