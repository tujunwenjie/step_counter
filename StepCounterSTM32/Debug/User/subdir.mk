################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/delay.c \
../User/step_counter.c \
../User/sys.c \
../User/ui.c 

OBJS += \
./User/delay.o \
./User/step_counter.o \
./User/sys.o \
./User/ui.o 

C_DEPS += \
./User/delay.d \
./User/step_counter.d \
./User/sys.d \
./User/ui.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o User/%.su User/%.cyclo: ../User/%.c User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../User -I../User/LCD -I../User/LSM9DS0 -I../User/SUART -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User

clean-User:
	-$(RM) ./User/delay.cyclo ./User/delay.d ./User/delay.o ./User/delay.su ./User/step_counter.cyclo ./User/step_counter.d ./User/step_counter.o ./User/step_counter.su ./User/sys.cyclo ./User/sys.d ./User/sys.o ./User/sys.su ./User/ui.cyclo ./User/ui.d ./User/ui.o ./User/ui.su

.PHONY: clean-User

