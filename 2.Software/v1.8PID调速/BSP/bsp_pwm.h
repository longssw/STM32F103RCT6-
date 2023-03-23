#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "stm32f10x.h"

enum pwm_chx { CH1 = 0, CH2, CH3, CH4 };

typedef struct pwm {
  /* 属性 */
  TIM_TypeDef *TIMx; /* 定时器几 */
  u32 arr;           /* 自动重装值 */
  u32 psc;           /* 时钟预分频数 */
  u32 fpwm;          /* pwm频率 */

  /* 方法 */
  void (*setup)(struct pwm *, enum pwm_chx, float); /* 设置对应通道的PWM */
} pwm_t;

void pwm_setup(struct pwm *const this, const enum pwm_chx ch,
               const float duty_cycle);
struct pwm pwm_static_create(TIM_TypeDef *const timx, const u32 fpwm);
#endif /* __BSP_PWM_H */
