/*
 * oled_display_task.c
 *
 *  Created on: Jul 13, 2025
 *      Author: Dinesh Bobburu
 */


#include "oled_display_task.h"

void oled_display_task(void *pParameters);

TaskHandle_t oled_display_task_handle;


void oled_display_task_Init(void) {
	BaseType_t status;
	status = xTaskCreate(oled_display_task, "OLED_DIS", TASK_STACK_SIZE, NULL, 2, &oled_display_task_handle);
	configASSERT_RTOS(status == pdPASS, "Oled Display Init Failed...\r\n");
}

void oled_display_task(void *pParameters) {
	while(1) {

	}
}
