#include <stdio.h>
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

struct ThreadArgs{
    int id;
    struct Setting jsonInput;
};

struct Setting{
    /* data */
    int repeat;
    int thread_num;
    struct Thread thread[MAX_THREADS];
};

char* stringGenerator(){
    int stringLenth = 10;
    char charSet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    // char *output = calloc(stringLenth+1, sizeof(char));
    char *output = malloc(stringLenth+1);

    for(int i=0; i<stringLenth; i++){
        int key = rand()%52;
        // printf("%c ", charSet[key]);
        output[i] = charSet[key];
    }
    output[stringLenth] = '\0';
    // printf("\n%s\n", output);

    return output;
}

void* function(void *arg){
    ThreadArgs *args = (ThreadArgs *)arg;
    for(int i=0; i<args.jsonInput.repeat; i++){
        printf("%s running time: s\t%s\n", args.jsonInput.thread[args.id].nam, stringGenerator());
    }
}

int main(){
    struct Setting setting;
    bool log = false;
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

    if(log){
        printf("\n** JSON PARSING RESULT **\n");
        printf("Repeat: %d\n", setting.repeat);
        printf("Thread Num: %d\n", setting.thread_num);
        printf("Thread:\n");
    }

    for(int i=0; i<setting.thread_num; i++){
        JSON_Object *threadObject = json_array_get_object(threadArray, i);
        const char *thread_name = json_object_get_string(threadObject, "name");
        setting.thread[i].name = thread_name;
        if(log){
            printf("    Name: %s\n", setting.thread[i].name);
        }
    }

    if(log){
        printf("*************************\n\n");
    }

    pthread_t threads[setting.thread_num];
    ThreadArgs args[setting.thread_num];
    void *result;

    for (int i = 0; i < setting.repeat; i++) {
        args[i].id = i;
        args[i].jsonInput = setting;
        if (pthread_create(&(threads[i]), NULL, function, &args[i])) {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }
    for (int i = 0; i < setting.repeat; i++) {
        if (pthread_join(threads[i], &result) != 0) {
            printf("Join Thread Fail\n");
        }
    }

    return 0;
}
