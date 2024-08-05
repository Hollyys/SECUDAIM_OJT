#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <time.h>

#define COUNT 1000000

void *func(void *arg);
// normal variable
// TLS variable
// shared variable
// atomic variable
// spin lock
// semaphore (mutex)

void benchmark(int THREAD_COUNT){
    pthread_t thread[THREAD_COUNT];
    void *result;

    for(int i=0; i<THREAD_COUNT; i++){
        if(pthread_create(&(thread[i]), NULL, func, (void *)i) != 0){
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }

    for(int i=0; i<THREAD_COUNT; i++){
        if(pthread_join(thread[i], &result) != 0){
            printf("Join Thread Fail\n");
        }
    }
}

int main(){
    benchmark(5);
    return 0;
}

void *func(void *arg){
    printf("[Thread(%d)] is running.\n", *((int*)(&arg)));
    sleep(5);
    printf("[Thread(%d)] is finish.\n", *((int*)(&arg)));

    pthread_exit(NULL);
}