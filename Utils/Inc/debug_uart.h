/*
 * debug_uart.h
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_DEBUG_UART_H_
#define INC_DEBUG_UART_H_

#define print_uart(format, ...) print_debug_msg(format, __VA_ARGS__)	// Macro to print debug message to UART 3

void debug_uart_init(void);	// Initialize UART 3 for debugging
void print_debug_msg(const char *message,...);	// Print debug message to UART 3

#endif /* INC_DEBUG_UART_H_ */
