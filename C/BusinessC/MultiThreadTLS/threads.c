// threads.c
#include "threads.h"
#include "HashTableLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

void signal_handler(int sig)
{
    printf("\nSIGINT received. Saving JSON and exiting in 3 seconds...\n");

    sleep(1);
    printf("3..\n");

    sleep(1);
    printf("2..\n");

    sleep(1);
    printf("1..\n");

	display();
	sleep(1);

    exit(0);
}

void *function(void *arg)
{
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
	hashtable = (struct bucket *)malloc(BUCKET_SIZE * sizeof(struct bucket));
	memset(hashtable, 0, BUCKET_SIZE * sizeof(struct bucket));

	printf("INT..\n");

    signal(SIGINT, signal_handler);


    FILE *file = fopen("hash.csv", "r");
    if (!file)
    {
        fprintf(stderr, "Could not open file\n");
        exit(0);
    }

	printf("%s RUNNING...\n", args->jsonInput->thread[args->id].name);
	
	char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file))
    {
        char *hash_key = strtok(buffer, ",");
        char *data = strtok(NULL, ",");

        add(hash_key, data);
    }
	sleep(1);
	printf("DONE!\n");

	while(1)
	{
		printf("SLEEP...\n");
		sleep(1);
	}

	fclose(file);
    free(hashtable);

    return NULL;
}

void load_settings(struct Setting *setting, const char *filename)
{
    JSON_Value *rootValue = json_parse_file(filename);
    if (rootValue == NULL)
    {
        fprintf(stderr, "Error parsing JSON file\n");
        exit(1);
    }
    JSON_Object *rootObject = json_value_get_object(rootValue);
    if (rootObject == NULL)
    {
        fprintf(stderr, "Error accessing JSON object\n");
        exit(1);
    }

    setting->thread_num = (int)json_object_get_number(rootObject, "thread_num");
    JSON_Array *threadArray = json_object_get_array(rootObject, "thread");

    for (int i = 0; i < setting->thread_num; i++)
    {
        JSON_Object *threadObject = json_array_get_object(threadArray, i);
        const char *thread_name = json_object_get_string(threadObject, "name");
        strcpy(setting->thread[i].name, thread_name);
    }

    json_value_free(rootValue);
}
