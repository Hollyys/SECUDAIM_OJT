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
