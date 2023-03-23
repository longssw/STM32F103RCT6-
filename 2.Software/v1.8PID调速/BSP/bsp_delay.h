#ifndef BSP_DELAY_H
#define BSP_DELAY_H

#include "stm32f10x.h"

/*!
 * \brief   改函数我测试不通过,无法实现延时功能
 */
void delay_ms(u32 nms);


void delay_init(void);
void delay_us(u32 nus);
void delay_xms(u32 nms);
#endif

