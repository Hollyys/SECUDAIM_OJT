#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUCKET_SIZE 31
#define MAX_THREADS 5

// 노드 구조체 정의
struct node {
    char *key;
    char *value;
    struct node *next;
};

// 버킷 구조체 정의
struct bucket {
    struct node *head;
};

// TLS를 사용하여 스레드별 해시 테이블 생성
__thread struct bucket *hashtable;

// 해시 함수
unsigned int hash_function(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str;
        str++;
    }
    return hash % BUCKET_SIZE;
}

// 노드 생성 함수
struct node* create_node(const char *key, const char *value) {
    struct node *new_node = (struct node*)malloc(sizeof(struct node));
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->next = NULL;
    return new_node;
}

// 해시 테이블에 데이터 추가 함수
void add_to_table(const char *key, const char *value) {
    unsigned int hash_index = hash_function(key);
    struct node *new_node = create_node(key, value);
    if (hashtable[hash_index].head == NULL) {
        hashtable[hash_index].head = new_node;
    } else {
        new_node->next = hashtable[hash_index].head;
        hashtable[hash_index].head = new_node;
    }
}

// 해시 테이블 출력 함수
void display_table() {
    for (int i = 0; i < BUCKET_SIZE; i++) {
        struct node *current = hashtable[i].head;
        printf("Bucket[%d]: ", i);
        while (current != NULL) {
            printf("(%s: %s) -> ", current->key, current->value);
            current = current->next;
        }
        printf("NULL\n");
    }
}

// 스레드 함수
void *thread_function(void *arg) {
    int thread_id = *((int*)arg);

    // TLS를 사용하여 각 스레드별 해시 테이블 초기화
    hashtable = (struct bucket*)malloc(BUCKET_SIZE * sizeof(struct bucket));
    memset(hashtable, 0, BUCKET_SIZE * sizeof(struct bucket));

    // 해시 테이블에 데이터 추가 (스레드마다 고유한 데이터)
    char key[10];
    char value[10];
    for (int i = 0; i < 10; i++) {
        snprintf(key, sizeof(key), "key%d-%d", thread_id, i);
        snprintf(value, sizeof(value), "value%d-%d", thread_id, i);
        add_to_table(key, value);
    }

    // 해시 테이블 출력
    printf("\nThread %d's hash table:\n", thread_id);
    display_table();

    free(hashtable);
    return NULL;
}

int main() {
    pthread_t threads[MAX_THREADS];
    int thread_ids[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

