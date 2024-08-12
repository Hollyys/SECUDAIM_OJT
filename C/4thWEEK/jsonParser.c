#include <stdio.h>
#include <stdbool.h>
#include "parson.h"

struct Thread{
    /* data */
    char name[20];
};


struct Setting{
    /* data */
    int repeat;
    int thread_num;
};

int main(){
    struct Setting setting;
    bool log = true;

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
        for(int i=0; i<setting.thread_num; i++){
            JSON_Object *threadObject = json_array_get_object(threadArray, i);
            const char *thread_name = json_object_get_string(threadObject, "name");
            printf("    Name: %s\n", thread_name);
        }
    }

    return 0;
}
