/*
 * debug_uart.h
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_DEBUG_UART_H_
#define INC_DEBUG_UART_H_
#include "app_resources.h"

/* prefixes include file/line for errors */
#define LOGI(fmt, ...)  dbg_printf("[I] " fmt "\r\n", ##__VA_ARGS__)
#define LOGW(fmt, ...)  dbg_printf("[W] " fmt "\r\n", ##__VA_ARGS__)
#define LOGE(fmt, ...)  dbg_printf("[E] %s:%d: " fmt "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)


//
//#define print_uart(format, ...) print_debug_msg(format, __VA_ARGS__)	// Macro to print debug message to UART 3
//
//void uart_debug_init(void);	// Initialize UART 3 for debugging
//void print_debug_msg(const char *message,...);	// Print debug message to UART 3

static inline void dbg_vprintf(const char *fmt, va_list ap);
static inline void dbg_printf(const char *fmt, ...);

extern UART_HandleTypeDef	huart3;

#endif /* INC_DEBUG_UART_H_ */
