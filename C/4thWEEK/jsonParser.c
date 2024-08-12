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

struct Setting{
    /* data */
    int repeat;
    int thread_num;
    struct Thread thread[MAX_THREADS];
};

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
    for(int i=0; i<args.jsonInput.repeat; i++){
        printf("%s running time: s\n", args.jsonInput.thread[args.id].nam);
    }
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
        setting.thread[i].name = thread_name;
        printf("setting.thread[%d].name: %s\n",i,setting.thread[i].name);
    }

    return 0;
}
