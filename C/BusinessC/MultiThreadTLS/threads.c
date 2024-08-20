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

volatile sig_atomic_t sigint_received = 0;
pthread_mutex_t print_mutex;

void signal_handler(int sig)
{
    printf("\nSIGINT received. Status will be printed in 3 seconds...\n");

	sigint_received = 1;
}

void *function(void *arg)
{
	struct ThreadArgs *args = (struct ThreadArgs *)arg;

	sigset_t *set = (sigset_t *)arg;
	int sig;

	initialize_table(args->jsonInput->thread[args->id].name);

    FILE *file = fopen("hash.csv", "r");
    if (!file)
    {
        fprintf(stderr, "Could not open file\n");
        exit(0);
    }

	printf("%s RUNNING...\n", args->jsonInput->thread[args->id].name);
	sleep(1);

	char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file))
    {
        char *hash_key = strtok(buffer, ",");
        char *data = strtok(NULL, ",");

        add(hash_key, data);
    }
	printf("%s DONE!\n", args->jsonInput->thread[args->id].name);
	sleep(1);

	while(!sigint_received)
	{
		printf("SLEEP...\n");
		sleep(2);
	}
	
	pthread_mutex_lock(&print_mutex);
	printf("\n%s thread HashTable:\n", args->jsonInput->thread[args->id].name);
	display();
	pthread_mutex_unlock(&print_mutex);

	fclose(file);
    free_table();

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

void create_join_thread()
{
	struct Setting setting;

	load_settings(&setting, "jparser.json");
	
	pthread_mutex_init(&print_mutex, NULL);
	pthread_t threads[setting.thread_num];
	struct ThreadArgs args[setting.thread_num];

	for (int i = 0; i < setting.thread_num; i++) {
		args[i].id = i;
    	args[i].jsonInput = &setting;

    	if (pthread_create(&threads[i], NULL, function, (void *)&args[i])) {
    		fprintf(stderr, "Error creating thread\n");
        	exit(1);
    	}
	}

    for (int i = 0; i < setting.thread_num; i++) {
        pthread_join(threads[i], NULL);
    }
	pthread_mutex_destroy(&print_mutex);
}

void setup_signal_handler()
{
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}
