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

    JSON_Value *rootValue;
    JSON_Object *rootObject;
    JSON_Array *thread_array = json_object_get_array(rootObject, "thread");

    rootValue = json_parse_file("jparser.json");
    rootObject = json_value_get_object(rootValue);

    setting.repeat = (int)json_object_get_number(rootObject, "repeat");
    setting.thread_num = (int)json_object_get_number(rootObject, "thread_num");

    printf("\n** JSON PARSING RESULT **\n");
    printf("Repeat: %d\n", setting.repeat);
    printf("Thread Num: %d\n", setting.thread_num);
    printf("Thread:\n");
    for(int i=0; i<setting.thread_num; i++){
        JSON_Object *thread_object = json_array_get_object(thread_array, i);
        char *thread_name = json_array_append_string(thread_object, "name");
        printf("    Name: %s\n", thread_name);
    }

    return 0;
}
