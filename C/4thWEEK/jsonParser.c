#include <stdio.h>
#include "parson.h"

typedef struct Thread
{
    /* data */
    char name[20];
};


typedef struct Setting
{
    /* data */
    int repeat;
    int thread_num;
};

int main(){
    struct Setting setting;

    Json_Value *rootValue;
    Json_Object *rootObject;

    rootValue = json_parse_file("example.json");
    rootObject = json_value_get_object(rootValue);

    setting.repeat = (int)json_object_get_number(rootObject, "repeat");
    setting.thread_num = (int)json_object_get_number(rootObject, "thread_num");

    printf("\n** JSON PARSING RESULT **\n");
    printf("Repeat: %d\n", setting.repeat);
    printf("Thread Num: %d\n", setting.thread_num);
    
    return 0;
}
