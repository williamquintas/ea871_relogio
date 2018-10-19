#include "buffer.h"
#include <stdlib.h>

buffer_circular* buffer_init(unsigned char max) {
	char *pointer = (char *) malloc(max * sizeof(char));
	buffer_circular *p = (buffer_circular *) malloc(sizeof(buffer_circular));
	p->buffer = pointer;
	p->head = 0;
	p->tail = 0;
	p->maxlen = max;
	return p;
}

int buffer_push(buffer_circular *b, char data){
	unsigned char next;
	next = b->head + 1;  // next is where head will point to after this write.
	if (next >= b->maxlen)
		next = 0;
	if (next == b->tail)  // if the head + 1 == tail, circular buffer is full
		return -1;
	b->buffer[b->head] = data;  // Load data and then move
	b->head = next;             // head to next data offset.
	return 0;  // return success to indicate successful push.
}

int buffer_pop(buffer_circular *b, char *data){
	unsigned char next;
	if (b->head == b->tail)  // if the head == tail, we don't have any data
		return -1;
	next = b->tail + 1;  // next is where tail will point to after this read.
	if(next >= b->maxlen)
		next = 0;
	*data = b->buffer[b->tail];  // Read data and then move
	b->tail = next;              // tail to next offset.
	return 0;  // return success to indicate successful push.
}

