
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <time.h>

#define COUNT 10000000

// test
void *test(void *arg);

// local variable
void *local();

// shared variable
int shared_variable = 0;
void *shared(void *arg);

// TLS variable
__thread int tls_variable = 0;
void *tls(void *arg);

// atomic variable
atomic_int atomic_variable;
void *atomic(void *arg);

// spin lock
// int spinlock_variable = 0;
// pthread_spinlock_t lock;
typedef struct {
    int value;
    pthread_spinlock_t lock;
} spinlockvariable;
void *spinlock(void *arg);

// semaphore (mutex)
int semaphore_variable = 0;
sem_t sem;
void *semaphore(void *arg);

void benchmark(int THREAD_COUNT)
{
    pthread_t thread[THREAD_COUNT];
    void *result;
    // time_t start, end;

    // local variable
    clock_t start = clock();
    printf("\n======== LOCAL VARIABLE ========\n");
    
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, local, NULL))
        {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_join(thread[i], &result) != 0)
        {
            printf("Join Thread Fail\n");
        }
    }
    clock_t end = clock();
    printf("Run time: %lf Sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    // shared variable
    start = clock();
    printf("\n======= SHARED VARIABLE ========\n");
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, shared, NULL) != 0)
        {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_join(thread[i], &result) != 0)
        {
            printf("Join Thread Fail\n");
        }
    }
    end = clock();
    printf("Run time: %lf Sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    // TLS variable
    start = clock();
    printf("\n========= TLS VARIABLE =========\n");
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, tls, NULL) != 0)
        {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_join(thread[i], &result) != 0)
        {
            printf("Join Thread Fail\n");
        }
    }
    end = clock();
    printf("Run time: %lf Sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    // atomic variable
    start = clock();
    printf("\n======= ATOMIC VARIABLE ========\n");
    void *atomic(void *arg);
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, atomic, NULL) != 0)
        {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_join(thread[i], &result) != 0)
        {
            printf("Join Thread Fail\n");
        }
    }
    end = clock();
    printf("Run time: %lf Sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    // spin lock
    start = clock();
    printf("\n========== SPIN LOCK ===========\n");
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, spinlock, NULL) != 0)
        {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_join(thread[i], &result) != 0)
        {
            printf("Join Thread Fail\n");
        }
    }
    end = clock();
    pthread_spin_destroy(&lock);
    printf("Run time: %lf Sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    // semaphore (mutex)
    start = clock();
    printf("\n========== SEMAPHORE ==========\n");
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, semaphore, NULL) != 0)
        {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_join(thread[i], &result) != 0)
        {
            printf("Join Thread Fail\n");
        }
    }
    end = clock();
    printf("Run time: %lf Sec\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main()
{
    benchmark(5);
    return 0;
}

void *test(void *arg)
{
    printf("[Thread(%d)] is running.\n", *((int *)(&arg)));
    sleep(5);
    printf("[Thread(%d)] is finish.\n", *((int *)(&arg)));

    pthread_exit(NULL);
}

void *local()
{
    int local_variable = 0;
    for (int i = 0; i < COUNT; i++)
    {
        local_variable++;
    }
    printf("Variable result: %d\n", local_variable);
    pthread_exit(NULL);
}

void *shared(void *arg)
{
    for (int i = 0; i < COUNT; i++)
    {
        shared_variable++;
    }
    printf("Variable result: %d\n", shared_variable);
    pthread_exit(NULL);
}

void *tls(void *arg)
{
    for (int i = 0; i < COUNT; i++)
    {
        tls_variable++;
    }
    printf("Variable result: %d\n", tls_variable);
    pthread_exit(NULL);
}

void *atomic(void *arg)
{
    for (int i = 0; i < COUNT; i++)
    {
        // atomic_variable++;
        atomic_fetch_add(&atomic_variable, 1);
    }
    printf("Variable result: %d\n", atomic_variable);
    pthread_exit(NULL);
}

void *spinlock(void *arg)
{
    spinlockvariable *variable = (spinlockvariable *)args;
    
    for (int i = 0; i < COUNT; i++)
    {
        pthread_spin_lock(&(variable->lock));
        variable->value++;
        pthread_spin_unlock(&(variable->lock));
    }
    printf("Variable result: %d\n", spinlock_variable);
    
    pthread_exit(NULL);
}

void *semaphore(void *arg)
{
    for (int i = 0; i < COUNT; i++)
    {
        sem_wait(&sem);
        semaphore_variable++;
        sem_post(&semaphore);
    }
    printf("Variable result: %d\n", semaphore_variable);
    pthread_exit(NULL);
}