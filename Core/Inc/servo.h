/*
 * servo.h
 *
 *  Created on: Oct 11, 2026
 *      Author: admin
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include "main.h"

#define SERVO_MIN 900
#define SERVO_CENTER 1800
#define SERVO_MAX 2800
#define ADC_MAX 4095

void Servo_init(void);
void Servo_setPulse(uint16_t us);


#endif /* INC_SERVO_H_ */
