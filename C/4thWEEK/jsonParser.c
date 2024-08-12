#include <stdio.h>
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

    JSON_Value *rootValue = json_parse_file("jparser.json");
    JSON_Object *rootObject = json_value_get_object(rootValue);
    setting.repeat = (int)json_object_get_number(rootObject, "repeat");
    setting.thread_num = (int)json_object_get_number(rootObject, "thread_num");
    JSON_Array *threadArray = json_object_get_array(rootObject, "thread");

    printf("\n** JSON PARSING RESULT **\n");
    printf("Repeat: %d\n", setting.repeat);
    printf("Thread Num: %d\n", setting.thread_num);
    printf("Thread:\n");
    for(int i=0; i<setting.thread_num; i++){
        JSON_Object *threadObject = json_array_get_object(threadArray, i);
        const char *thread_name = json_object_get_string(threadObject, "name");
        printf("    Name: %s\n", thread_name);
    }

    return 0;
}
