// threads.h
#ifndef THREADS_H
#define THREADS_H

#include "parson.h"

#define MAX_THREADS 10
#define STRINGLENTH 10

struct Thread {
    char name[20];
};

struct Setting {
    int thread_num;
    struct Thread thread[MAX_THREADS];
};

struct ThreadArgs {
    int id;
    struct Setting *jsonInput;
};

void signal_handler(int sig);
void* function(void *arg);
void load_settings(struct Setting *setting, const char *filename);
void create_join_thread();
void setup_signal_handler();

#endif // THREADS_H

