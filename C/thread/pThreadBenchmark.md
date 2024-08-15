## 240805 [Pthread Benchmark]

1. Heap & Stack
    - Thread들은 Heap영역과 Data 영역을 같이 공유하지만 Stack은 따로따로 사용한다.
    - Stack에는 Local 변수가 저장됨
2. TLS?
    - Thread Local Storage : Thread들 마다 각기 가지고 있는 독립적인 저장공간
    
    ```c
    __thread int tls_variable;
    ```
    
3. Atomic Variable
    - 원자성(Atomic)을 보장하는 변수
    
    ```c
    #include <atdatomic.h>
    
    atomic_int a;
    atomic_bool b;
    ```
    
4. False Sharing
    - Thread간의 공유되지 않은 데이터이지만 캐시구조의 특성으로 마치 공유되는 것으로 인식되어 불필요한 성능 저하 현상이 일어나는 것
    - 각 코어가 동일한 캐시 라인 내의 서로 다른 위치의 데이터를 지역 캐시를 통해 참조할 때 발생 → 사실 서로 다른 메모리 공간에 접근하지만 캐시는 이를 인지할 수 없기 때문에 동기화가 이루어져 성능이 하락한다.
5. Invalidation Storm
    - 
6. Pthread
    - Code
        - pthread_create.c
            
            ```c
            #include <stdio.h>
            #include <pthread.h>
            #include <unistd.h>
            #include <stdlib.h>
            
            void *pthread_ex(void *);
            int main(){
            	int sts;
            	pthread_t thread_id;
            	void *t_return;
            	if((sts=pthread_create((&thread_id),NULL,pthread_ex,NULL))!=0){
            		perror("error\n\n");
            		exit(1);
            	}
            	printf("thread id %lx \n",thread_id);
            	sleep(3);
            	return 0;
            	// main thread가 종료되면 다른 thread도 종료됨
            }
            void *pthread_ex(void *arg)
            {
            	int i=0;
            	while(i<10){
            		sleep(1);
            		printf("thread.. %d\n",i);
            		i++;
            	}
            }
            ```
            
        - pthread_join.c
            
            ```c
            #include <stdio.h>
            #include <unistd.h>
            #include <pthread.h>
            #include <stdlib.h>
            
            void *thread_ex(void *arg);
            int main(){
            	int sts;
            	pthread_t thread_id;
            	void *t_return;
            	void** re;
            	sts=pthread_create(&thread_id,NULL,thread_ex,NULL);
            	printf("thread_id : %lx\n",thread_id);
            	sleep(3);
            	pthread_join(thread_id,(void**)re);
            	// 다른 thread가 종료되기 전에 main thread를 대기시킴
            	return 0;
            }
            void *thread_ex(void *arg){
            	int i=0;
            	while(i<10){
            		printf("threading %d\n",i);
            		i++;
            		if(i==10)
            			exit(1);
            	}
            }
            ```
            
        - pthread_multi.c
            
            ```c
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
            
                rand_num = 1 + (int)(5.0*rand()/(RAND_MAX+1.0));
                sleep(rand_num);
                
                printf("[thread(%d)] job done\n", my_number);
            
                pthread_exit(NULL);
            }
            ```
            
