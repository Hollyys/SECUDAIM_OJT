#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "parson.h"

#define MAX_THREADS 5

struct Thread {
    char name[20];
};

struct Setting {
    int repeat;
    int thread_num;
    struct Thread thread[MAX_THREADS];
};

struct ThreadArgs {
    int id;
    struct Setting *jsonInput; // 포인터로 수정
};

char* stringGenerator() {
    int stringLength = 10;
    char charSet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *output = malloc(stringLength + 1);

    for (int i = 0; i < stringLength; i++) {
        int key = rand() % 52;
        output[i] = charSet[key];
    }
    output[stringLength] = '\0';

    return output;
}

void* function(void *arg) {
    struct ThreadArgs *args = (struct ThreadArgs *)arg;

    printf("repeat: %d\n", args->jsonInput->repeat);
    printf("thread_num: %d\n", args->jsonInput->thread_num);
    printf("name: %s\n", args->jsonInput->thread[args->id].name);

    for (int i = 0; i < args->jsonInput->repeat; i++){
        /* code */
        sleep(1);
        printf("%s running time: %ds\n", args->jsonInput->thread[args->id].name, i+1);
        printf("generated string: %s\n", stringGenerator());
    }

    return NULL;
}

int main() {
    struct Setting setting;
    srand((unsigned int)time(NULL));

    JSON_Value *rootValue = json_parse_file("jparser.json");
    if (rootValue == NULL) {
        fprintf(stderr, "Error parsing JSON file\n");
        return 1;
    }
    JSON_Object *rootObject = json_value_get_object(rootValue);
    if (rootObject == NULL) {
        fprintf(stderr, "Error accessing JSON object\n");
        return 1;
    }
    setting.repeat = (int)json_object_get_number(rootObject, "repeat");
    setting.thread_num = (int)json_object_get_number(rootObject, "thread_num");
    JSON_Array *threadArray = json_object_get_array(rootObject, "thread");

    for (int i = 0; i < setting.thread_num; i++) {
        JSON_Object *threadObject = json_array_get_object(threadArray, i);
        const char *thread_name = json_object_get_string(threadObject, "name");
        strcpy(setting.thread[i].name, thread_name);
    }

    // Threading
    pthread_t threads[setting.thread_num];
    struct ThreadArgs args[setting.thread_num];

    for (int i = 0; i < setting.thread_num; i++) {
        args[i].id = i;
        args[i].jsonInput = &setting; // 포인터로 설정

        if (pthread_create(&threads[i], NULL, function, (void*)&args[i])) {
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }

    for (int i = 0; i < setting.thread_num; i++) {
        pthread_join(threads[i], NULL);
    }

    json_value_free(rootValue);
    return 0;
}
