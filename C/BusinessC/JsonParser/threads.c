// threads.c
#include "threads.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

void signal_handler(int sig)
{
	JSON_Value *rootValue = NULL;
    printf("\nSIGINT received. Saving JSON and exiting in 3 seconds...\n");

 	json_serialize_to_file_pretty(rootValue, "output.json");

    sleep(1);
    printf("3..\n");

    sleep(1);
    printf("2..\n");

    sleep(1);
    printf("1..\n");

	json_value_free(rootValue);

    exit(0);
}

char *stringGenerator()
{
    int stringLength = STRINGLENTH;
    char charSet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *output = malloc(stringLength + 1);

    for (int i = 0; i < stringLength; i++)
    {
        int key = rand() % 52;
        output[i] = charSet[key];
    }
    output[stringLength] = '\0';

    return output;
}

void *function(void *arg)
{
    struct ThreadArgs *args = (struct ThreadArgs *)arg;

    signal(SIGINT, signal_handler);

	JSON_Value *rootValue = json_value_init_object();
	JSON_Object *rootObject = json_value_get_object(rootValue);

	json_object_set_number(rootObject, "repeat_cnt", args->jsonInput->repeat);
	
	JSON_Value *repeatArrayValue = json_value_init_array();
	JSON_Array *repeatArray = json_value_get_array(repeatArrayValue);
    for (int i = 0; i < args->jsonInput->repeat; i++)
    {
        char *generatedString = stringGenerator();
        json_array_append_string(repeatArray, generatedString);
        free(generatedString);

        sleep(1);
        printf("%s\trunning time: %ds\n", args->jsonInput->thread[args->id].name, i + 1);
    }

	json_object_set_value(rootObject, "repeat", repeatArrayValue);

	json_value_free(rootValue);

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
    setting->repeat = (int)json_object_get_number(rootObject, "repeat");
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

void run()
{
	struct Setting setting;
    srand((unsigned int)time(NULL));

    load_settings(&setting, "jparser.json");

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

	while(1){
		printf("Waiting for SIGINT..\n");
		sleep(2);
	}
}
