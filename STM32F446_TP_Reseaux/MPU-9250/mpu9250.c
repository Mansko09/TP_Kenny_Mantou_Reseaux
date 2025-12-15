#include "mpu9250.h"
#include "stdio.h"

// Facteurs de conversion pour les échelles choisies
#define ACCEL_SENSITIVITY   4096.0f   // LSB/g pour ±8g  (16384 / 4 = 4096)
#define GYRO_SENSITIVITY    32.8f     // LSB/(°/s) pour ±1000°/s  (131 / 1 = 131, mais pour ±1000 c'est 131 * 1000/250 = 32.8)
#define MPU9250_ADDR 0x68
#define MPU9250_I2C_ADDR    (MPU9250_ADDR << 1)  // Adresse 8 bits pour HAL

static HAL_StatusTypeDef write_reg(uint8_t reg, uint8_t val) {
    return HAL_I2C_Mem_Write(&hi2c1, MPU9250_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
}

static HAL_StatusTypeDef read_regs(uint8_t reg, uint8_t *buf, uint16_t len) {
    return HAL_I2C_Mem_Read(&hi2c1, MPU9250_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 100);
}

bool MPU9250_Init(void) {
    // Vérification de l'identité
    uint8_t who = MPU9250_WhoAmI();
    if (who != 0x71) {
        return false;  // Pas un MPU9250
    }

    // Sortie du mode sleep
    if (write_reg(MPU9250_PWR_MGMT_1, 0x00) != HAL_OK) {
        return false;
    }
    HAL_Delay(100);

    // ±8g pour accéléromètre
    write_reg(MPU9250_ACCEL_CONFIG, 0x10);  // AFS_SEL = 2 → ±8g

    // ±1000 °/s pour gyroscope
    write_reg(MPU9250_GYRO_CONFIG, 0x10);   // FS_SEL = 2 → ±1000 °/s

    // Filtre numérique (DLPF) : bande passante ~44 Hz acc / ~42 Hz gyro
    write_reg(MPU9250_CONFIG, 0x03);

    return true;
}

uint8_t MPU9250_WhoAmI(void) {
    uint8_t val = 0;
    read_regs(MPU9250_WHO_AM_I, &val, 1);
    return val;
}

void MPU9250_ReadAll(mpu9250_data_t *data) {
    uint8_t buf[12];  // 6 bytes acc + 6 bytes gyro

    // Lecture accéléromètre (0x3B à 0x40) + gyroscope (0x43 à 0x48)
    // On lit 12 bytes à partir de ACCEL_XOUT_H pour avoir les deux
    if (read_regs(MPU9250_ACCEL_XOUT_H, buf, 12) == HAL_OK) {
        int16_t raw_ax = (int16_t)(buf[0]  << 8 | buf[1]);
        int16_t raw_ay = (int16_t)(buf[2]  << 8 | buf[3]);
        int16_t raw_az = (int16_t)(buf[4]  << 8 | buf[5]);

        int16_t raw_gx = (int16_t)(buf[8]  << 8 | buf[9]);
        int16_t raw_gy = (int16_t)(buf[10] << 8 | buf[11]);
        int16_t raw_gz = (int16_t)(buf[12] << 8 | buf[13]);  // Attention : buf[12] n'existe pas → correction ci-dessous

        // Correction : on lit bien 14 bytes pour sauter la température
        uint8_t full_buf[14];
        if (read_regs(MPU9250_ACCEL_XOUT_H, full_buf, 14) == HAL_OK) {
            raw_ax = (int16_t)(full_buf[0]  << 8 | full_buf[1]);
            raw_ay = (int16_t)(full_buf[2]  << 8 | full_buf[3]);
            raw_az = (int16_t)(full_buf[4]  << 8 | full_buf[5]);

            raw_gx = (int16_t)(full_buf[8]  << 8 | full_buf[9]);
            raw_gy = (int16_t)(full_buf[10] << 8 | full_buf[11]);
            raw_gz = (int16_t)(full_buf[12] << 8 | full_buf[13]);

            // Conversion en unités physiques
            data->accel_x = raw_ax / ACCEL_SENSITIVITY;
            data->accel_y = raw_ay / ACCEL_SENSITIVITY;
            data->accel_z = raw_az / ACCEL_SENSITIVITY;

            data->gyro_x  = raw_gx / GYRO_SENSITIVITY;
            data->gyro_y  = raw_gy / GYRO_SENSITIVITY;
            data->gyro_z  = raw_gz / GYRO_SENSITIVITY;
        }
    }
}

void MPU9250_PrintData(const mpu9250_data_t *data)
{
    printf("Accéléromètre (g):\r\n");
    printf("  X: %+.3f\r\n", data->accel_x);
    printf("  Y: %+.3f\r\n", data->accel_y);
    printf("  Z: %+.3f\r\n", data->accel_z);

    printf("Gyroscope (°/s):\r\n");
    printf("  X: %+.3f\r\n", data->gyro_x);
    printf("  Y: %+.3f\r\n", data->gyro_y);
    printf("  Z: %+.3f\r\n", data->gyro_z);

    printf("------------------------\r\n");
}
