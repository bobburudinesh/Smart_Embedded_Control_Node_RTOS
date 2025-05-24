/*
 * sensor_task.c
 *
 *  Created on: May 24, 2025
 *      Author: Dinesh Bobburu
 */

#include "main_SECN.h"
#include "app_resources.h"
#include "sensor_task.h"

TaskHandle_t	sensor_task_handle;

BaseType_t	status;
void sensor_task_init(void) {
	status = xTaskCreate(v_sensor_task, "Sensor_Task", TASK_STACK_SIZE, NULL, 2, &sensor_task_handle);
	configASSERT_RTOS(status == pdPASS, "Sensor Task Init Failed\n");
}

void v_sensor_task(void *pvParamerter) {
	uint8_t data;
	while(1) {
		 data = generate_sensor_data();
		if(xQueueSend(xSensor_Queue,(void *)&data,pdMS_TO_TICKS(200)) != pdPASS) {
			print_error_uart();
		}
		print_debug_msg("Data Sent ON Queue: %u\n", data);
		vTaskDelay(pdMS_TO_TICKS(2000));
	}
}

TaskHandle_t get_sensor_task_handle(void) {
	return sensor_task_handle;
}

static uint8_t generate_sensor_data(void) {
	uint8_t value = (uint8_t)(rand() % 10);
	return value;
}
