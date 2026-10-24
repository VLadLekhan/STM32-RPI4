/*
 * tasks.c
 *
 *  Created on: Sep 22, 2026
 *      Author: admin
 */

#include "tasks.h"

void StartTask_ReadSensor(void *argument)
{
  /* USER CODE BEGIN StartTask_ReadSensor */
	int16_t ax, ay, az;
	AccelData_t sample;
  /* Infinite loop */
  for(;;)
  {
	if (MPU6500_ReadAccel(&ax, &ay, &az) == MPU6500_OK){
		sample.accel_x = ax;
		sample.accel_y = ay;
		sample.accel_z = az;

		osMessageQueuePut(sensorDataQueuHandle, &sample, 0, 0);
	}

    osDelay(40);
  }
  /* USER CODE END StartTask_ReadSensor */
}

void StartTask_SendMAVLINK(void *argument)
{
  /* USER CODE BEGIN StartTask_SendMAVLINK */
	AccelData_t received;
	mavlink_message_t msg;
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  /* Infinite loop */
  for(;;)
  {
	  osMessageQueueGet(sensorDataQueuHandle, &received, 0, osWaitForever);

	  mavlink_msg_raw_imu_pack(1, 1, &msg, 0, received.accel_x, received.accel_y, received.accel_z, 0, 0, 0, 0, 0, 0, 0, 0);

	  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

	  osMutexAcquire(uartMutexHandle, osWaitForever);

	  for(uint8_t i = 0; i < len; i++){
		  while(!(USART1 -> ISR & USART_ISR_TXE));
		  USART1 -> TDR = buf[i];
	  }

	  osMutexRelease(uartMutexHandle);

    osDelay(10);
  }
  /* USER CODE END StartTask_SendMAVLINK */
}

void StartTaskHeartbeat(void *argument)
{
  /* USER CODE BEGIN StartTaskHeartbeat */
	mavlink_message_t msg;
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  /* Infinite loop */
  for(;;)
  {
	 mavlink_msg_heartbeat_pack(1, 1, &msg, MAV_TYPE_GENERIC, MAV_AUTOPILOT_GENERIC, 0, 0, MAV_STATE_ACTIVE);
	 uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

	 osMutexAcquire(uartMutexHandle, osWaitForever);

	 for (uint8_t i = 0; i < len; i++){
	 while(!(USART1 -> ISR & USART_ISR_TXE));
	 USART1 -> TDR = buf[i];
	 }

	 osMutexRelease(uartMutexHandle);

    osDelay(1000);
  }
  /* USER CODE END StartTaskHeartbeat */
}

void StartTask_Servo(void *argument)
{
  /* USER CODE BEGIN StartTask_Servo */
  /* Infinite loop */
  for(;;)
  {
	  uint16_t us = SERVO_MIN + (adc_raw * (SERVO_MAX - SERVO_MIN)) / ADC_MAX;
	  Servo_setPulse(us);
	  osDelay(20);
  }
  /* USER CODE END StartTask_Servo */
}

