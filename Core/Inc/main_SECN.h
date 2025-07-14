/*
 * main_SECN.h
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_MAIN_SECN_H_
#define INC_MAIN_SECN_H_

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

#include "error_handler.h"
#include "debug_uart.h"
#include "app_resources.h"


#define TASK_STACK_SIZE		200

extern RTC_HandleTypeDef	hrtc;

void button_irq_handler(void);
void timer_init_LPM(void); // Test Purpose Timer 2
void timer_init_Basic(void); // test purpose Timer 7.
void get_frequency_from_IC(void);
uint32_t get_PCK1(void);
void timer_OC_Init(void);
void timer_PWM_Init(void);
void rtc_init(void);
void rtc_date_time_config(void);

#endif /* INC_MAIN_SECN_H_ */
