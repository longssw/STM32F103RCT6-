#ifndef BSP_DELAY_H
#define BSP_DELAY_H

#include "stm32f10x.h"

/*!
 * \brief   �ĺ����Ҳ��Բ�ͨ��,�޷�ʵ����ʱ����
 */
void delay_ms(u32 nms);


void delay_init(void);
void delay_us(u32 nus);
void delay_xms(u32 nms);
#endif

