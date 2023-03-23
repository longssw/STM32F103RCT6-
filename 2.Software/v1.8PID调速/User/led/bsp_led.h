#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

/*!
 * \brief   ����LED�ĵ�ƽ
 */
#define LED_TRUE 0

#define LED_EFFECT_END 0xFFFE
#define LED_EFFECT_AGN 0xFFFF

typedef struct led {
  // ����
  GPIO_TypeDef *GPIOx;  // ������GPIO��(�磺GPIOA)
  uint32_t GPIO;        // IO����(�磺GPIO_Pin_0)

  uint16_t time;
  const uint16_t *pbuf;
} led_t;

struct led led_stiact_create(GPIO_TypeDef *const gpiox, const uint32_t gpio);
void led_setup(led_t *const led, const uint16_t buf[]);
void led_proc(led_t *const led);

#endif /* __BSP_LED_H */
