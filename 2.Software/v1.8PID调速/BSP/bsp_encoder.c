/*****************************************************************************
 * copyleft (c) 2023 �� ˼ ά
 *
 * �� �� ��:   bsp_encoder.c
 * ��    ��:
 * ��    ��:   ��˼ά
 * ��    ��:   2023-03-03 19:09:41
 * ��    ��:   v1.0.0
 * ---------------------------------------------------------------------------
 * �ļ��޸���ʷ:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-03-03  |   v1.0.0    |    ssw     |     �����ļ�
 * ---------------------------------------------------------------------------
 * ��    ע:   �ǳ���лgitee�������ṩ��stm32������ģʽԴ��
 * https://gitee.com/royalboyer/ServoMotorDriver-STM32/blob/master/HARDWARE/ENC/enc.c
 ******************************************************************************/
/*================================ ͷ �� �� ==================================*/
#include "bsp_encoder.h" /* "stm32f10x.h" */
/*============================== ˽ �� �� �� ================================*/
static void Encoder_TIM2_Init(void);
static void Encoder_TIM4_Init(void);
/*=========================== �� �� �� / �� �� �� ============================*/
/*=============================== �� �� �� =================================*/
/*============================== ȫ �� �� �� ================================*/
/*============================== �� �� �� �� ================================*/

void Encoder_TIM2_Init(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_ICInitTypeDef TIM_ICInitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(
      RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,
      ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,
                      ENABLE);  // enable SWD not JTAG
  GPIO_PinRemapConfig(
      GPIO_FullRemap_TIM2,
      ENABLE);  // GPIO_PartialRemap1_TIM2/GPIO_PartialRemap2_TIM2/GPIO_FullRemap_TIM2
  GPIO_PinRemapConfig(
      GPIO_PartialRemap1_TIM2,
      ENABLE);  // GPIO_PartialRemap1_TIM2/GPIO_PartialRemap2_TIM2/GPIO_FullRemap_TIM2
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  // GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //??JTAG,SWD?????
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  TIM_DeInit(TIM2);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);
  TIM_TimeBaseStruct.TIM_Period = 65535;
  TIM_TimeBaseStruct.TIM_Prescaler = 0;
  TIM_TimeBaseStruct.TIM_ClockDivision =
      TIM_CKD_DIV1;  // TIM_CKD_DIV1/TIM_CKD_DIV2/TIM_CKD_DIV4
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

  // TIM_EncoderInterfaceConfig(TIM2,
  // TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling,
                             TIM_ICPolarity_Falling);
  TIM_ICStructInit(&TIM_ICInitStruct);
  TIM_ICInitStruct.TIM_ICFilter = 5;
  TIM_ICInit(TIM2, &TIM_ICInitStruct);
  TIM_ClearFlag(TIM2, TIM_IT_Update);
  // TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  TIM_SetCounter(TIM2, 0);
}

static void Encoder_TIM3_Init(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_ICInitTypeDef TIM_ICInitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
  GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  TIM_DeInit(TIM3);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);
  TIM_TimeBaseStruct.TIM_Period = 65535;
  TIM_TimeBaseStruct.TIM_Prescaler = 0;
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);

  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising,
                             TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStruct);
  TIM_ICInitStruct.TIM_ICFilter = 5;
  TIM_ICInit(TIM3, &TIM_ICInitStruct);
  TIM_ClearFlag(TIM3, TIM_IT_Update);
  // TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
  TIM_SetCounter(TIM3, 0);
}

static void Encoder_TIM4_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_ICInitTypeDef TIM_ICInitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = 65535;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising,
                             TIM_ICPolarity_Rising);

  TIM_ICStructInit(&TIM_ICInitStruct);
  TIM_ICInitStruct.TIM_ICFilter = 10;
  TIM_ICInit(TIM4, &TIM_ICInitStruct);

  TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  TIM_SetCounter(TIM4, 0);

  TIM_Cmd(TIM4, ENABLE);
}

static void Encoder_TIM5_Init(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_ICInitTypeDef TIM_ICInitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  TIM_DeInit(TIM5);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);
  TIM_TimeBaseStruct.TIM_Period = 65535;
  TIM_TimeBaseStruct.TIM_Prescaler = 0;
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStruct);

  TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising,
                             TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStruct);
  TIM_ICInitStruct.TIM_ICFilter = 5;
  TIM_ICInit(TIM5, &TIM_ICInitStruct);
  TIM_ClearFlag(TIM5, TIM_IT_Update);
  // TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM5, ENABLE);
  TIM_SetCounter(TIM5, 0);
}
/*============================== �ӿں������� ================================*/

/*
******************************************************************************
�� �� ��:  EncoderInit
����˵��:  ��������ʼ��
******************************************************************************
*/
void EncoderInit(void) {
  Encoder_TIM2_Init();
  Encoder_TIM3_Init();
  Encoder_TIM4_Init();
  Encoder_TIM5_Init();
}

int encoder_read(u8 TIMX) {
  int count;
  switch (TIMX) {
    case 2:
      count = (short)TIM2->CNT;
      TIM2->CNT = 0;
      break;
    case 3:
      count = (short)TIM3->CNT;
      TIM3->CNT = 0;
      break;
    case 4:
      count = (short)TIM4->CNT;
      TIM4->CNT = 0;
      break;
    case 5:
      count = (short)TIM5->CNT;
      TIM5->CNT = 0;
      break;
    default:
      count = 0;
  }
  return count;
}

/*
******************************************************************************
�� �� ��:  TIMx_IRQHandler
����˵��:  ��ʱ��x�жϺ���
��ϸ����˵��:  �����жϺ�����жϱ�־λ
��    �Σ� ��
�� �� ֵ:  ��
******************************************************************************
*/
void TIM2_IRQHandler(void) {
  //   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != 0) {
  //     TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  //   }
  if (TIM2->SR & 0X0001)  // Overflow interrupt //����ж�
  {
  }
  TIM2->SR &= ~(1 << 0);  // Clear the interrupt flag bit //����жϱ�־λ
}

void TIM3_IRQHandler(void) {
  if (TIM3->SR & 0X0001)  // Overflow interrupt //����ж�
  {
  }
  TIM3->SR &= ~(1 << 0);  // Clear the interrupt flag bit //����жϱ�־λ
  //   if (TIM_GetITStatus(TIM3, TIM_IT_Update) != 0) {
  //     TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  //   }
}

void TIM4_IRQHandler(void) {
  if (TIM4->SR & 0X0001)  // Overflow interrupt //����ж�
  {
  }
  TIM4->SR &= ~(1 << 0);  // Clear the interrupt flag bit //����жϱ�־λ
  //   if (TIM_GetITStatus(TIM4, TIM_IT_Update) != 0) {
  //     TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  //   }
}

void TIM5_IRQHandler(void) {
  if (TIM5->SR & 0X0001)  // Overflow interrupt //����ж�
  {
  }
  TIM5->SR &= ~(1 << 0);  // Clear the interrupt flag bit //����жϱ�־λ
  //   if (TIM_GetITStatus(TIM5, TIM_IT_Update) != 0) {
  //     TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
  //   }
}
