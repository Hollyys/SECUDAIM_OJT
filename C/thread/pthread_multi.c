#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

/* 최대 쓰레드 개수 */
#define MAX_THREAD_NUM 5

/* thread main function 선언 */
void *thread_function(void *arg);

int main()
{
    int res;
    pthread_t thread_handle[MAX_THREAD_NUM];
    void *thread_result;
    int i;

    /* MAX_THREAD_NUM만큼 pthread를 생성한다.*/
    for(i = 0; i < MAX_THREAD_NUM; i++){
        /**
        pthread를 생성하여 생성된 pthread에 대한 handle을 thread_handle 배열에 저장한다.
        pthread의 main_function은 세번째 인자인 thread_function이 되며, 인자로 몇번째로 생성된 pthread인지를 나타내는
        i값을 넘긴다.
        */
        res = pthread_create(&(thread_handle[i]), NULL, thread_function, (void *)i);
        /* 결과값을 확인하여 Error가 발생하였는지 확인한다. */
        if(res != 0){
            printf("[Main] Thread Creation Failed.\n");
            exit(1);
        } 
    }

    /* 메시지 출력 */
    printf("[Main] Waiting for threads to finish..\n");

    /* MAX_THREAD_NUM만큼 생성된 모든 pthread가 종료되기를 기다린다. */
    for(i = 0; i < MAX_THREAD_NUM; i++){
        /*
        생성된 pthread가 종료되기를 기다린다.
        pthread 내부에서 pthread_exit가 호출되기 전까지 블록된다.
        pthread_exit의 인자를 pthread_result값으로 전달받는다.
        */

        res = pthread_join(thread_handle[i], &thread_result);

        /* 결과값을 확인하여 pthread가 올바로 join되었는지 확인한다. */
        if(res == 0){
            printf("[Main] join thread(%d)\n",i);
        }else{
            printf("[Main] join thread(%d) failed\n", i);
        }
    }

    /* 메시지 출력 */
    printf("[Main] All done\n");
    exit(1); 
} 

void *thread_function(void *arg)
{
    /* 인자로 받은 값을 my_number에 저장한다. */
    int my_number = *((int*)(&arg));
    int rand_num;

    /* 몇번째 pthread가 실행중인지 메시지를 출력한다. */
    printf("[thread(%d)] is running\n", my_number);

    /* 1부터 5 사이의 임의의 값을 생성한다. */
    rand_num = 1 + (int)(5.0*rand()/(RAND_MAX+1.0));

    /* 생성된 임의의 값만큼 sleep한다. */
    sleep(rand_num);

    /* sleep이 끝났음을 메시지로 출력한다 */
    printf("[thread(%d)] job done\n", my_number);

    /* thread 작업이 종료되었음을 알린다. */
    pthread_exit(NULL);
}