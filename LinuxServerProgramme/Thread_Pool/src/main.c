#include<stdio.h>
#include<pthread.h>
#include"threadpool.h"

void *work_one(void *arg){
	int i = 10000;
	while(i--){
		if(i == 0){
			printf("work thread %ld process success\n",(long long)arg);
		}
	}
	usleep(100*1000);
	//printf("work thread one end\n");
}

void *work_two(void *arg){
	int i = 10000;
	while(i--){
		if(i == 0){
			printf("work thread two process success\n");
		}
	}
	sleep(1);
	printf("work thread two end\n");
}

int main(){
	
	threadpool_t tp;
	threadpool_task_t task;
	
	long long i = 0;
	
	threadpool_init(&tp, 0);
	
	while(i++ < 100){
		task.m_fp = work_one;
		task.m_arg = (void *)i;
		threadpool_add(&tp, &task);
		usleep(1*1000);
	}
	
	sleep(5);
	printf("end sleep \n");
	threadpool_destory(&tp);
	
	threadpool_init(&tp, 0);
	i = 0;
	while(i++ < 100){
		task.m_fp = work_one;
		task.m_arg = (void *)i;
		threadpool_add(&tp, &task);
		usleep(1*1000);
	}
	sleep(5);
	printf("end sleep \n");
	threadpool_destory(&tp);
	
	return 1;
}