Copyright 20190508 Peng Wen(https://github.com/expectmeeting)

Licensed under the Apache License, Version 2.0;
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

#ifndef QUEUE_H_
#define QUEUE_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/** @type macro definition
 *  @brief the macro is used to declare the type of element in the queue, the default type is int, 
 *  and could be changed by user
 */
#ifndef queue_type
#define queue_type int
#endif

/** @type macro definition
 *  @brief the macro is used to static initializer the variable of type queue_t 
 */
#define QUEUE_INITIALIZER {0, -1, 0, 8, (queue_type *)malloc(sizeof(queue_type)*8)}
 
 
/** @type struct
 *  @brief a queue struct is used to manage circular queue
 *  @param m_head -> the first element stored in the queue
 *  @param m_tail -> the last element stored in the queue
 *  @param m_size -> the number of elements stored in the queue
 *  @param m_capacity -> the number of elements that the requested storage space can hold
 *  @param m_buf -> pointer to the requested storage space
 */
typedef struct queue{
	int m_head;
	int m_tail;
	int m_size;
	int m_capacity;
	queue_type *m_buf;
}queue_t;

/** @type function
 *  @brief the function is used to dynamic initializer the variable of type queue_t
 *  @param queue -> the pointer of queue_t type
 *  @return void
 */
void queue_init(queue_t *queue);

/** @type function
 *  @brief the function is used to add element
 *  @param queue -> the pointer of queue_t type
 *  @param value -> the pointer of queue_type type, which point to element
 *  @return 0 -> success ; -1 -> failed
 */
int queue_push(queue_t *queue, queue_type *value);

/** @type function
 *  @brief the function is used to delete element
 *  @param queue -> the pointer of queue_t type
 *  @param value -> the pointer of queue_type type, which point to element
 *  @return 0 -> success ; -1 -> failed
 */
int queue_pop(queue_t *queue, queue_type *task);

/** @type function
 *  @brief the function is used to destory object of queue_t type, and it can be reininitialized
 *  @param queue -> the pointer of queue_t type
 *  @return 0 -> success ; -1 -> failed
 */
int queue_destory(queue_t *queue);

/** @type function
 *  @brief the function is used to confirm queue is empty or not empty
 *  @param queue -> the pointer of queue_t type
 *  @return 0 -> nor empty ; 1 -> empty
 */
int queue_empty(queue_t *queue);
#endif