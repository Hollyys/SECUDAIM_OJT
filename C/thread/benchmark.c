#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <time.h>

#define COUNT 100000000

// Function prototypes
void *local();
void *shared(void *arg);
void *tls(void *arg);
void *atomic(void *arg);
void *spinlock(void *arg);
void *semaphore(void *arg);

// Shared variables
int shared_variable = 0;
atomic_int atomic_variable = 0;
int semaphore_variable = 0;
__thread int tls_variable = 0;

// Spinlock variable structure
typedef struct {
    int value;
    pthread_spinlock_t lock;
} spinlockvariable;

// Semaphore
sem_t sem;

void benchmark(int THREAD_COUNT) {
    pthread_t thread[THREAD_COUNT];
    void *result;
    clock_t start, end;

    // Local variable test
    start = clock();
    printf("\n====== THREAD COUNT: %d ======", THREAD_COUNT);
    printf("\n======== LOCAL VARIABLE ========\n");
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&(thread[i]), NULL, local, NULL)) {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_join(thread[i], &result) != 0) {
            printf("Join Thread Fail\n");
        }
    }
    end = clock();
    printf("Run time: %lf Sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Shared variable test
    start = clock();
    printf("\n======= SHARED VARIABLE ========\n");
    shared_variable = 0; // Reset for accurate measurement
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&(thread[i]), NULL, shared, NULL) != 0) {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_join(thread[i], &result) != 0) {
            printf("Join Thread Fail\n");
        }
    }
    end = clock();
    printf("Run time: %lf Sec\n", (double)(end - start) / CLOCKS_PER_SEC);
    printf("Variable result: %d\n", shared_variable);

    // TLS variable test
    start = clock();
    printf("\n========= TLS VARIABLE =========\n");
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&(thread[i]), NULL, tls, NULL) != 0) {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_join(thread[i], &result) != 0) {
            printf("Join Thread Fail\n");
        }
    }
    end = clock();
    printf("Run time: %lf Sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Atomic variable test
    start = clock();
    printf("\n======= ATOMIC VARIABLE ========\n");
    atomic_store(&atomic_variable, 0); // Reset for accurate measurement
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&(thread[i]), NULL, atomic, NULL) != 0) {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_join(thread[i], &result) != 0) {
            printf("Join Thread Fail\n");
        }
    }
    end = clock();
    printf("Run time: %lf Sec\n", (double)(end - start) / CLOCKS_PER_SEC);
    printf("Variable result: %d\n", atomic_variable);

    // Spinlock variable test
    spinlockvariable sp_var = {0}; // Initialize structure
    pthread_spin_init(&sp_var.lock, 0); // Initialize spinlock

    start = clock();
    printf("\n========== SPIN LOCK ===========\n");
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&(thread[i]), NULL, spinlock, &sp_var) != 0) {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_join(thread[i], &result) != 0) {
            printf("Join Thread Fail\n");
        }
    }
    end = clock();
    printf("Run time: %lf Sec\n", (double)(end - start) / CLOCKS_PER_SEC);
    printf("Variable result: %d\n", sp_var.value);
    pthread_spin_destroy(&sp_var.lock); // Destroy spinlock

    // Semaphore variable test
    start = clock();
    printf("\n========== SEMAPHORE ===========\n");
    semaphore_variable = 0; // Reset for accurate measurement
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&(thread[i]), NULL, semaphore, NULL) != 0) {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_join(thread[i], &result) != 0) {
            printf("Join Thread Fail\n");
        }
    }
    end = clock();
    printf("Run time: %lf Sec\n", (double)(end - start) / CLOCKS_PER_SEC);
    printf("Variable result: %d\n", semaphore_variable);

    printf("\n======== TEST FINISHED =========\n\n");
}

int main() {
    // Initialize semaphore
    sem_init(&sem, 0, 1);

    benchmark(5);
    benchmark(10);
    benchmark(20);

    // Destroy semaphore
    sem_destroy(&sem);

    return 0;
}

void *local() {
    int local_variable = 0;
    for (int i = 0; i < COUNT; i++) {
        local_variable++;
    }
    printf("Local Variable result: %d\n", local_variable);
    pthread_exit(NULL);
}

void *shared(void *arg) {
    for (int i = 0; i < COUNT; i++) {
        shared_variable++;
    }
    pthread_exit(NULL);
}

void *tls(void *arg) {
    for (int i = 0; i < COUNT; i++) {
        tls_variable++;
    }
    printf("TLS Variable result: %d\n", tls_variable);
    pthread_exit(NULL);
}

void *atomic(void *arg) {
    for (int i = 0; i < COUNT; i++) {
        atomic_fetch_add(&atomic_variable, 1);
    }
    pthread_exit(NULL);
}

void *spinlock(void *arg) {
    spinlockvariable *variable = (spinlockvariable *)arg;
    for (int i = 0; i < COUNT; i++) {
        pthread_spin_lock(&variable->lock);
        variable->value++;
        pthread_spin_unlock(&variable->lock);
    }
    pthread_exit(NULL);
}

void *semaphore(void *arg) {
    for (int i = 0; i < COUNT; i++) {
        sem_wait(&sem);
        semaphore_variable++;
        sem_post(&sem);
    }
    pthread_exit(NULL);
}
