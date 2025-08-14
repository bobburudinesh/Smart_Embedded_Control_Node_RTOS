/*
 * ring_buffer.c
 *
 *  Created on: Aug 12, 2025
 *      Author: Dinesh Bobburu
 */


#include "ring_buffer.h"

void ringbuffer_Init(ring_buffer_t *rb, uint8_t *buffer, uint16_t size) {
	rb->head = 0;
	rb->tail = 0;
	rb->capacity = size;
	rb->buffer = buffer;
}
bool ringbuffer_push_isr(ring_buffer_t *rb, uint8_t byte) {
	uint16_t position = (rb->head+1) & (rb->capacity-1);
	if(position == rb->tail) return false;
	rb->buffer[rb->head] = byte;
	rb->head = position;
	return true;
}
bool ringbuffer_pop(ring_buffer_t *rb, uint8_t *byte) {
	if(rb->head == rb->tail) return false;
	*byte = rb->buffer[rb->tail];
	rb->tail = (rb->tail+1) & (rb->capacity - 1);
	return true;
}
int ringbuffer_read(ring_buffer_t *rb, uint8_t *dest, uint16_t numberOfBytes) {
	uint16_t readBytesCount = 0;
	uint8_t data;
	while((readBytesCount < numberOfBytes) && ringbuffer_pop(rb, &data)) { dest[readBytesCount++] = data;}
	return readBytesCount;
}
