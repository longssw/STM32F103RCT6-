#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

/*!
 * \brief   点亮LED的电平
 */
#define LED_TRUE 0

#define LED_EFFECT_END 0xFFFE
#define LED_EFFECT_AGN 0xFFFF

typedef struct led {
  // 属性
  GPIO_TypeDef *GPIOx;  // 所属的GPIO组(如：GPIOA)
  uint32_t GPIO;        // IO引脚(如：GPIO_Pin_0)

  uint16_t time;
  const uint16_t *pbuf;
} led_t;

struct led led_stiact_create(GPIO_TypeDef *const gpiox, const uint32_t gpio);
void led_setup(led_t *const led, const uint16_t buf[]);
void led_proc(led_t *const led);

#endif /* __BSP_LED_H */
