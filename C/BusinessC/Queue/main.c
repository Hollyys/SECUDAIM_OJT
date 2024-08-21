#include <stdio.h>
#include "queue.h"

int main()
{
	QueueType Q;
	init(&Q);

	enqueue(&Q, "911 GT3 rs");
	enqueue(&Q, "BMW M2cs");
	enqueue(&Q, "Ferrari 458");
	enqueue(&Q, "Avante N");

	display(&Q);

	return 0;
}
