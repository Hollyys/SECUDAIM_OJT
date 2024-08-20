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

// 출력 동기화를 위한 뮤텍스
pthread_mutex_t print_mutex;

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

// 해시 테이블 초기화 함수
void initialize_table() {
    hashtable = (struct bucket*)malloc(BUCKET_SIZE * sizeof(struct bucket));
    memset(hashtable, 0, BUCKET_SIZE * sizeof(struct bucket));
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
void display_table(int thread_id) {
    pthread_mutex_lock(&print_mutex);  // 출력 전에 뮤텍스 잠금
    printf("Thread %d's hash table:\n", thread_id);
    for (int i = 0; i < BUCKET_SIZE; i++) {
        struct node *current = hashtable[i].head;
        printf("Bucket[%d]: ", i);
        while (current != NULL) {
            printf("(%s: %s) -> ", current->key, current->value);
            current = current->next;
        }
        printf("NULL\n");
    }
    pthread_mutex_unlock(&print_mutex);  // 출력 후 뮤텍스 해제
}

// 해시 테이블 메모리 해제 함수
void free_table() {
    for (int i = 0; i < BUCKET_SIZE; i++) {
        struct node *current = hashtable[i].head;
        while (current != NULL) {
            struct node *tmp = current;
            current = current->next;
            free(tmp->key);
            free(tmp->value);
            free(tmp);
        }
    }
    free(hashtable);
}

// 스레드 함수
void *thread_function(void *arg) {
    int thread_id = *((int*)arg);

    // 각 스레드마다 고유한 해시 테이블 초기화
    initialize_table();

    // 해시 테이블에 데이터 추가
    char key[10];
    char value[10];
    for (int i = 0; i < 10; i++) {
        snprintf(key, sizeof(key), "key%d-%d", thread_id, i);
        snprintf(value, sizeof(value), "value%d-%d", thread_id, i);
        add_to_table(key, value);
    }

    // 해시 테이블 출력
    display_table(thread_id);

    // 해시 테이블 메모리 해제
    free_table();

    return NULL;
}

int main() {
    pthread_t threads[MAX_THREADS];
    int thread_ids[MAX_THREADS];

    // 뮤텍스 초기화
    //pthread_mutex_init(&print_mutex, NULL);

    // 스레드 생성
    for (int i = 0; i < MAX_THREADS; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(1);
        }
    }

    // 스레드 종료 대기
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 뮤텍스 해제
    //pthread_mutex_destroy(&print_mutex);

    return 0;
}

