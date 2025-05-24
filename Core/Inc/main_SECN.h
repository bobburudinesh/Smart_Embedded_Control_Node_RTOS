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

#include"string.h"
#include"stdio.h"
#include "stdlib.h"
#include "stdarg.h"

#include "error_handler.h"
#include "debug_uart.h"

void button_irq_handler(void);



#endif /* INC_MAIN_SECN_H_ */
