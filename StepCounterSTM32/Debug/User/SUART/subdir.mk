################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/SUART/S_UART.c 

OBJS += \
./User/SUART/S_UART.o 

C_DEPS += \
./User/SUART/S_UART.d 


# Each subdirectory must supply rules for building sources it contributes
User/SUART/%.o User/SUART/%.su User/SUART/%.cyclo: ../User/SUART/%.c User/SUART/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../User -I../User/LCD -I../User/LSM9DS0 -I../User/SUART -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User-2f-SUART

clean-User-2f-SUART:
	-$(RM) ./User/SUART/S_UART.cyclo ./User/SUART/S_UART.d ./User/SUART/S_UART.o ./User/SUART/S_UART.su

.PHONY: clean-User-2f-SUART

