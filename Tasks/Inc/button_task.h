/*
 * button_task.h
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_BUTTON_TASK_H_
#define INC_BUTTON_TASK_H_

void button_task_init(void);
void v_button_task_handler(void *pvParameters);


TaskHandle_t get_button_task_handle(void);	// return task handle of button_task

#endif /* INC_BUTTON_TASK_H_ */
