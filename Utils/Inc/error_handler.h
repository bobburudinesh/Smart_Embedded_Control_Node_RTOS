/*
 * error_handler.h
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_ERROR_HANDLER_H_
#define INC_ERROR_HANDLER_H_

#define print_error_uart()	Error_Handler_System(__FILE__, __LINE__, __func__)	// Print function where error occurred and hand in there.

void Error_Handler_RTOS(const char *file, int line, const char *message);
void Error_Handler_System(const char *file, int line, const char *message);


#endif /* INC_ERROR_HANDLER_H_ */
