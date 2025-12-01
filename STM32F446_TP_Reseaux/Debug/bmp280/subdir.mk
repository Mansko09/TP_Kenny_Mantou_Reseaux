################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bmp280/BMP280_simple.c 

OBJS += \
./bmp280/BMP280_simple.o 

C_DEPS += \
./bmp280/BMP280_simple.d 


# Each subdirectory must supply rules for building sources it contributes
bmp280/%.o bmp280/%.su bmp280/%.cyclo: ../bmp280/%.c bmp280/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/mbeng/Documents/ENSEA_3e_ESE/TP_Bus_et_reseaux/TP_Kenny_Mantou_Reseaux/STM32F446_TP_Reseaux/shell" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/mbeng/Documents/ENSEA_3e_ESE/TP_Bus_et_reseaux/TP_Kenny_Mantou_Reseaux/STM32F446_TP_Reseaux/bmp280" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-bmp280

clean-bmp280:
	-$(RM) ./bmp280/BMP280_simple.cyclo ./bmp280/BMP280_simple.d ./bmp280/BMP280_simple.o ./bmp280/BMP280_simple.su

.PHONY: clean-bmp280

