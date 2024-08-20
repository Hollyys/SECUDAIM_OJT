#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>

#define NUM_THREADS 5
#define STRING_LENGTH 20

pthread_t threads[NUM_THREADS];
char *thread_messages[NUM_THREADS];
volatile sig_atomic_t sigint_received = 0;

// 시그널 핸들러
void sigint_handler(int sig) {
    sigint_received = 1;
}

// 스레드 함수
void* thread_function(void *arg) {
    int thread_id = *((int*)arg);
    
    while (!sigint_received) {
        printf("Thread %d: SLEEP\n", thread_id);
        sleep(1);  // 1초 간격으로 SLEEP 출력
    }

    // 시그널이 발생하면 메시지 출력
    printf("Thread %d received signal. Message: %s\n", thread_id, thread_messages[thread_id]);

    return NULL;
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // 스레드 생성 및 고유 메시지 저장
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_messages[i] = malloc(STRING_LENGTH);
        snprintf(thread_messages[i], STRING_LENGTH, "Hello from thread %d!", i);
        
        int *thread_id = malloc(sizeof(int));
        *thread_id = i;
        if (pthread_create(&threads[i], NULL, thread_function, thread_id) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(1);
        }
    }

    // 스레드 종료 대기
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        free(thread_messages[i]);
    }

    printf("All threads have terminated. Exiting program.\n");
    return 0;
}

