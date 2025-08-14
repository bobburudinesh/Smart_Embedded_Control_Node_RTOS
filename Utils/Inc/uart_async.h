/*
 * uart_async.h
 *
 *  Created on: Aug 12, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_UART_ASYNC_H_
#define INC_UART_ASYNC_H_

#include "app_resources.h"
#include "ring_buffer.h"
#include "stdbool.h"

typedef struct {
	UART_HandleTypeDef *huart;
	ring_buffer_t ring_buf;
	uint8_t buffer[RINGBUF_SZ];
	xSemaphoreHandle uart_semaHandle;

	uint16_t asm_i;
	uint8_t  asm_state;
	bool     asm_overflowed;
}uart_async_t;

void uart_async_init(uart_async_t *aUart, UART_HandleTypeDef *huart);
void uart_async_isr_byte(uart_async_t *aUart,  BaseType_t *highPriorityTaskWoken);
bool uart_async_getline(uart_async_t *aUart, uint8_t *buf, uint16_t maxSize, bool *checksum_OK);
uint16_t uart_async_write(uart_async_t *aUart, const uint8_t *buf, uint16_t len, uint32_t delay);
void uart_async_enable_irqs(uart_async_t *aUart);

#endif /* INC_UART_ASYNC_H_ */
