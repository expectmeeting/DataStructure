#include<stdio.h>
#include"queue.h"

#define queue_type int
int main(){
	queue_t queue = QUEUE_INITIALIZER;
	int i = 0;
	printf("%d %d %d %d %ld\n",queue.m_head,queue.m_tail,queue.m_size,queue.m_capacity,queue.m_buf);
	while(i++ < 16){
		queue_push(&queue, &i);
	}
	
	queue_pop(&queue, &i);
	printf("%d\n",i);
	queue_pop(&queue, &i);
	printf("%d\n",i);
	queue_pop(&queue, &i);
	printf("%d\n",i);
	
	
	return 1;
}