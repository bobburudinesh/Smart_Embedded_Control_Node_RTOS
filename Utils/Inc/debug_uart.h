/*
 * debug_uart.h
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_DEBUG_UART_H_
#define INC_DEBUG_UART_H_

#define print_uart(format, ...) print_debug_msg(format, __VA_ARGS__)

void debug_uart_init(void);
void print_debug_msg(const char *message,...);

#endif /* INC_DEBUG_UART_H_ */