7. Benchmarking
    - Code - ver 1
        
        ```c
        
        #include <stdio.h>
        #include <stdlib.h>
        #include <unistd.h>
        #include <pthread.h>
        #include <semaphore.h>
        #include <stdatomic.h>
        #include <time.h>
        
        #define COUNT 10000000000000
        
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
                sem_post(&sem);
            }
            pthread_exit(NULL);
        }
        ```
        
    - Code - ver 2
        
        ```c
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
        
        ```
        
    - 최종 코드에서 수정된 부분
        - Spin Lock
            1. Spin Lock변수를 구조체 형식으로 선언
                
                ```c
                	typedef struct {
                    int value;
                    pthread_spinlock_t lock;
                } spinlockvariable;
                ```
                
            2. Lock / Unlock 함수를 반복문 안 쪽으로 이동
                
                ```c
                void *spinlock(void *arg) {
                    spinlockvariable *variable = (spinlockvariable *)arg;
                    for (int i = 0; i < COUNT; i++) {
                        pthread_spin_lock(&variable->lock);
                        variable->value++;
                        pthread_spin_unlock(&variable->lock);
                    }
                    pthread_exit(NULL);
                }
                ```
                
            3. Spin Lock변수 구조체, Lock 초기화
                
                ```c
                spinlockvariable sp_var = {0}; // Initialize structure
                pthread_spin_init(&sp_var.lock, 0); // Initialize spinlock
                ```
                
            
            위와 같은 수정을 통해 Spin Lock이 진행되지 않는 문제를 해결할 수 있었다.
            
    - Output
        
        ```bash
        ====== THREAD COUNT: 5 ======
        ======== LOCAL VARIABLE ========
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Run time: 0.184990 Sec
        
        ======= SHARED VARIABLE ========
        Run time: 4.231574 Sec
        Variable result: 117236104
        
        ========= TLS VARIABLE =========
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        Run time: 0.253511 Sec
        
        ======= ATOMIC VARIABLE ========
        Run time: 30.962243 Sec
        Variable result: 500000000
        
        ========== SPIN LOCK ===========
        Run time: 139.963237 Sec
        Variable result: 500000000
        
        ========== SEMAPHORE ===========
        Run time: 156.215318 Sec
        Variable result: 500000000
        
        ======== TEST FINISHED =========
        
        ====== THREAD COUNT: 10 ======
        ======== LOCAL VARIABLE ========
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Run time: 0.347072 Sec
        
        ======= SHARED VARIABLE ========
        Run time: 8.413164 Sec
        Variable result: 157061614
        
        ========= TLS VARIABLE =========
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        Run time: 0.510184 Sec
        
        ======= ATOMIC VARIABLE ========
        Run time: 63.088782 Sec
        Variable result: 1000000000
        
        ========== SPIN LOCK ===========
        Run time: 460.450485 Sec
        Variable result: 1000000000
        
        ========== SEMAPHORE ===========
        Run time: 411.591699 Sec
        Variable result: 1000000000
        
        ======== TEST FINISHED =========
        
        ====== THREAD COUNT: 20 ======
        ======== LOCAL VARIABLE ========
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Local Variable result: 100000000
        Run time: 0.758851 Sec
        
        ======= SHARED VARIABLE ========
        Run time: 17.632334 Sec
        Variable result: 215777255
        
        ========= TLS VARIABLE =========
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        TLS Variable result: 100000000
        Run time: 1.028905 Sec
        
        ======= ATOMIC VARIABLE ========
        Run time: 126.408820 Sec
        Variable result: 2000000000
        
        ========== SPIN LOCK ===========
        Run time: 2062.567324 Sec
        Variable result: 2000000000
        
        ========== SEMAPHORE ===========
        Run time: 901.056674 Sec
        Variable result: 2000000000
        
        ======== TEST FINISHED =========
        ```
        
        ```bash
        ====== THREAD COUNT: 20 ======
        ======== LOCAL VARIABLE ========
        Run time: 0.770956 Sec
        Variable result: 2000000000
        
        ======= SHARED VARIABLE ========
        Run time: 13.390216 Sec
        Variable result: 270121959
        
        ========= TLS VARIABLE =========
        Run time: 1.024892 Sec
        Variable result: 2000000000
        
        ======= ATOMIC VARIABLE ========
        Run time: 105.222245 Sec
        Variable result: 2000000000
        
        ========== SPIN LOCK ===========
        Run time: 1785.954893 Sec
        Variable result: 2000000000
        
        ========== SEMAPHORE ===========
        Run time: 909.553734 Sec
        Variable result: 2000000000
        
        ======== TEST FINISHED =========
        ```