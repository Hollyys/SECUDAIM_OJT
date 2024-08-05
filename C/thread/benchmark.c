#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <time.h>

#define COUNT 1000000

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
void *spinlock(void *arg);

// semaphore (mutex)
void *semaphore(void *arg);

void benchmark(int THREAD_COUNT)
{
    pthread_t thread[THREAD_COUNT];
    void *result;

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, test, (void *)i) != 0)
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

    // local variable
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, local, NULL)
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

    // shared variable
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, shared, (void *)shared_variable) != 0)
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

    // TLS variable
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, tls, (void *)tls_variable) != 0)
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

    // atomic variable
    void *atomic(void *arg);
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, atomic, (void *)atomic_variable) != 0)
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

    // spin lock
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, spinlock, (void *)i) != 0)
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

    // semaphore (mutex)
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&(thread[i]), NULL, semaphore, (void *)i) != 0)
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
        *((int *)(&arg))++;
    }
    pthread_exit(NULL);
}

void *tls(void *arg)
{
    for (int i; i < COUNT; i++)
    {
        *((int *)(&arg))++;
    }
    pthread_exit(NULL);
}

void *atomic(void *arg)
{
    for (int i; i < COUNT; i++)
    {
        *((int *)(&arg))++;
    }
    pthread_exit(NULL);
}

void *spinlock(void *arg)
{
    for (int i; i < COUNT; i++)
    {
        *((int *)(&arg))++;
    }
    pthread_exit(NULL);
}

void *semaphore(void *arg)
{
    for (int i; i < COUNT; i++)
    {
        *((int *)(&arg))++;
    }
    pthread_exit(NULL);
}