/*
 * buffer.h
 *
 *  Created on: Oct 19, 2018
 *      Author: ea871
 */

#ifndef BUFFER_H_
#define BUFFER_H_

typedef struct {
    char *buffer;
    unsigned char head;
    unsigned char tail;
    unsigned char maxlen;
} buffer_circular;

buffer_circular* buffer_init(unsigned char max);

int buffer_push(buffer_circular *b, char data);

int buffer_pop(buffer_circular *b, char *data);
#endif /* BUFFER_H_ */
