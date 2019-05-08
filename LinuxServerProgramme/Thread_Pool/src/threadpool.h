Copyright 20190508 Peng Wen (https://github.com/expectmeeting)

Licensed under the Apache License, Version 2.0; you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

/** @type enum
 *  @brief the status of thread pool
 *  @param THREADPOOL_STOP -> thread pool is uninitialized ,and can be reinitialized
 *  @param THREADPOOL_RUN -> thread pool has been initialized
 */
typedef enum{
	THREADPOOL_STOP = -1,
	THREADPOOL_RUN = 1
}threadpool_status_t;

/** @type struct
 *  @brief the queue of thread pool used to save task, which include function pointer and argument
 *  @param m_head -> the location of first task in queue
 *  @param m_tail -> the location of last task in queue
 *  @param m_size -> the number of task in queue
 *  @param m_capacity -> the size of queue
 *  @param m_buf -> the pointer that pointing to head address of queue in memory
 */
typedef struct threadpool_queue{
	int m_head;
	int m_tail;
	int m_size;
	int m_capacity;
	void *m_buf;
}threadpool_queue_t;// task queue

/** @type struct
 *  @brief the queue of thread pool used to save task, which include function pointer and argument
 *  @param m_head -> the location of first task in queue
 *  @param m_tail -> the location of last task in queue
 *  @param m_size -> the number of task in queue
 *  @param m_capacity -> the size of queue
 *  @param m_buf -> the pointer that pointing to head address of queue in memory
 */
typedef struct threadpool{
	pthread_mutex_t m_mutex;
	pthread_cond_t	m_cond;
	int m_num;
	threadpool_status_t m_run;
	threadpool_queue_t m_task;
	pthread_t *m_head;
}threadpool_t;


typedef struct threadpool_task{
	void *(*m_fp)(void *);//function point
	void *m_arg;//argument
}threadpool_task_t;

/** @type function
 *  @brief the function is used to dynamic initializer the variable of type threadpool_queue_t
 *  @param queue -> the pointer of threadpool_queue_t type
 *  @return void
 */
void threadpool_queue_init(threadpool_queue_t *queue);

/** @type function
 *  @brief the function is used to add element
 *  @param queue -> the pointer of threadpool_queue_t type
 *  @param task -> the pointer of threadpool_task_t type, which point to element
 *  @return 0 -> success ; -1 -> failed
 */
int threadpool_queue_push(threadpool_queue_t *queue, threadpool_task_t *task);

/** @type function
 *  @brief the function is used to delete element
 *  @param queue -> the pointer of threadpool_queue_t type
 *  @param value -> the pointer of threadpool_task_t type, which point to element
 *  @return 0 -> success ; -1 -> failed
 */
int threadpool_queue_pop(threadpool_queue_t *queue, threadpool_task_t *task);

/** @type function
 *  @brief the function is used to destory object of threadpool_queue_t type, and it can be reininitialized
 *  @param queue -> the pointer of threadpool_queue_t type
 *  @return 0 -> success ; -1 -> failed
 */
int threadpool_queue_destory(threadpool_queue_t *queue);

/** @type function
 *  @brief the function is used to dynamic initializer the variable of type threadpool_t
 *  @param tp -> the pointer of threadpool_t type
 *  @param num -> the number of work thread, if num is 0, default value is set up 10
 *  @return 0 -> success
 */
int threadpool_init(threadpool_t *tp, int num);

/** @type function
 *  @brief the function is the main function of new creat thread
 *  @param arg -> the pointer of void type, which in effect need transmit the object of threadpool_t type
 *  @return void
 */
void *threadpool_worker(void *arg);

/** @type function
 *  @brief the function is used to add new task
 *  @param tp -> the pointer of threadpool_t type
 *  @param task -> the pointer of threadpool_task_t type
 *  @return 0 -> success
 */
int threadpool_add(threadpool_t *tp, threadpool_task_t *task);

/** @type function
 *  @brief the function is used to destory object of threadpool_t type, and it can be reininitialized
 *  @param tp -> the pointer of threadpool_t type
 *  @return 0 -> success 
 */
int threadpool_destory(threadpool_t *tp);


#endif
