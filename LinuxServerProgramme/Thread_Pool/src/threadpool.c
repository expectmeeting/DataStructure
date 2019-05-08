#include"threadpool.h"
#define default_num 10

void threadpool_queue_init(threadpool_queue_t *queue){
	queue->m_head = 0;
	queue->m_tail = -1;
	queue->m_size = 0;
	queue->m_capacity = 8;
	queue->m_buf = (void *)malloc(sizeof(threadpool_task_t)*queue->m_capacity);
};

int threadpool_queue_push(threadpool_queue_t *queue, threadpool_task_t *task){
	if(queue == NULL || task == NULL){
		return -1;
	}
	
	threadpool_task_t *buf;
	
	//扩容会影响任务的先后顺序
	if(queue->m_size == queue->m_capacity){
		queue->m_capacity = queue->m_capacity << 1;
		
		buf = (threadpool_task_t *) malloc(sizeof(threadpool_task_t)*queue->m_capacity);
		
		if(queue->m_head < queue->m_tail){
			memcpy(buf, queue->m_buf, sizeof(threadpool_task_t)*queue->m_size);
		}
		else{
			memcpy(buf, (threadpool_task_t *)queue->m_buf + queue->m_head, sizeof(threadpool_task_t)*(queue->m_size - queue->m_tail - 1));
			memcpy(buf + (queue->m_size - queue->m_tail - 1), queue->m_buf, sizeof(threadpool_task_t)*(queue->m_tail + 1));
		}
		free(queue->m_buf);
		queue->m_buf = (void *) buf;	
		queue->m_head = 0;
		queue->m_tail = queue->m_size - 1;
	}
	
	buf = (threadpool_task_t *) queue->m_buf;
	
	if(queue->m_tail < queue->m_capacity - 1){
		queue->m_tail++;
	}
	else if(queue->m_tail == queue->m_capacity - 1){
		queue->m_tail = 0;
	}
	queue->m_size++;
	
	buf[queue->m_tail] = *task;
	
	return 0;
}

int threadpool_queue_pop(threadpool_queue_t *queue, threadpool_task_t *task){
	if(queue == NULL || queue->m_size == 0){
		return -1;
	}
	
	threadpool_task_t *buf = (threadpool_task_t *)queue->m_buf;
	*task = buf[queue->m_head];
	buf[queue->m_head].m_fp = NULL;
	buf[queue->m_head].m_arg = NULL;
	
	if(queue->m_head < queue->m_capacity - 1){
		queue->m_head++;
	}
	else if(queue->m_head == queue->m_capacity - 1){
		queue->m_head = 0;
	}
	queue->m_size--;
	
	return 0;
}

int threadpool_queue_destory(threadpool_queue_t *queue){
	if(queue == NULL || queue->m_buf == NULL){
		return -1;
	}
	free(queue->m_buf);
	queue->m_buf = NULL;
	
	queue->m_size = queue->m_capacity = 0;
	queue->m_head = 0;
	queue->m_tail = -1;
	
	return 0;
}

int threadpool_init(threadpool_t *tp, int num){
	int i;
	
	//init variable
	tp->m_num = default_num;
	tp->m_run = THREADPOOL_RUN;
	tp->m_head = NULL;
	pthread_mutex_init(&tp->m_mutex, NULL);
	pthread_cond_init(&tp->m_cond, NULL);
	threadpool_queue_init(&tp->m_task);
	
	if(num != 0){
		tp->m_num = num;
	}
	tp->m_head = (pthread_t *) malloc(sizeof(pthread_t)*tp->m_num);
	printf("tp->m_head: %ld\n",(long long)tp->m_head);
	for(i = 0; i < tp->m_num; ++i){
		if(pthread_create(&tp->m_head[i], NULL, threadpool_worker, tp) != 0){
			printf("create thread failed, errno : %s",strerror(errno));
			exit(-1);
		}
	}
	printf("thread pool init success\n");

	return 0;
}

void *threadpool_worker(void *arg){
	threadpool_t *tp = (threadpool_t *)arg;
	threadpool_task_t task; 
	
	while(1){
		pthread_mutex_lock(&tp->m_mutex);
		
		while((tp->m_run == THREADPOOL_RUN) && (tp->m_task.m_size == 0)){
			pthread_cond_wait(&tp->m_cond, &tp->m_mutex);
		}
		
		if(tp->m_run == THREADPOOL_STOP){
			pthread_mutex_unlock(&tp->m_mutex);
			printf("thread %d exit\n",pthread_self());
			pthread_exit(NULL);
		}
		
		if(threadpool_queue_pop(&tp->m_task, &task) != 0){
			printf("calling threadpool_queue_pop() failed\n");
			pthread_mutex_unlock(&tp->m_mutex);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&tp->m_mutex);
		(*task.m_fp)(task.m_arg);
		
		task.m_fp = NULL;
		task.m_arg = NULL;
	}
}

int threadpool_add(threadpool_t *tp, threadpool_task_t *task){
	pthread_mutex_lock(&tp->m_mutex);
	
	threadpool_queue_push(&tp->m_task, task);
	pthread_cond_signal(&tp->m_cond);
	
	pthread_mutex_unlock(&tp->m_mutex);
	
	return 0;
}

int threadpool_destory(threadpool_t *tp){
	int i;
	
	tp->m_run = THREADPOOL_STOP;
	pthread_mutex_lock(&tp->m_mutex);
	
	pthread_cond_broadcast(&tp->m_cond);
	
	pthread_mutex_unlock(&tp->m_mutex);
	
	for(i = 0; i < tp->m_num; ++i){
		pthread_join(tp->m_head[i], NULL);
		pthread_cond_broadcast(&tp->m_cond);
	}
	
	threadpool_queue_destory(&tp->m_task);

	free(tp->m_head);
	return 0;
}