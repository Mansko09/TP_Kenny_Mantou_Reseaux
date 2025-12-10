/*
 * BMP280.c
 *
 *  Created on: Oct 3, 2020
 *      Author: cbares
 */

#include "stdio.h"
#include "stdlib.h"

#include "main.h"
#include "BMP280_simple.h"

extern I2C_HandleTypeDef hi2c1;

// Adresses des registres importants
#define BMP280_REG_CALIB  0x88
#define BMP280_REG_ID     0xD0
#define BMP280_REG_CTRL   0xF4
#define BMP280_REG_CONFIG 0xF5
#define BMP280_REG_DATA   0xF7

// Structure pour stocker les données de calibration (fixe pour chaque capteur)
typedef struct {
	uint16_t dig_T1;
	int16_t  dig_T2;
	int16_t  dig_T3;
	uint16_t dig_P1;
	int16_t  dig_P2;
	int16_t  dig_P3;
	int16_t  dig_P4;
	int16_t  dig_P5;
	int16_t  dig_P6;
	int16_t  dig_P7;
	int16_t  dig_P8;
	int16_t  dig_P9;
} BMP280_CalibData;

BMP280_CalibData bmp_calib;

int32_t t_fine; // Variable globale nécessaire pour la compensation de pression

void BMP280_Init_complete(void) {
    // 1. Lire les 24 octets de calibration à partir du registre 0x88
    uint8_t* cal_data = BMP280_Read_Reg(BMP280_REG_CALIB, 24);

    if (cal_data != NULL) {
        // Reconstruction des variables (LSB d'abord, puis MSB)
        bmp_calib.dig_T1 = (cal_data[1] << 8) | cal_data[0];
        bmp_calib.dig_T2 = (cal_data[3] << 8) | cal_data[2];
        bmp_calib.dig_T3 = (cal_data[5] << 8) | cal_data[4];
        bmp_calib.dig_P1 = (cal_data[7] << 8) | cal_data[6];
        bmp_calib.dig_P2 = (cal_data[9] << 8) | cal_data[8];
        bmp_calib.dig_P3 = (cal_data[11] << 8) | cal_data[10];
        bmp_calib.dig_P4 = (cal_data[13] << 8) | cal_data[12];
        bmp_calib.dig_P5 = (cal_data[15] << 8) | cal_data[14];
        bmp_calib.dig_P6 = (cal_data[17] << 8) | cal_data[16];
        bmp_calib.dig_P7 = (cal_data[19] << 8) | cal_data[18];
        bmp_calib.dig_P8 = (cal_data[21] << 8) | cal_data[20];
        bmp_calib.dig_P9 = (cal_data[23] << 8) | cal_data[22];

        // CRUCIAL : Libérer la mémoire allouée dans ta fonction Read_Reg
        free(cal_data);
    } else {
        printf("Erreur: Impossible de lire la calibration\r\n");
        return;
    }

    // 2. Configuration du capteur (Registre 0xF4 - ctrl_meas)
    // Configuration : Temp x2, Pres x16, Mode Normal
    // Valeur binaire : 010 101 11 -> 0x57
    // osrs_t (2) | osrs_p (5) | mode (3)
    uint8_t config_val = 0x57;

    if (BMP280_Write_Reg(BMP280_REG_CTRL, config_val) != 0) {
        printf("Erreur configuration BMP280\r\n");
    }

    // 3. Configuration filtre/standby (Registre 0xF5 - config)
    // Standby 1000ms, Filter x16
    // Valeur : 0x10 (t_sb=101, filter=100) -> 0xA0 ? Non, exemple simple 0x00 pour défaut
    // On laisse par défaut ou on écrit si besoin.
}

