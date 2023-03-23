#ifndef BSP_ADC_H
#define BSP_ADC_H

#include "stm32f10x.h"

/* ADC采样通道数 */
#define ADC_CHANNEL_SUM 1

void adc_init(void);
extern u16 AD_Value[1];

#endif /* BSP_ADC_H */
