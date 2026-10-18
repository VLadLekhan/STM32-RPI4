/*
 * servo.c
 *
 *  Created on: Oct 11, 2026
 *      Author: admin
 */

#include "servo.h"

extern TIM_HandleTypeDef htim3;

void Servo_init(void){
	TIM3 -> CCR1 = SERVO_CENTER;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void Servo_setPulse(uint16_t us){
	if (us < SERVO_MIN){
		us = SERVO_MIN;
	}

	else if(us > SERVO_MAX){
		us = SERVO_MAX;
	}

	TIM3 -> CCR1 = us;

}


