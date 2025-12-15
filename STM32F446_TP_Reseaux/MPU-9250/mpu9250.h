/*
 * mpu9250.h
 *
 *  Created on: Dec 15, 2025
 *      Author: knn64
 */

#ifndef MPU9250_H_
#define MPU9250_H_

#ifndef MPU9250_H
#define MPU9250_H

#include "stm32f4xx_hal.h"  // Adapter selon votre série STM32 (f4, f1, etc.)
#include <stdint.h>
#include <stdbool.h>

// Adresses I2C (7 bits)
#define MPU9250_ADDR_AD0_LOW    0x68
#define MPU9250_ADDR_AD0_HIGH   0x69
#define AK8963_ADDR             0x0C

// Registres importants MPU9250
#define MPU9250_WHO_AM_I        0x75
#define MPU9250_PWR_MGMT_1      0x6B
#define MPU9250_PWR_MGMT_2      0x6C
#define MPU9250_INT_PIN_CFG     0x37  // Pour bypass magnétomètre
#define MPU9250_USER_CTRL       0x6A
#define MPU9250_ACCEL_XOUT_H    0x3B
#define MPU9250_GYRO_XOUT_H     0x43
#define MPU9250_TEMP_OUT_H      0x41
#define MPU9250_EXT_SENS_DATA_00 0x49

#define MPU9250_ACCEL_CONFIG    0x1C
#define MPU9250_GYRO_CONFIG     0x1B
#define MPU9250_CONFIG          0x1A  // DLPF

// Registres AK8963 (magnétomètre)
#define AK8963_WHO_AM_I         0x00  // Doit retourner 0x48
#define AK8963_ST1              0x02
#define AK8963_HXL              0x03
#define AK8963_CNTL1            0x0A
#define AK8963_ASAX             0x10  // Adjustments fuse ROM

// Structure avec valeurs converties en unités physiques
typedef struct {
    float accel_x;  // en g
    float accel_y;  // en g
    float accel_z;  // en g

    float gyro_x;   // en degrés/seconde
    float gyro_y;   // en degrés/seconde
    float gyro_z;   // en degrés/seconde
} mpu9250_data_t;
// Handle I2C externe (généré par CubeMX)
extern I2C_HandleTypeDef hi2c1;

// Fonctions
bool MPU9250_Init(void);  // true si succès
void MPU9250_ReadAll(mpu9250_data_t *data);
void MPU9250_PrintData(const mpu9250_data_t *data);
uint8_t MPU9250_WhoAmI(void);

#endif // MPU9250_H


#endif /* MPU9250_H_ */
