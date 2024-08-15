// main.c
#include "threads.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    struct Setting setting;
    srand((unsigned int)time(NULL));

    load_settings(&setting, "jparser.json");

    pthread_t threads[setting.thread_num];
    struct ThreadArgs args[setting.thread_num];

    for (int i = 0; i < setting.thread_num; i++) {
        args[i].id = i;
        args[i].jsonInput = &setting;

        if (pthread_create(&threads[i], NULL, function, (void *)&args[i])) {
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }

    for (int i = 0; i < setting.thread_num; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
 
