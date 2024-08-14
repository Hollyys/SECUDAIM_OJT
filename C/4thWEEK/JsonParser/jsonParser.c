#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "parson.h"

#define MAX_THREADS 10
#define STRINGLENTH 10

struct Thread
{
    char name[20];
};

struct Setting
{
    int repeat;
    int thread_num;
    struct Thread thread[MAX_THREADS];
};

struct ThreadArgs
{
    int id;
    struct Setting *jsonInput; // 포인터로 수정
};

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

    JSON_Value *rootValue;
    JSON_Object *rootObject;

    rootValue = json_value_init_object();
    rootObject = json_value_get_object(rootValue);

    json_object_set_number(rootObject, "repeat_cnt", args->jsonInput->repeat);

    JSON_Value *repeatArrayValue = json_value_init_array();
    JSON_Array *repeatArray = json_value_get_array(repeatArrayValue);

    for (int i = 0; i < args->jsonInput->repeat; i++)
    {
        /* code */
        char *generatedString = malloc(STRINGLENTH + 1);
        strcpy(generatedString, stringGenerator());
        json_array_append_string(repeatArray, generatedString);
        free(generatedString);

        sleep(1);
        printf("%s\trunning time: %ds\n", args->jsonInput->thread[args->id].name, i + 1);
    }

    json_object_set_value(rootObject, "repeat", repeatArrayValue);
    json_serialize_to_file_pretty(rootValue, "output.json");

    json_value_free(rootValue);

    return NULL;
}

int main()
{
    struct Setting setting;
    srand((unsigned int)time(NULL));

    JSON_Value *rootValue = json_parse_file("jparser.json");
    if (rootValue == NULL)
    {
        fprintf(stderr, "Error parsing JSON file\n");
        return 1;
    }
    JSON_Object *rootObject = json_value_get_object(rootValue);
    if (rootObject == NULL)
    {
        fprintf(stderr, "Error accessing JSON object\n");
        return 1;
    }
    setting.repeat = (int)json_object_get_number(rootObject, "repeat");
    setting.thread_num = (int)json_object_get_number(rootObject, "thread_num");
    JSON_Array *threadArray = json_object_get_array(rootObject, "thread");

    for (int i = 0; i < setting.thread_num; i++)
    {
        JSON_Object *threadObject = json_array_get_object(threadArray, i);
        const char *thread_name = json_object_get_string(threadObject, "name");
        strcpy(setting.thread[i].name, thread_name);
    }

    // Threading
    pthread_t threads[setting.thread_num];
    struct ThreadArgs args[setting.thread_num];

    for (int i = 0; i < setting.thread_num; i++)
    {
        args[i].id = i;
        args[i].jsonInput = &setting;

        if (pthread_create(&threads[i], NULL, function, (void *)&args[i]))
        {
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }

    for (int i = 0; i < setting.thread_num; i++)
    {
        pthread_join(threads[i], NULL);
    }

    json_value_free(rootValue);
    return 0;
}
