#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashTableLinkedList.h"

int main()
{
	hashtable = (struct bucket *)malloc(BUCKET_SIZE * sizeof(struct bucket));
	memset(hashtable, 0, BUCKET_SIZE * sizeof(struct bucket));

	FILE *file = fopen("hash.csv", "r");
	if (!file)
	{
		fprintf(stderr, "Could not open file\n");
		return 1;
	}

	//display();

	char buffer[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE, file))
	{
		char *hash_key = strtok(buffer, ",");
		char *data = strtok(NULL, ",");
		printf("key: %s, data: %s", hash_key, data);
		add(hash_key, data);
		//display();
	}

	fclose(file);
	free(hashtable);

	display();

	return 0;
}
