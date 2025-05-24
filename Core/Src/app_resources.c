/*
 * app_resources.c
 *
 *  Created on: May 24, 2025
 *      Author: Dinesh Bobburu
 */


#include "app_resources.h"

#define SENSOR_QUEUE_LENGTH 	10

SemaphoreHandle_t	xbutton_Sema;	// Semaphore to synchronize button ISR to button task

Tasks_t current_task;

TaskHandle_t next_task_to_delete;

QueueHandle_t	xSensor_Queue;


void app_resources_init(void) {

	xbutton_Sema = xSemaphoreCreateBinary();
	if(!xbutton_Sema) {
		print_error_uart();
	}
	xSensor_Queue = xQueueCreate(SENSOR_QUEUE_LENGTH, sizeof(uint8_t));
	if(!xSensor_Queue) {
		print_error_uart();
	}
}
