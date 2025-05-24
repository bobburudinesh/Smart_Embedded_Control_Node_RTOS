/*
 * led_task.h
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_LED_TASK_H_
#define INC_LED_TASK_H_

void led_task_init(void);	// This creates LED_TASKS for Green and Orange LED's

void v_led_task_green(void *pvParameters);	//This function is task handler for Green LED
void v_led_task_orange(void *pvParameters);	//This function is task handler for Orange LED

TaskHandle_t get_led_task_green_handle(void);	// returns task handle of led_task_green
TaskHandle_t get_led_task_orange_handle(void);	// returns task handle of led_task_orange


#endif /* INC_LED_TASK_H_ */
