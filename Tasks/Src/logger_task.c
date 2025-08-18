/*
 * logger_task.c
 *
 *  Created on: Aug 14, 2025
 *      Author: Dinesh Bobburu
 */


#include "logger_task.h"

#include "sensor_task.h"

void logger_task(void *arg) {
	while(1) {
		sensor_data_t	sensor_data;
		if(xQueueReceive(qSensorToLogger, &sensor_data, 0) == pdTRUE) {
			logger_packet_t logger_data;
			switch(sensor_data.msg_type){
			case MSG_SENSOR_SAMPLE:
				logger_data.age_ms = ms_now();
				uint16_t L = (uint16_t)strnlen(sensor_data.line, 256);
				uint8_t *p = logger_data.payload;
				if(p+2+L > (p+sizeof(logger_data.payload))) {
					L = (uint16_t)(sizeof(logger_data.payload) - 2);
				}
				*p++ = 0x10;

				*p++ = (uint16_t)L;
				memcpy(p, sensor_data.line, L);
				p += L;
				logger_data.payload_len = (uint16_t)(p - logger_data.payload);
				if(xQueueSend(qLoggerToCrypto, &logger_data, 0 != pdTRUE)) {
					// TODO: Handle queue error
					logger_packet_t trash;
					(void)xQueueReceive(qLoggerToCrypto, &trash, 0);
					if(xQueueSend(qLoggerToCrypto, &logger_data, 0 != pdTRUE)) {
						LOGW("qCryptoToModem full (drop)");
					}

				}
				break;

			default:
				break;
			}


		} else {
			// TODO: Handle Queue Error
		}
	}
}
