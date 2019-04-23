#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int i = 0;

void *thread1(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		++i;
		if(i%10 == 0){
			pthread_cond_signal(&cond);
		}
		pthread_mutex_unlock(&mutex);
		
		usleep(100*1000);
	}
}

void *thread2(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		++i;
		if(i%10 == 0){
			pthread_cond_signal(&cond);
		}
		pthread_mutex_unlock(&mutex);
		
		usleep(100*1000);
	}
}

void *thread3(void *arg){
	int temp = 0;
	while(1){
		pthread_mutex_lock(&mutex);
		if(temp != i){
			while(i%10 != 0){
				pthread_cond_wait(&cond, &mutex);
			}
			printf("thread ID: 3, i = %d\n", i);
			temp = i;
		}	
		pthread_mutex_unlock(&mutex);
		usleep(100*1000);
	}
}

void *thread4(void *arg){
	int temp = 0;
	while(1){
		pthread_mutex_lock(&mutex);
		if(temp != i){
			while(i%10 != 0){
				pthread_cond_wait(&cond, &mutex);
			}
			printf("thread ID: 4, i = %d\n", i);
			temp = i;
		}	
		pthread_mutex_unlock(&mutex);
		usleep(100*1000);
	}
}

int main()
{
	pthread_t pid1;
	pthread_t pid2;
	pthread_t pid3;
	pthread_t pid4;
	
	pthread_create(&pid1,NULL,thread1,(void *)NULL);
	pthread_detach(pid1);
	pthread_create(&pid2,NULL,thread2,(void *)NULL);
	pthread_detach(pid2);
	
	pthread_create(&pid3,NULL,thread3,(void *)NULL);
	pthread_detach(pid3);
	pthread_create(&pid4,NULL,thread4,(void *)NULL);
	pthread_detach(pid4);
	
	pthread_exit(NULL);
	return 0;
}