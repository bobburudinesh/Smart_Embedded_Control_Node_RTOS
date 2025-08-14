/*
 * ring_buffer.h
 *
 *  Created on: Aug 12, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_RING_BUFFER_H_
#define INC_RING_BUFFER_H_

#include "app_resources.h"
#include "stdbool.h"
#include "stdint.h"

typedef struct {
	volatile uint16_t head, tail;
	uint16_t capacity; // it will always be power of 2
	uint8_t *buffer;
	//
}ring_buffer_t;

void ringbuffer_Init(ring_buffer_t *rb, uint8_t *buffer, uint16_t size);
bool ringbuffer_push_isr(ring_buffer_t *rb, uint8_t byte);
bool ringbuffer_pop(ring_buffer_t *rb, uint8_t *byte);
int ringbuffer_read(ring_buffer_t *rb, uint8_t *dest, uint16_t numberOfBytes);

#endif /* INC_RING_BUFFER_H_ */
