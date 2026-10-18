/*
 * tasks.h
 *
 *  Created on: Sep 22, 2026
 *      Author: admin
 */

#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "cmsis_os.h"
#include "mpu6500.h"
#include "mavlink.h"
#include "servo.h"

extern osMessageQueueId_t sensorDataQueuHandle;
extern osMutexId_t uartMutexHandle;
extern volatile uint16_t adc_raw;

void StartTask_ReadSensor(void *argument);
void StartTask_SendMAVLINK(void *argument);
void StartTaskHeartbeat(void *argument);
void StartTask_Servo(void *argument);


#endif /* INC_TASKS_H_ */
