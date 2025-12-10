/*
 * BMP280.h
 *
 *  Created on: Oct 3, 2020
 *      Author: cbares
 */

#ifndef SRC_BMP280_SIMPLE_H_
#define SRC_BMP280_SIMPLE_H_

extern uint8_t temp_calib[3];
extern uint8_t pres_calib[9];

void BMP280_Init_complete(void);
void BMP280_Read_Data(float *temperature, float *pressure);

int BMP280_check();
int BMP280_init();
BMP280_S32_t BMP280_get_temperature();
int BMP280_get_pressure();
uint8_t* BMP280_Read_Reg(uint8_t reg, uint8_t value);
int BMP280_Write_Reg(uint8_t reg, uint8_t value);


#endif /* SRC_BMP280_H_ */
