################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MPU-9250/mpu9250.c 

OBJS += \
./MPU-9250/mpu9250.o 

C_DEPS += \
./MPU-9250/mpu9250.d 


# Each subdirectory must supply rules for building sources it contributes
MPU-9250/%.o MPU-9250/%.su MPU-9250/%.cyclo: ../MPU-9250/%.c MPU-9250/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/knn64/git/TP_Kenny_Mantou_Reseaux/STM32F446_TP_Reseaux/shell" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/knn64/git/TP_Kenny_Mantou_Reseaux/STM32F446_TP_Reseaux/bmp280" -I"C:/Users/knn64/git/TP_Kenny_Mantou_Reseaux/STM32F446_TP_Reseaux/MPU-9250" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MPU-2d-9250

clean-MPU-2d-9250:
	-$(RM) ./MPU-9250/mpu9250.cyclo ./MPU-9250/mpu9250.d ./MPU-9250/mpu9250.o ./MPU-9250/mpu9250.su

.PHONY: clean-MPU-2d-9250

