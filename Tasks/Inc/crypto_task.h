/*
 * crypto_task.h
 *
 *  Created on: Aug 14, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_CRYPTO_TASK_H_
#define INC_CRYPTO_TASK_H_

#include "app_resources.h"

void crypto_task(void *args) {


	while(1) {
		logger_packet_t	raw_payload;
		if(xQueueReceive(qLoggerToCrypto, &raw_payload, 0) == pdTRUE) {
			tx_payload_t	encrypted_payload;
			// TODO: Handle Cryptography

			encrypted_payload.age_ms = raw_payload.age_ms;
			encrypted_payload.buf = raw_payload.payload;
			encrypted_payload.len = raw_payload.payload_len;

			if(xQueueSend(qCryptoToModem, &encrypted_payload, 0) != pdTRUE) {
				// TODO: Handle queue error
			}
		} else {
			//TODO: Handle queue Error
		}

	}
}


#endif /* INC_CRYPTO_TASK_H_ */
