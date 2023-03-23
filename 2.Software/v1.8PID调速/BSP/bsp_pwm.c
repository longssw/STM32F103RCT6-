
#include "bsp_pwm.h"

#include <math.h>

/*!
 * \brief   标准库TIM8 4通道PWM初始化
 *
 * \param[in] arr 自动重装值
 * \param[in] psc 时钟预分频数
 */
void TIM8_PWM_Init(u16 arr, u16 psc) {
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);  // 使能定时器3时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,
                         ENABLE);  // 使能GPIO外设和AFIO复用功能模块时钟

  GPIO_InitStructure.GPIO_Pin =
      GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;  // TIM_CH2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  // 初始化GPIO

  // 初始化TIM8
  TIM_TimeBaseStructure.TIM_Period =
      arr;  // 设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_TimeBaseStructure.TIM_Prescaler =
      psc;  // 设置用来作为TIMx时钟频率除数的预分频值
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;  // 设置时钟分割:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode =
      TIM_CounterMode_Up;  // TIM向上计数模式
  TIM_TimeBaseInit(
      TIM8,
      &TIM_TimeBaseStructure);  // 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

  // 初始化TIM8 Channel2 PWM模式
  TIM_OCInitStructure.TIM_OCMode =
      TIM_OCMode_PWM2;  // 选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // 比较输出使能
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCPolarity =
      TIM_OCPolarity_Low;  // 输出极性:TIM输出比较极性高
  TIM_OC1Init(TIM8, &TIM_OCInitStructure);  // 根据T指定的参数初始化外设TIM8 OC2

  TIM_OC1PreloadConfig(TIM8,
                       TIM_OCPreload_Enable);  // 使能TIM8在CCR2上的预装载寄存器

  TIM_SetCompare1(TIM8, 0);

  TIM_OCInitStructure.TIM_OCMode =
      TIM_OCMode_PWM2;  // 选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // 比较输出使能
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCPolarity =
      TIM_OCPolarity_Low;  // 输出极性:TIM输出比较极性高
  TIM_OC2Init(TIM8, &TIM_OCInitStructure);  // 根据T指定的参数初始化外设TIM8 OC2

  TIM_OC2PreloadConfig(TIM8,
                       TIM_OCPreload_Enable);  // 使能TIM8在CCR2上的预装载寄存器

  TIM_SetCompare2(TIM8, 0);

  TIM_OCInitStructure.TIM_OCMode =
      TIM_OCMode_PWM2;  // 选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // 比较输出使能
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCPolarity =
      TIM_OCPolarity_Low;  // 输出极性:TIM输出比较极性高
  TIM_OC3Init(TIM8, &TIM_OCInitStructure);  // 根据T指定的参数初始化外设TIM8 OC2

  TIM_OC3PreloadConfig(TIM8,
                       TIM_OCPreload_Enable);  // 使能TIM8在CCR2上的预装载寄存器

  TIM_SetCompare3(TIM8, 0);

  TIM_OCInitStructure.TIM_OCMode =
      TIM_OCMode_PWM2;  // 选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // 比较输出使能
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCPolarity =
      TIM_OCPolarity_Low;  // 输出极性:TIM输出比较极性高
  TIM_OC4Init(TIM8, &TIM_OCInitStructure);  // 根据T指定的参数初始化外设TIM8 OC2

  TIM_OC4PreloadConfig(TIM8,
                       TIM_OCPreload_Enable);  // 使能TIM8在CCR2上的预装载寄存器

  TIM_SetCompare4(TIM8, 0);

  TIM_Cmd(TIM8, ENABLE);  // 不使能TIM8

  TIM_CtrlPWMOutputs(TIM8, ENABLE);
}

/*!
 * \brief   计算最合适的分频和重装值
 *
 * \param[in] fpwm pwm频率
 * \param[out] arr 自动重装值
 * \param[out] psc 时钟预分频数
 */
static void get_arr_psc(u32 *arr, u32 *psc, u32 fpwm) {
  float midFloat, clkFloat;
  long clkInt;
  long midInt;
  clkFloat = 72000000.0f / fpwm;
  if (clkFloat - (long)clkFloat >= 0.5f)
    clkInt = clkFloat + 1;
  else
    clkInt = (long)clkFloat;

  midFloat = sqrt(clkFloat); /* 开方 */
  if (midFloat - (long)midFloat >= 0.5f)
    midInt = (long)midFloat + 1;
  else
    midInt = (long)midFloat;
  /* 找一组最接近的 */
  for (int i = midInt; i >= 1; i--) {
    if (clkInt % i == 0) {
      *psc = i;
      *arr = clkInt / i;
      break;
    }
  }
}

static void setup(pwm_t *pwm, enum pwm_chx ch, float duty_cycle) {
  u16 load_arr;
  load_arr = (u16)(pwm->arr * duty_cycle);
  switch (ch) {
    case CH1:
      TIM_SetCompare1(pwm->TIMx, load_arr);
      break;
    case CH2:
      TIM_SetCompare2(pwm->TIMx, load_arr);
      break;
    case CH3:
      TIM_SetCompare3(pwm->TIMx, load_arr);
      break;
    case CH4:
      TIM_SetCompare4(pwm->TIMx, load_arr);
      break;
  }
}

void pwm_init(pwm_t *pwm) {
  /* 解算最优值 */
  get_arr_psc(&pwm->arr, &pwm->psc, pwm->fpwm);

  if (pwm->TIMx == TIM8) {
    TIM8_PWM_Init(pwm->arr - 1, pwm->psc - 1);
  }

  pwm->setup = setup;
}

struct pwm pwm_static_create(TIM_TypeDef *const timx, const u32 fpwm) {
  struct pwm this;
  get_arr_psc(&this.arr, &this.psc, fpwm); /* 解算最优值 */
  if (timx == TIM8) {
    TIM8_PWM_Init(this.arr - 1, this.psc - 1);
  }
  this.TIMx = timx;
  this.fpwm = fpwm;
  this.setup = setup;

  return this;
}

void pwm_setup(struct pwm *const this, const enum pwm_chx ch,
               const float duty_cycle) {
  u16 load_arr;
  load_arr = (u16)(this->arr * duty_cycle);
  if (load_arr > this->arr) load_arr = this->arr;
  switch (ch) {
    case CH1:
      TIM_SetCompare1(this->TIMx, load_arr);
      break;
    case CH2:
      TIM_SetCompare2(this->TIMx, load_arr);
      break;
    case CH3:
      TIM_SetCompare3(this->TIMx, load_arr);
      break;
    case CH4:
      TIM_SetCompare4(this->TIMx, load_arr);
      break;
  }
}
