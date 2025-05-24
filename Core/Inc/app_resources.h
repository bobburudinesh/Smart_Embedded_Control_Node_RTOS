/*
 * app_resources.h
 *
 *  Created on: May 24, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_APP_RESOURCES_H_
#define INC_APP_RESOURCES_H_
#include "main_SECN.h"

extern SemaphoreHandle_t	xbutton_Sema;

extern TaskHandle_t next_task_to_delete;




typedef enum {
	idle_Task = 0,
	Led_Green_Task,
	Led_Orange_Task,
	Button_Task
}Tasks_t;

extern Tasks_t current_task;

#endif /* INC_APP_RESOURCES_H_ */
