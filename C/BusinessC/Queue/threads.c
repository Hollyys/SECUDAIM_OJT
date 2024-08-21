#include "threads.h"
#include "queue.h"
#include <pthread.h>

void* enqueue_function(void *arg){
		struct ThreadArgs *args = (struct ThreadArgs *)arg;
		enqueue(args->Q, args->data);
		
		return NULL;
}
