#ifndef __BSP_ENCODER_H
#define __BSP_ENCODER_H
/*=============================== ͷ�ļ����� ==================================*/
#include "stm32f10x.h"
/*============================== �궨��/�ض��� ================================*/

// struct encoder
// {
//     speed
// };



void EncoderInit(void);
int encoder_read(u8 TIMX);
#endif /* __BSP_LED_H */
