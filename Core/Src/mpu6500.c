/*
 * mpu6500.c
 *
 *  Created on: Sep 16, 2026
 *      Author: admin
 */


#include "mpu6500.h"

static uint8_t I2C_WaitFlag(uint32_t flag, uint32_t timeout_ms){
	uint32_t start = HAL_GetTick();

	while(!(I2C1 -> ISR & flag)){
		if (I2C1 -> ISR & I2C_ISR_NACKF) {
			I2C1 -> ICR |= I2C_ICR_NACKCF;
			return 1;
		}  else if ((HAL_GetTick() - start) >  timeout_ms) {
			return 2;
		}
	}

	return 0;
}


MPU6500_Status_t MPU6500_Init (void){
	I2C1 -> CR2 = (MPU6500_ADDR << 1) | (1 << I2C_CR2_NBYTES_Pos) | (I2C_CR2_START);

	if (I2C_WaitFlag(I2C_ISR_TXIS, 20) != 0) return MPU6500_ERR_COMM;
	I2C1 -> TXDR = MPU6500_REG_WHO_AM_I;

	if (I2C_WaitFlag(I2C_ISR_TC, 20) != 0) return MPU6500_ERR_COMM;

	I2C1 -> CR2 = (MPU6500_ADDR << 1) | (1 << I2C_CR2_NBYTES_Pos) | (I2C_CR2_RD_WRN) | (I2C_CR2_START) | (I2C_CR2_AUTOEND);

	if (I2C_WaitFlag(I2C_ISR_RXNE, 20) != 0) return MPU6500_ERR_COMM;
	uint8_t id = I2C1 -> RXDR;
	if (I2C_WaitFlag(I2C_ISR_STOPF, 20) != 0) return MPU6500_ERR_COMM;
	I2C1->ICR |= I2C_ICR_STOPCF;


	if (id != MPU6500_WHO_AM_I_VALUE){
		return MPU6500_ERR_WHO_AM_I;
	}

	I2C1 -> CR2 = (MPU6500_ADDR << 1) | (2 << I2C_CR2_NBYTES_Pos) | (I2C_CR2_START) | (I2C_CR2_AUTOEND);

	if (I2C_WaitFlag(I2C_ISR_TXIS, 20) != 0) return MPU6500_ERR_COMM;
	I2C1 -> TXDR = MPU6500_REG_PWR_MGMT_1;

	if (I2C_WaitFlag(I2C_ISR_TXIS, 20) != 0) return MPU6500_ERR_COMM;
	I2C1 -> TXDR = 0x00;

	if (I2C_WaitFlag(I2C_ISR_STOPF, 20) != 0) return MPU6500_ERR_COMM;
    I2C1->ICR |= I2C_ICR_STOPCF;


	return MPU6500_OK;
}

MPU6500_Status_t MPU6500_ReadAccel(int16_t *x,int16_t *y,int16_t *z){
	I2C1 -> CR2 = (MPU6500_ADDR << 1) | (1 << I2C_CR2_NBYTES_Pos) | (I2C_CR2_START);

	if (I2C_WaitFlag(I2C_ISR_TXIS, 20) != 0) return MPU6500_ERR_COMM;
	I2C1 -> TXDR = MPU6500_REG_ACCEL_XOUT_H;

	if (I2C_WaitFlag(I2C_ISR_TC, 20) != 0) return MPU6500_ERR_COMM;

	I2C1 -> CR2 = (MPU6500_ADDR << 1) | (6 << I2C_CR2_NBYTES_Pos) | (I2C_CR2_RD_WRN) | (I2C_CR2_START) | (I2C_CR2_AUTOEND);

	uint8_t raw[6];
	for (uint8_t i = 0; i < sizeof(raw)/sizeof(raw[0]); i++){

		if (I2C_WaitFlag(I2C_ISR_RXNE, 20) != 0) return MPU6500_ERR_COMM;
		raw[i] = I2C1 -> RXDR;

	}

	if (I2C_WaitFlag(I2C_ISR_STOPF, 20) != 0) return MPU6500_ERR_COMM;
	I2C1 -> ICR |= I2C_ICR_STOPCF;

	*x = (int16_t)((raw[0] << 8) | raw[1]);
	*y = (int16_t)((raw[2] << 8) | raw[3]);
	*z = (int16_t)((raw[4] << 8) | raw[5]);

	return MPU6500_OK;
}

