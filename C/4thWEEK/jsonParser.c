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

    rootValue = json_parse_file("jparser.json");
    rootObject = json_value_get_object(rootValue);

    setting.repeat = (int)json_object_get_number(rootObject, "repeat");
    setting.thread_num = (int)json_object_get_number(rootObject, "thread_num");

    printf("\n** JSON PARSING RESULT **\n");
    printf("Repeat: %d\n", setting.repeat);
    printf("Thread Num: %d\n", setting.thread_num);

    return 0;
}
