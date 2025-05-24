
/*
 * error_handler.c
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */


#include "main_SECN.h"


void Error_Handler_RTOS(const char *file, int line, const char *message) {

	print_debug_msg("%s : Assertion failed in %s, line %d: %s\n", file, file, line, message);
	taskDISABLE_INTERRUPTS();
	for (;;);
}


void Error_Handler_System(const char *file, int line, const char *message) {
	print_debug_msg("%s : Assertion failed in %s, line %d: %s\n", file, file, line, message);
	taskDISABLE_INTERRUPTS();
	for (;;);
}
