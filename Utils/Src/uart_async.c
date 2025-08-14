/*
 * uart_async.c
 *
 *  Created on: Aug 12, 2025
 *      Author: Dinesh Bobburu
 */


#include "uart_async.h"
#include "stdint.h"
#include "stdbool.h"

#define ENABLE_CHECKSUM		0U

static bool compute_checksum_gps_sensor(uint8_t *data);
enum { WAIT_DOLLAR = 0, COLLECT };
void uart_async_init(uart_async_t *aUart, UART_HandleTypeDef *huart) {
	aUart->huart = huart;
	aUart->uart_semaHandle = xSemaphoreCreateBinary();
	aUart->asm_i = 0;
	aUart->asm_overflowed = WAIT_DOLLAR;
	aUart->asm_state = 0;
	ringbuffer_Init(&aUart->ring_buf, aUart->buffer, RINGBUF_SZ);
}
void uart_async_isr_byte(uart_async_t *aUart, BaseType_t *highPriorityTaskWoken) {
	USART_TypeDef *hUartInstance = aUart->huart->Instance;
	volatile uint32_t SR = hUartInstance->SR; // read SR then DR that will clear RXNE.
	uint8_t c = (uint8_t)(hUartInstance->DR & 0xFF);
	ringbuffer_push_isr(&aUart->ring_buf, c);
	if(c == '\n') {// End of Line from sensor
		xSemaphoreGiveFromISR(aUart->uart_semaHandle, highPriorityTaskWoken);
	}
	if (__HAL_UART_GET_FLAG(&huart1_sensor, UART_FLAG_IDLE)) {
	        __HAL_UART_CLEAR_IDLEFLAG(&huart1_sensor);
	        xSemaphoreGiveFromISR(aUart->uart_semaHandle, highPriorityTaskWoken);
	   }

}

static bool compute_checksum_gps_sensor(uint8_t *data) {
	if(*data != '$') {
		return false;
	}
	uint8_t *c = data;
	c++;
	uint8_t checksum = 0;

	while(*c && (*c != '*' && (*c != '\r') && (*c != '\n'))) { checksum ^= (uint8_t)(*c++);}
	if(*c != '*') {
		return false;
	}
	if(!c[1] || !c[2]) {return false;}
	uint8_t high = ((c[1] <= '9') ? c[1] - '0' : (c[1]&0x5F) - 'A' + 10 );
	uint8_t low = ((c[2] <='9') ? c[2] - '0' : (c[2]&0x5F) - 'A' + 10 );
	uint8_t actual_checksum = (uint8_t)((high << 4) | low);
	return (checksum == actual_checksum);

}
bool uart_async_getline(uart_async_t *aUart, uint8_t *buf, uint16_t maxSize, bool *checksum_OK) {
	uint8_t c;
	if(*checksum_OK) {*checksum_OK = false;}
	while(ringbuffer_pop(&aUart->ring_buf, &c)) {
		switch(aUart->asm_state) {
		case WAIT_DOLLAR:
			if(c == '$') {
				aUart->asm_i = 0;
				if((aUart->asm_i+1) <maxSize) {
					buf[aUart->asm_i++] = c;
					aUart->asm_state = COLLECT;
				}
			}
			break;
		case COLLECT:
			if(c == '\n') {
				if((!aUart->asm_overflowed) && (aUart->asm_i<maxSize)) {
					buf[aUart->asm_i] = 0;
				}

				aUart->asm_state = WAIT_DOLLAR;
				aUart->asm_i = 0;
				if(aUart->asm_overflowed) {
					aUart->asm_overflowed = false;
				}
				if(ENABLE_CHECKSUM) {
					if(compute_checksum_gps_sensor(buf)) {
						*checksum_OK = true;
					}
				} else {
					*checksum_OK = true;
				}
				return true;
			} else {
				if(aUart->asm_i+1 < maxSize){buf[aUart->asm_i++] = (char)c;}
				else {
					// Overflow happened keep discarding until \n
					aUart->asm_overflowed = true;
					//aUart->asm_i = 0;
					//aUart->asm_state = WAIT_DOLLAR;
				}
			}
			break;
		}

	}
	return false;
}
uint16_t uart_async_write(uart_async_t *aUart, const uint8_t *buf, uint16_t len, uint32_t delay) {
	if(HAL_UART_Transmit(aUart->huart, buf, len, delay) == HAL_OK) {return len;}
	return 0;

}

void uart_async_enable_irqs(uart_async_t *aUart){
    __HAL_UART_ENABLE_IT(aUart->huart, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(aUart->huart, UART_IT_IDLE);
}
