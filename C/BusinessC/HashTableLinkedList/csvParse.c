#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashTableLinkedList.h"

int run()
{
	FILE *file = fopen("hash.csv", "r");
	if (!file)
	{
		fprintf(stderr, "Could not open file\n");
		return 1;
	}

	char buffer[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE, file))
	{
		char *hash_key = strtok(buffer, ",");
		char *data = strtok(NULL, ",");
		printf("key: %s, data: %s", hash_key, data);
		add(hash_key, data);
	}

	fclose(file);
	return 0;
}

