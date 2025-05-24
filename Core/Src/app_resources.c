/*
 * app_resources.c
 *
 *  Created on: May 24, 2025
 *      Author: Dinesh Bobburu
 */


#include "app_resources.h"

SemaphoreHandle_t	xbutton_Sema;
Tasks_t current_task;
TaskHandle_t next_task_to_delete;
void app_resources_init(void) {

	xbutton_Sema = xSemaphoreCreateBinary();
	if(!xbutton_Sema) {
			print_error_uart();
	}
}
