#include "FreeRTOS.h"
#include "task.h"
#include "bsp_delay.h"
#include "bsp_iic_mpu6050.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#define LOG_TAG "IMU"
#include <elog.h>

void mpu6050_task(void *parameter)
{
  short gyro[3], accel[3];

  MPU6050.IIC.GPIOx_SCL = GPIOB;
  MPU6050.IIC.GPIOx_SDA = GPIOB;
  MPU6050.IIC.GPIO_SCL = GPIO_Pin_8;
  MPU6050.IIC.GPIO_SDA = GPIO_Pin_9;
  MPU6050.IIC.delay_us = delay_us;
  log_i("MPU6050 init ID = [0x%x]", mpu6050_init(&MPU6050));
  mpu_dmp_init();
  while (1)
  {
    mpu_dmp_get_data(&MPU6050.data.Pitch, &MPU6050.data.Roll, &MPU6050.data.Yaw); // ½Ç¶È
    MPU6050.read_gyro(&MPU6050, &gyro[0], &gyro[1], &gyro[2]);
    MPU6050.read_accel(&MPU6050, &accel[0], &accel[1], &accel[2]);
    MPU6050.data.gyrox = (float)(gyro[0] * 2000 / 32768);
    MPU6050.data.gyroy = (float)(gyro[1] * 2000 / 32768);
    MPU6050.data.gyroz = (float)(gyro[2] * 2000 / 32768);

    MPU6050.data.aacx = (float)(accel[0] * 8 / 32768);
    MPU6050.data.aacy = (float)(accel[1] * 8 / 32768);
    MPU6050.data.aacz = (float)(accel[2] * 8 / 32768);
		
    log_v("angle [%f][%f][%f]", MPU6050.data.Pitch, MPU6050.data.Roll, MPU6050.data.Yaw);
    //log_v("g [%f][%f][%f]", MPU6050.data.gyrox, MPU6050.data.gyroy, MPU6050.data.gyroz);
    //log_v("a [%f][%f][%f]", MPU6050.data.aacx, MPU6050.data.aacy, MPU6050.data.aacz);
    vTaskDelay(100);
  }
}
