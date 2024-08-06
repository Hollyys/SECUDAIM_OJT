
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <time.h>

#define COUNT 1000000000

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
int spinlock_variable = 0;
pthread_spinlock_t lock;
void *spinlock(void *arg);

// semaphore (mutex)
int semaphore_variable = 0;
sem_t sem;
void *semaphore(void *arg);

void benchmark(int THREAD_COUNT)
{
    pthread_t thread[THREAD_COUNT];
    void *result;
    time_t start, end;

    // local variable
    time(&start);
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
    printf("Run time: %f\n", (float)(end-start));
    time(&end);

    // shared variable
    time(&start);
    printf("\n======= SHARED VARIABLE =======\n");
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
    printf("Run time: %f\n", (float)(end-start));
    time(&end);

    // TLS variable
    time(&start);
    printf("\n========= TLS VARIABLE =========\n");
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, tls, NULL) != 0)
        {
            printf("THREAD CREATION FAILED\n");
            exit(1);
        }
    }
    printf("Run time: %f\n", (float)(end-start));
    time(&end);

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_join(thread[i], &result) != 0)
        {
            printf("Join Thread Fail\n");
        }
    }
    printf("Run time: %f\n", (float)(end-start));
    time(&end);

    // atomic variable
    time(&start);
    printf("\n======= ATOMIC VARIABLE =======\n");
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
    printf("Run time: %f\n", (float)(end-start));
    time(&end);

    // spin lock
    time(&start);
    printf("\n========= SPIN LOCK =========\n");
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
    printf("Run time: %f\n", (float)(end-start));
    time(&end);

    // semaphore (mutex)
    time(&start);
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
    printf("Run time: %f\n", (float)(end-start));
    time(&end);
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
    for (int i; i < COUNT; i++)
    {
        local_variable++;
    }
    pthread_exit(NULL);
}

void *shared(void *arg)
{
    for (int i; i < COUNT; i++)
    {
        shared_variable++;
    }
    pthread_exit(NULL);
}

void *tls(void *arg)
{
    for (int i; i < COUNT; i++)
    {
        tls_variable++;
    }
    pthread_exit(NULL);
}

void *atomic(void *arg)
{
    for (int i; i < COUNT; i++)
    {
        atomic_variable++;
    }
    pthread_exit(NULL);
}

void *spinlock(void *arg)
{
    pthread_spin_lock(&lock);
    for (int i; i < COUNT; i++)
    {
        spinlock_variable++;
    }
    pthread_spin_unlock(&lock);
    pthread_exit(NULL);
}

void *semaphore(void *arg)
{
    for (int i; i < COUNT; i++)
    {
        sem_wait(&sem);
        semaphore_variable++;
        sem_post(&semaphore);
    }
    pthread_exit(NULL);
}