// Lit la Température (en °C) et la Pression (en hPa)
void BMP280_Read_Data(float *temperature, float *pressure) {
    // Lire 6 octets de données à partir de 0xF7 (Press MSB, LSB, XLSB, Temp MSB, LSB, XLSB)
    uint8_t* raw_data = BMP280_Read_Reg(BMP280_REG_DATA, 6);

    if (raw_data == NULL) return; // Sécurité

    // Recombiner les données brutes (ADC)
    int32_t adc_P = (raw_data[0] << 12) | (raw_data[1] << 4) | (raw_data[2] >> 4);
    int32_t adc_T = (raw_data[3] << 12) | (raw_data[4] << 4) | (raw_data[5] >> 4);

    // On libère la mémoire tout de suite
    free(raw_data);

    // --- Algorithme de compensation (Source: Datasheet Bosch) ---

    // 1. Calcul de la Température
    int32_t var1, var2;
    var1 = ((((adc_T >> 3) - ((int32_t)bmp_calib.dig_T1 << 1))) * ((int32_t)bmp_calib.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)bmp_calib.dig_T1)) * ((adc_T >> 4) - ((int32_t)bmp_calib.dig_T1))) >> 12) * ((int32_t)bmp_calib.dig_T3)) >> 14;
    t_fine = var1 + var2;
    *temperature = (t_fine * 5 + 128) >> 8;
    *temperature = *temperature / 100.0f; // Conversion en degrés Float

    // 2. Calcul de la Pression
    int64_t p_var1, p_var2, p;
    p_var1 = ((int64_t)t_fine) - 128000;
    p_var2 = p_var1 * p_var1 * (int64_t)bmp_calib.dig_P6;
    p_var2 = p_var2 + ((p_var1 * (int64_t)bmp_calib.dig_P5) << 17);
    p_var2 = p_var2 + (((int64_t)bmp_calib.dig_P4) << 35);
    p_var1 = ((p_var1 * p_var1 * (int64_t)bmp_calib.dig_P3) >> 8) + ((p_var1 * (int64_t)bmp_calib.dig_P2) << 12);
    p_var1 = (((((int64_t)1) << 47) + p_var1)) * ((int64_t)bmp_calib.dig_P1) >> 33;

    if (p_var1 == 0) {
        *pressure = 0; // Éviter la division par zéro
    } else {
        p = 1048576 - adc_P;
        p = (((p << 31) - p_var2) * 3125) / p_var1;
        p_var1 = (((int64_t)bmp_calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
        p_var2 = (((int64_t)bmp_calib.dig_P8) * p) >> 19;
        p = ((p + p_var1 + p_var2) >> 8) + (((int64_t)bmp_calib.dig_P7) << 4);
        *pressure = (float)p / 256.0f; // Pression en Pa
        *pressure = *pressure / 100.0f; // Conversion en hPa
    }
}

int BMP280_Write_Reg(uint8_t reg, uint8_t value) {
	uint8_t buf[3];
	HAL_StatusTypeDef ret;

	buf[0] = reg;
	buf[1] = value;
	ret = HAL_I2C_Master_Transmit(&hi2c1, BMP280_ADDR, buf, 2, HAL_MAX_DELAY);
	if (ret != 0) {
		printf("Problem with I2C Transmit\r\n");
	}

	ret = HAL_I2C_Master_Receive(&hi2c1, BMP280_ADDR, buf, 1, HAL_MAX_DELAY);
	if (ret != 0) {
		printf("Problem with I2C Receive\r\n");
	}

	if (buf[0] == value) {
		return 0;
	} else {
		return 1;
	}
}

uint8_t* BMP280_Read_Reg(uint8_t reg, uint8_t length) {
	uint8_t *buf;
	HAL_StatusTypeDef ret;

	ret = HAL_I2C_Master_Transmit(&hi2c1, BMP280_ADDR, &reg, 1, HAL_MAX_DELAY);
	if (ret != 0) {
		printf("Problem with I2C Transmit\r\n");
	}

	buf = (uint8_t*) malloc(length);
	ret = HAL_I2C_Master_Receive(&hi2c1, BMP280_ADDR, buf, length,
			HAL_MAX_DELAY);
	if (ret != 0) {
		printf("Problem with I2C Receive\r\n");
	}

	return buf;
}
