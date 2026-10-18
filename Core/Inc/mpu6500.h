/*
 * mpu6500.h
 *
 *  Created on: Sep 16, 2026
 *      Author: admin
 */

#ifndef INC_MPU6500_H_
#define INC_MPU6500_H_

#include "main.h"

#define MPU6500_ADDR 0x68
#define MPU6500_REG_WHO_AM_I 0x75
#define MPU6500_REG_PWR_MGMT_1 0x6B
#define MPU6500_REG_ACCEL_XOUT_H 0x3B
#define MPU6500_WHO_AM_I_VALUE 0x70

typedef enum {
	MPU6500_OK = 0,
	MPU6500_ERR_WHO_AM_I,
	MPU6500_ERR_COMM
}MPU6500_Status_t;

typedef struct {
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
}AccelData_t;

MPU6500_Status_t MPU6500_Init (void);
MPU6500_Status_t MPU6500_ReadAccel(int16_t *x,int16_t *y,int16_t *z);

#endif /* INC_MPU6500_H_ */
