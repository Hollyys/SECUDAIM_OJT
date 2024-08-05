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
	//pthread_join(sts,(void**)re);
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