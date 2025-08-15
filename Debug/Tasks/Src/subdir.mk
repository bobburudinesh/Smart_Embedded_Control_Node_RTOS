################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Tasks/Src/Activity_Monitor_Task.c \
../Tasks/Src/crypto_task.c \
../Tasks/Src/logger_task.c \
../Tasks/Src/oled_display_task.c \
../Tasks/Src/sensor_task.c 

OBJS += \
./Tasks/Src/Activity_Monitor_Task.o \
./Tasks/Src/crypto_task.o \
./Tasks/Src/logger_task.o \
./Tasks/Src/oled_display_task.o \
./Tasks/Src/sensor_task.o 

C_DEPS += \
./Tasks/Src/Activity_Monitor_Task.d \
./Tasks/Src/crypto_task.d \
./Tasks/Src/logger_task.d \
./Tasks/Src/oled_display_task.d \
./Tasks/Src/sensor_task.d 


# Each subdirectory must supply rules for building sources it contributes
Tasks/Src/%.o Tasks/Src/%.su Tasks/Src/%.cyclo: ../Tasks/Src/%.c Tasks/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/Sensors/Inc" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/Utils/Inc" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/Tasks/Inc" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/FreeRTOS/portable/GCC/ARM_CM4F" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/SEGGER/Config" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/SEGGER/OS" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/SEGGER/SEGGER" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/FreeRTOS" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/FreeRTOS/include" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/FreeRTOS/portable/GCC" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Tasks-2f-Src

clean-Tasks-2f-Src:
	-$(RM) ./Tasks/Src/Activity_Monitor_Task.cyclo ./Tasks/Src/Activity_Monitor_Task.d ./Tasks/Src/Activity_Monitor_Task.o ./Tasks/Src/Activity_Monitor_Task.su ./Tasks/Src/crypto_task.cyclo ./Tasks/Src/crypto_task.d ./Tasks/Src/crypto_task.o ./Tasks/Src/crypto_task.su ./Tasks/Src/logger_task.cyclo ./Tasks/Src/logger_task.d ./Tasks/Src/logger_task.o ./Tasks/Src/logger_task.su ./Tasks/Src/oled_display_task.cyclo ./Tasks/Src/oled_display_task.d ./Tasks/Src/oled_display_task.o ./Tasks/Src/oled_display_task.su ./Tasks/Src/sensor_task.cyclo ./Tasks/Src/sensor_task.d ./Tasks/Src/sensor_task.o ./Tasks/Src/sensor_task.su

.PHONY: clean-Tasks-2f-Src

