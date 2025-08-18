/*
 * modem_task.c
 *
 *  Created on: Aug 17, 2025
 *      Author: Dinesh Bobburu
 */

#include "modem_task.h"
#include "ring_buffer.h"
#include "uart_async.h"

uart_async_t uart_async_modem_t;

static inline int uart6_write(const void *buf, uint16_t len, uint32_t to_ms){
  if (HAL_UART_Transmit(&huart6_modem, (uint8_t*)buf, len, to_ms) == HAL_OK) return 0;
  LOGE("Modem UART TX failed"); return -1;
}

void modem_task(void *args) {
	uart_async_init(&uart_async_modem_t, &huart6_modem);
	if(!uart_async_modem_t.uart_semaHandle) {
		LOGE("Failed to init modem uart sema");
	}
	uart_async_enable_irqs(&huart6_modem);
	LOGI("Modem Init Successful");

	while(1) {
		tx_payload_t tx_modem;
		if(xQueueReceive(qCryptoToModem, &tx_modem, 0) == pdTRUE) {

			(void)uart6_write("AT+ECHO=\"", 9, 100);
			(void)uart6_write(tx_modem.buf, tx_modem.len, 200);
			(void)uart6_write("\"\r", 2, 50);
			LOGI("Payload sent");
		}
	}
}
