#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "parson.h"

#define MAX_THREADS 5

struct Thread{
    /* data */
    char name[20];
};

struct Setting{
    /* data */
    int repeat;
    int thread_num;
    struct Thread thread[MAX_THREADS];
};

struct ThreadArgs{
    int id;
    struct Setting jsonInput;
}

char* stringGenerator(){
    int stringLenth = 10;
    char charSet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *output = malloc(stringLenth+1);

    for(int i=0; i<stringLenth; i++){
        int key = rand()%52;
        output[i] = charSet[key];
    }
    output[stringLenth] = '\0';

    return output;
}

void* function(void *arg){
    printf("repeat: %d\n", arg.jsonInput.repeat);
    printf("thread_num: %d\n", arg.jsonInput.thread_num);
    printf("name: %s\n", arg.jsonInput.thread[arg.id].name);
}

int main(){
    struct Setting setting;
    srand((unsigned int)time(NULL));

    JSON_Value *rootValue = json_parse_file("jparser.json");
    if(rootValue == NULL){
        fprintf(stderr, "Error parsing JSON file\n");
        return 1;
    }
    JSON_Object *rootObject = json_value_get_object(rootValue);
    if(rootObject == NULL){
        fprintf(stderr, "Error accessing JSON object\n");
        return 1;
    }
    setting.repeat = (int)json_object_get_number(rootObject, "repeat");
    setting.thread_num = (int)json_object_get_number(rootObject, "thread_num");
    JSON_Array *threadArray = json_object_get_array(rootObject, "thread");

    for(int i=0; i<setting.thread_num; i++){
        JSON_Object *threadObject = json_array_get_object(threadArray, i);
        const char *thread_name = json_object_get_string(threadObject, "name");
        setting.thread[i].id = i;
        strcpy(setting.thread[i].name, thread_name);
    }

    // Threading
    struct ThreadArgs args[setting.thread_num];

    for(int i=0; i<setting.thread_num; i++){
        if(pthread_create(&threads[i], NULL, function, (void*)&args[i])){
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }

    for(int i=0; i<setting.thread_num; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}
