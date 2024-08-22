#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "parson.h"

#define THREAD_NUM 1
#define QUEUE_SIZE 10

typedef struct {
    char* data[QUEUE_SIZE];
    int front, rear;
    pthread_mutex_t lock;
} Queue;

void init_queue(Queue *q) {
    q->front = q->rear = 0;
    pthread_mutex_init(&(q->lock), NULL);
}

int is_empty(Queue *q) {
    return q->front == q->rear;
}

int is_full(Queue *q) {
    return (q->rear + 1) % QUEUE_SIZE == q->front;
}

void enqueue(Queue *q, char *str) {
    pthread_mutex_lock(&(q->lock));
    if (is_full(q)) {
        printf("Queue is full\n");
    } else {
        q->data[q->rear] = strdup(str);
        q->rear = (q->rear + 1) % QUEUE_SIZE;
    }
    pthread_mutex_unlock(&(q->lock));
}

char* dequeue(Queue *q) {
    pthread_mutex_lock(&(q->lock));
    char* str = NULL;
    if (is_empty(q)) {
        printf("Queue is empty\n");
    } else {
        str = q->data[q->front];
        q->front = (q->front + 1) % QUEUE_SIZE;
    }
    pthread_mutex_unlock(&(q->lock));
    return str;
}

void display(Queue *q){
	printf("Front: %d, Rear: %d\n", q->front, q->rear);
	for(int i=q->front; i<=q->rear; i++){
		printf("[%s] ", q->data[i]);
	}
	printf("\n");
}

Queue q;

void* thread_function(void* arg) {
    JSON_Array *strings = (JSON_Array*)arg;

    for (size_t i = 0; i < json_array_get_count(strings); i++) {
        const char *str = json_array_get_string(strings, i);
        enqueue(&q, (char *)str);
        printf("Enqueued: %s\n", str);
    }

    return NULL;
}

int main() {
    // 큐 초기화
    init_queue(&q);

    // JSON 파일 파싱
    JSON_Value *root_value = json_parse_file("input.json");
    if (root_value == NULL) {
        fprintf(stderr, "Error parsing JSON file\n");
        return 1;
    }

    JSON_Object *root_object = json_value_get_object(root_value);
    JSON_Array *strings = json_object_get_array(root_object, "strings");

    pthread_t threads[THREAD_NUM];

    // 스레드 생성하여 큐에 문자열 저장
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void*)strings);
    }

    // 스레드 종료 대기
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

    // 큐의 내용 출력
    display(&q);

    // JSON 메모리 해제
    json_value_free(root_value);

    return 0;
}

