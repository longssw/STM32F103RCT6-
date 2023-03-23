#ifndef __BSP_ENCODER_H
#define __BSP_ENCODER_H
/*=============================== 头文件包含 ==================================*/
#include "stm32f10x.h"
/*============================== 宏定义/重定义 ================================*/

// struct encoder
// {
//     speed
// };



void EncoderInit(void);
int encoder_read(u8 TIMX);
#endif /* __BSP_LED_H */
