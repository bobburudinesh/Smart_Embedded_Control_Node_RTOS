/*
 * app_resources.h
 *
 *  Created on: May 24, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_APP_RESOURCES_H_
#define INC_APP_RESOURCES_H_

#pragma once
#include "stm32f4xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "event_groups.h"

#include"string.h"
#include"stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "stdint.h"
#include "stdbool.h"

#define BIT_0	( 1 << 0 )
#define BIT_1	( 1 << 1 )


#define DEBUG_ENABLE		1U

#if DEBUG_ENABLE
#define DEBUG_UART_PORT		UART3
#define SEGGER_SYSTEM_VIEW_UART_PORT	UART2
#endif

#define SENSOR_UART_PORT	UART1
#define MODEM_UART_PORT		UART6
#define SENSOR_TASK_PRIO	(tskIDLE_PRIORITY + 3)
#define MODEM_TASK_PRIO		(tskIDLE_PRIORITY + 3)
#define LOGGER_TASK_PRIO	(tskIDLE_PRIORITY + 2)
#define CRYPTO_TASK_PRIO	(tskIDLE_PRIORITY + 2)
#define OTA_TASK_PRIO		(tskIDLE_PRIORITY + 1)
#define SYSTEM_TASK_PRIO	(tskIDLE_PRIORITY + 4)

#define LOGGER_QUEUE_LEN		(32)
#define MODEM_QUEUE_LEN			(16)
#define CRYPTO_QUEUE_LEN		(16)

#define SENSOR_POLL_PERIOD_MS		(pdMS_TO_TICKS(1000))

#define RINGBUF_SZ				(1024)

#define LINEBUF_SZ				(128)

#define LOGGER_PAYLOAD_SZ		(512)


typedef enum {
	idle_Task = 0,
	Led_Green_Task,
	Led_Orange_Task,
	Button_Task
}Tasks_t;

typedef enum {
	SYS_STATE_INIT = 0,
	SYS_STATE_RUNNING,
	SYS_STATE_ERROR,
	SYS_STATE_RESET
} system_state_t;

typedef enum {
	MSG_SENSOR_SAMPLE,
	MSG_LOG_PACKET,
	MSG_CRYPTO_ENCRYPT_REQ,
	MSG_CRYPTO_ENCRYPTED,
	MSG_SYSTEM_HEARTBEAT,

	MSG_LIMIT_EXCEDDED = 255
} msg_type_t;

typedef struct {
	uint32_t age_ms;
	char line[LINEBUF_SZ];	//TODO: make dynamic length sensor buffer
	uint16_t len;
	uint8_t sensor_id;
} sensor_data_t;

typedef struct {
	uint32_t age_ms;
	uint8_t payload[LINEBUF_SZ+8];
	uint16_t payload_len;
} logger_packet_t;

typedef struct {
	uint32_t age_ms;
	uint8_t buf[LINEBUF_SZ+32];
	uint16_t len;
} tx_payload_t;

typedef struct {
	uint32_t corr_id;
	uint8_t in[256];
	uint16_t in_len;
} crypto_encrypt_req_t;

typedef struct {
	uint32_t corr_id;
	uint8_t out[256];
	uint16_t out_len;
	uint8_t status;
} crypto_encrypt_resp_t;


extern QueueHandle_t qSensorToLogger;
extern QueueHandle_t qLoggerToCrypto;
extern QueueHandle_t qCryptoToModem;

extern UART_HandleTypeDef huart1_sensor;
extern UART_HandleTypeDef huart6_modem;

static inline uint32_t ms_now(void) {return (uint32_t)(xTaskGetTickCount()*portTICK_PERIOD_MS);}

/*
void app_resources_init(void);

extern SemaphoreHandle_t	xbutton_Sema;

extern QueueHandle_t	xSensor_Queue;

extern TaskHandle_t next_task_to_delete;

extern EventGroupHandle_t	xSensor_Button_Event_Group;

extern QueueHandle_t	xSensor_Queue_Ultrasonic;

void vHeartbeat_Timer_Callback( TimerHandle_t xTimer );


*/





#endif /* INC_APP_RESOURCES_H_ */
