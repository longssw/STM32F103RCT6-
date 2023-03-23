
#include "bsp_pwm.h"

#include <math.h>

/*!
 * \brief   ��׼��TIM8 4ͨ��PWM��ʼ��
 *
 * \param[in] arr �Զ���װֵ
 * \param[in] psc ʱ��Ԥ��Ƶ��
 */
void TIM8_PWM_Init(u16 arr, u16 psc) {
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);  // ʹ�ܶ�ʱ��3ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,
                         ENABLE);  // ʹ��GPIO�����AFIO���ù���ģ��ʱ��

  GPIO_InitStructure.GPIO_Pin =
      GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;  // TIM_CH2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  // ��ʼ��GPIO

  // ��ʼ��TIM8
  TIM_TimeBaseStructure.TIM_Period =
      arr;  // ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure.TIM_Prescaler =
      psc;  // ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;  // ����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode =
      TIM_CounterMode_Up;  // TIM���ϼ���ģʽ
  TIM_TimeBaseInit(
      TIM8,
      &TIM_TimeBaseStructure);  // ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

  // ��ʼ��TIM8 Channel2 PWMģʽ
  TIM_OCInitStructure.TIM_OCMode =
      TIM_OCMode_PWM2;  // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // �Ƚ����ʹ��
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCPolarity =
      TIM_OCPolarity_Low;  // �������:TIM����Ƚϼ��Ը�
  TIM_OC1Init(TIM8, &TIM_OCInitStructure);  // ����Tָ���Ĳ�����ʼ������TIM8 OC2

  TIM_OC1PreloadConfig(TIM8,
                       TIM_OCPreload_Enable);  // ʹ��TIM8��CCR2�ϵ�Ԥװ�ؼĴ���

  TIM_SetCompare1(TIM8, 0);

  TIM_OCInitStructure.TIM_OCMode =
      TIM_OCMode_PWM2;  // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // �Ƚ����ʹ��
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCPolarity =
      TIM_OCPolarity_Low;  // �������:TIM����Ƚϼ��Ը�
  TIM_OC2Init(TIM8, &TIM_OCInitStructure);  // ����Tָ���Ĳ�����ʼ������TIM8 OC2

  TIM_OC2PreloadConfig(TIM8,
                       TIM_OCPreload_Enable);  // ʹ��TIM8��CCR2�ϵ�Ԥװ�ؼĴ���

  TIM_SetCompare2(TIM8, 0);

  TIM_OCInitStructure.TIM_OCMode =
      TIM_OCMode_PWM2;  // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // �Ƚ����ʹ��
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCPolarity =
      TIM_OCPolarity_Low;  // �������:TIM����Ƚϼ��Ը�
  TIM_OC3Init(TIM8, &TIM_OCInitStructure);  // ����Tָ���Ĳ�����ʼ������TIM8 OC2

  TIM_OC3PreloadConfig(TIM8,
                       TIM_OCPreload_Enable);  // ʹ��TIM8��CCR2�ϵ�Ԥװ�ؼĴ���

  TIM_SetCompare3(TIM8, 0);

  TIM_OCInitStructure.TIM_OCMode =
      TIM_OCMode_PWM2;  // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // �Ƚ����ʹ��
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCPolarity =
      TIM_OCPolarity_Low;  // �������:TIM����Ƚϼ��Ը�
  TIM_OC4Init(TIM8, &TIM_OCInitStructure);  // ����Tָ���Ĳ�����ʼ������TIM8 OC2

  TIM_OC4PreloadConfig(TIM8,
                       TIM_OCPreload_Enable);  // ʹ��TIM8��CCR2�ϵ�Ԥװ�ؼĴ���

  TIM_SetCompare4(TIM8, 0);

  TIM_Cmd(TIM8, ENABLE);  // ��ʹ��TIM8

  TIM_CtrlPWMOutputs(TIM8, ENABLE);
}

/*!
 * \brief   ��������ʵķ�Ƶ����װֵ
 *
 * \param[in] fpwm pwmƵ��
 * \param[out] arr �Զ���װֵ
 * \param[out] psc ʱ��Ԥ��Ƶ��
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

  midFloat = sqrt(clkFloat); /* ���� */
  if (midFloat - (long)midFloat >= 0.5f)
    midInt = (long)midFloat + 1;
  else
    midInt = (long)midFloat;
  /* ��һ����ӽ��� */
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
  /* ��������ֵ */
  get_arr_psc(&pwm->arr, &pwm->psc, pwm->fpwm);

  if (pwm->TIMx == TIM8) {
    TIM8_PWM_Init(pwm->arr - 1, pwm->psc - 1);
  }

  pwm->setup = setup;
}

struct pwm pwm_static_create(TIM_TypeDef *const timx, const u32 fpwm) {
  struct pwm this;
  get_arr_psc(&this.arr, &this.psc, fpwm); /* ��������ֵ */
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
