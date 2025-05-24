################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c 

OBJS += \
./SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o 

C_DEPS += \
./SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d 


# Each subdirectory must supply rules for building sources it contributes
SEGGER/SEGGER/Syscalls/%.o SEGGER/SEGGER/Syscalls/%.su SEGGER/SEGGER/Syscalls/%.cyclo: ../SEGGER/SEGGER/Syscalls/%.c SEGGER/SEGGER/Syscalls/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/Utils/Inc" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/Tasks/Inc" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/FreeRTOS/portable/GCC/ARM_CM4F" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/SEGGER/Config" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/SEGGER/OS" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/SEGGER/SEGGER" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/FreeRTOS" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/FreeRTOS/include" -I"/Users/dineshbobburu/Documents/Embedded Systems/Projects/STM32/Smart_Embedded_Control_Node_RTOS/FreeRTOS/portable/GCC" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SEGGER-2f-SEGGER-2f-Syscalls

clean-SEGGER-2f-SEGGER-2f-Syscalls:
	-$(RM) ./SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.cyclo ./SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d ./SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o ./SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.su

.PHONY: clean-SEGGER-2f-SEGGER-2f-Syscalls

