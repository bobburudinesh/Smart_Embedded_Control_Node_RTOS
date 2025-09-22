/*
 * logger_task.c
 *
 *  Created on: Aug 14, 2025
 *      Author: Dinesh Bobburu
 */


#include "logger_task.h"

#include "sensor_task.h"
#define MAX_SENSOR_FIELDS		5;
void logger_task(void *arg) {
	logger_packet_t logger_data = {0};
	uint8_t isChunckFull = false;
	while(1) {
		sensor_data_t	sensor_data;
		uint32_t timeout = 0;
		if(( logger_data.payload_len + estimate_sensor_tlv_size(&sensor_data)) > LOGGER_PAYLOAD_SZ){
				isChunckFull = true;
			}
		if(xQueueReceive(qSensorToLogger, &sensor_data, 0) == pdTRUE) {
			switch(sensor_data.sensor_id){
			case MSG_SENSOR_SAMPLE:
				logger_data.age_ms = ms_now();
				uint16_t L = (uint16_t)strnlen(sensor_data.len, LINEBUF_SZ);
				uint8_t *p = logger_data.payload;
				if(p+2+L > (p+sizeof(logger_data.payload))) {
					L = (uint16_t)(sizeof(logger_data.payload) - 2);
				}
				*p++ = 0x10;

				*(uint16_t*)p++ = L;
				memcpy(p, sensor_data.line, L);
				p += L;
				logger_data.payload_len = (uint16_t)(p - logger_data.payload);
				if(xQueueSend(qLoggerToCrypto, &logger_data, 0 == pdTRUE)) {
					logger_data = memset(&logger_data, sizeof(logger_packet_t));

				} else {
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


static manufactureLoggerPayload(sensor_data_t *sensor_data, logger_packet_t *logger_data) {
	uint8_t id = 0;
	uint8_t *p = &logger_data->payload[logger_data->payload_len];
	uint16_t current_sensor_line_length = sensor_data->len;
		// load version into playload
		*p++ = id;
		*p++ = 1;
		*p++ = 2;
		id++;
		// load age of message
		*p++ = id;
		*p++ = sizeof(uint32_t);
		memcpy(p, &sensor_data->age_ms, sizeof(uint32_t));
		p += sizeof(uint32_t);
		id++;
		//load sensor id tag
		*p++ = id;
		*p++ = 1;
		*p++ = sensor_data->sensor_id;
		id++;
		// load length of data
		*p++ = id;
		*p++ = sizeof(uint16_t);
		memcpy(p, &current_sensor_line_length, sizeof(uint16_t));
		p += sizeof(uint16_t);
		id++;
		// load data
		*p++ = id;
		memcpy(p, &current_sensor_line_length, sizeof(uint16_t));
		p += sizeof(uint16_t);
		strncpy(p, sensor_data->line, current_sensor_line_length);
		p +=current_sensor_line_length;
		logger_data->payload_len = p - logger_data->payload;
}


uint16_t estimate_sensor_tlv_size(sensor_data_t *sd) {
    return 3 + 6 + 3 + 4 + 3 + sd->len;
}

/*
 * void logger_task(void *params) {
    logger_chunk_t chunk;
    sensor_data_t incoming;
    TickType_t timeout = pdMS_TO_TICKS(LOGGER_TIMEOUT_MS);

    chunk.count = 0;
    chunk.total_size = 0;

    while (1) {
        if (xQueueReceive(xLoggerQueue, &incoming, timeout) == pdTRUE) {
            uint16_t padded_size = sizeof(incoming.sensor_id) +
                                   sizeof(incoming.timestamp) +
                                   sizeof(incoming.data_len) +
                                   incoming.data_len;

            if ((chunk.total_size + padded_size <= MAX_CHUNK_SIZE) &&
                chunk.count < MAX_SENSOR_PER_CHUNK) {

                memcpy(&chunk.packets[chunk.count], &incoming, sizeof(sensor_data_t));
                chunk.total_size += padded_size;
                chunk.count++;

                // Adjust timeout: reset since we just received something
                timeout = pdMS_TO_TICKS(LOGGER_TIMEOUT_MS);
            } else {
                // Current chunk full, send it
                xQueueSend(xCryptoQueue, &chunk, portMAX_DELAY);

                // Start new chunk with this packet
                chunk.count = 0;
                chunk.total_size = 0;
                memcpy(&chunk.packets[0], &incoming, sizeof(sensor_data_t));
                chunk.total_size += padded_size;
                chunk.count = 1;

                timeout = pdMS_TO_TICKS(LOGGER_TIMEOUT_MS);
            }
        } else {
            // Timeout happened, flush whatever is in the chunk
            if (chunk.count > 0) {
                xQueueSend(xCryptoQueue, &chunk, portMAX_DELAY);
                chunk.count = 0;
                chunk.total_size = 0;
            }

            // Reset timeout
            timeout = pdMS_TO_TICKS(LOGGER_TIMEOUT_MS);
        }
    }
}
 * */
