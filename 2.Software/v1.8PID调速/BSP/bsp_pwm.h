#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "stm32f10x.h"

enum pwm_chx { CH1 = 0, CH2, CH3, CH4 };

typedef struct pwm {
  /* ���� */
  TIM_TypeDef *TIMx; /* ��ʱ���� */
  u32 arr;           /* �Զ���װֵ */
  u32 psc;           /* ʱ��Ԥ��Ƶ�� */
  u32 fpwm;          /* pwmƵ�� */

  /* ���� */
  void (*setup)(struct pwm *, enum pwm_chx, float); /* ���ö�Ӧͨ����PWM */
} pwm_t;

void pwm_setup(struct pwm *const this, const enum pwm_chx ch,
               const float duty_cycle);
struct pwm pwm_static_create(TIM_TypeDef *const timx, const u32 fpwm);
#endif /* __BSP_PWM_H */
