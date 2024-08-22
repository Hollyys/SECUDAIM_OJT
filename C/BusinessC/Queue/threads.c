#include "queue.h"

extern QueueType Q;

void* enqueue_function(void *arg){
		//struct ThreadArgs *args = (struct ThreadArgs *)arg;
		JSON_Array *strings = (JSON_Array*)arg;

		for(int i=0; i<json_array_get_count(strings); i++)
		{
			const char *string = json_array_get_string(strings, i);
			enqueue(&Q, (element)string);
		}
		
		return NULL;
}
