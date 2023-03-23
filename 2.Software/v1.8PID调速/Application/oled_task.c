#if 0
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_delay.h"
#include "bsp_iic_oled.h"
#include "bsp_iic_mpu6050.h"

#include "bsp_tb6612.h"
#include "bsp_init.h"
extern tb6612_t motor12;

#define LOG_TAG "OELD"
#include <elog.h>

#include "bsp_adc.h"

OELD_TypeDef OLED_096 = {
    .IIC.GPIOx_SCL = GPIOA,
    .IIC.GPIOx_SDA = GPIOA,
    .IIC.GPIO_SCL = GPIO_Pin_5,
    .IIC.GPIO_SDA = GPIO_Pin_4,
    .IIC.delay_us = delay_us,
};

void oled_task(void *parameter)
{
    delay_init();
    oled_init(&OLED_096);
    log_i("OLED init");

    adc_init();
    log_i("ADC init");
    while (1)
    {
#if 0
    OLED_096.OLED_Print(&OLED_096, 0, 0, "AX %f", MPU6050.data.aacx);
    OLED_096.OLED_Print(&OLED_096, 0, 1, "AY %f", MPU6050.data.aacy);
    OLED_096.OLED_Print(&OLED_096, 0, 2, "AZ %f", MPU6050.data.aacz);
#endif
#if 0
    OLED_096.OLED_Print(&OLED_096, 0, 0, "GX %f", MPU6050.data.gyrox);
    OLED_096.OLED_Print(&OLED_096, 0, 1, "GY %f", MPU6050.data.gyroy);
    OLED_096.OLED_Print(&OLED_096, 0, 2, "GZ %f", MPU6050.data.gyroz);
#endif
#if 0
        OLED_096.OLED_Print(&OLED_096, 0, 0, "X %f", MPU6050.data.Pitch);
        OLED_096.OLED_Print(&OLED_096, 0, 1, "Y %f", MPU6050.data.Roll);
        OLED_096.OLED_Print(&OLED_096, 0, 2, "Z %f", MPU6050.data.Yaw);
#endif
#if 0
/* ADC */
        OLED_096.OLED_Print(&OLED_096, 0, 3, "B0 [%d]", AD_Value[0]);
#endif
#if 1
        OLED_096.OLED_Print(&OLED_096, 0, 0, "A1 [%d]", GPIO_ReadInputDataBit(motor12.GPIOx_A1, motor12.GPIO_A1));
        // OLED_096.OLED_Print(&OLED_096, 0, 0, "C12 [%d]", GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12));
        OLED_096.OLED_Print(&OLED_096, 0, 1, "A2 [%d]", GPIO_ReadInputDataBit(motor12.GPIOx_A2, motor12.GPIO_A2));
        OLED_096.OLED_Print(&OLED_096, 0, 2, "B1 [%d]", GPIO_ReadInputDataBit(motor12.GPIOx_B1, motor12.GPIO_B1));
        OLED_096.OLED_Print(&OLED_096, 0, 3, "B2 [%d]", GPIO_ReadInputDataBit(motor12.GPIOx_B2, motor12.GPIO_B2));
#endif
        // GPIO_SetBits(motor12.GPIOx_A1, motor12.GPIO_A1);
        // GPIO_WriteBit(motor12.GPIOx_A1, motor12.GPIO_A1, (BitAction)(1));
        // motor12.ioctl(&motor12,motor12.GPIO_A1, 1);
        vTaskDelay(10);
    }
}
#endif
