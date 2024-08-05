#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <time.h>

#define COUNT 1000000

void *func(void *arg);

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
    int number = *((int*)(&arg));
    printf("[Thread(%d)] is running.\n", number);
}