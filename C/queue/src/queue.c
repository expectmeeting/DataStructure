#include"queue.h"

void queue_init(queue_t *queue){
	queue->m_head = 0;
	queue->m_tail = -1;
	queue->m_size = 0;
	queue->m_capacity = 8;
	queue->m_buf = (queue_type *)malloc(sizeof(queue_type)*queue->m_capacity);
};

int queue_push(queue_t *queue, queue_type *value){
	if(queue == NULL || value == NULL){
		return -1;
	}
	
	// tempory memory
	queue_type *buf;
	
	//expand capacity
	if(queue->m_size == queue->m_capacity){
		
		queue->m_capacity = queue->m_capacity << 1;	
		buf = (queue_type *)malloc(sizeof(queue_type)*queue->m_capacity);
		if(queue->m_head < queue->m_tail){
			memcpy(buf, queue->m_buf, sizeof(queue_type)*queue->m_size);
		}
		else{
			memcpy(buf, queue->m_buf + queue->m_head, sizeof(queue_type)*(queue->m_size - queue->m_tail - 1));
			memcpy(buf + (queue->m_size - queue->m_tail - 1), queue->m_buf, sizeof(queue_type)*(queue->m_tail + 1));
		}
		free(queue->m_buf);
		queue->m_buf = buf;	
		queue->m_head = 0;
		queue->m_tail = queue->m_size - 1;
	}
	
	if(queue->m_tail < queue->m_capacity - 1){
		queue->m_tail++;
	}
	else if(queue->m_tail == queue->m_capacity - 1){
		queue->m_tail = 0;
	}
	queue->m_size++;
	queue->m_buf[queue->m_tail] = *value;

	return 0;
}

int queue_pop(queue_t *queue, queue_type *value){
	if(queue == NULL || queue->m_size == 0){
		return -1;
	}
	
	*value = queue->m_buf[queue->m_head];
	memset(queue->m_buf+queue->m_head, 0, sizeof(queue_type));
	
	if(queue->m_head < queue->m_capacity - 1){
		queue->m_head++;
	}
	else if(queue->m_head == queue->m_capacity - 1){
		queue->m_head = 0;
	}
	queue->m_size--;
	
	return 0;
}

int queue_destory(queue_t *queue){
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

int queue_empty(queue_t *queue){
	if(queue == NULL){
		return -1;
	}
	
	if(queue->m_size > 0){
		return 0;
	}
	else{
		return 1;
	}
}