/*****************************************************************************
 * copyleft (c) 2023 �� ˼ ά
 *
 * �� �� ��:   bsp_led.c
 * ��    ��:   LED����
 * ��    ��:   �� ˼ ά
 * ��    ��:   2023-02-27 23:55:27
 * ��    ��:   v1.0.0
 * --------------------------------------------------------------------------
 * �ļ��޸���ʷ:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-02-28  |   v1.0.0    |    ssw     |     ����ע��
 * --------------------------------------------------------------------------
 *****************************************************************************/
/*=============================== ͷ �� �� =================================*/
#include "bsp_led.h"

#include <stdio.h>/* NULL */
/*============================== ˽ �� �� �� ================================*/
static void ioctl(led_t *led, uint8_t logic);
static void gpio_init(led_t *led);
/*=========================== �� �� �� / �� �� �� ============================*/
/*=============================== �� �� �� =================================*/
/*============================== ȫ �� �� �� ================================*/
/*============================== �� �� �� �� ================================*/
void led_setup(led_t *const led, const uint16_t buf[]) {
  led->pbuf = buf;
  led->time = *(led->pbuf + 1);
}

void led_proc(led_t *const led) {
  if (NULL == led->pbuf) return;
  if (led->time) {
    led->time--;
  } else {
    if (*(led->pbuf + 2) == LED_EFFECT_END) {
    } else {
      led->pbuf += 2;
      if (*led->pbuf == LED_EFFECT_AGN) {
        led->pbuf = led->pbuf - (*(led->pbuf + 1) * 2);
      }
      led->time = *(led->pbuf + 1);
    }
  }
  ioctl(led, *led->pbuf);
}

struct led led_stiact_create(GPIO_TypeDef *gpiox, const uint32_t gpio) {
  struct led this;

  this.GPIOx = gpiox;
  this.GPIO = gpio;
  this.pbuf = NULL;
  this.time = 0;

  gpio_init(&this);
  return this;
}

static void gpio_init(led_t *const led) {
  GPIO_InitTypeDef GPIO_Initure;

  if (led->GPIOx == GPIOA) { /* ����GPIO���ʼ��GPIOʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); /* ʹ��GPIOAʱ�� */
  }
  if (led->GPIOx == GPIOB) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); /* ʹ��GPIOBʱ�� */
  }
  if (led->GPIOx == GPIOC) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); /* ʹ��GPIOCʱ�� */
  }
  if (led->GPIOx == GPIOD) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); /* ʹ��GPIODʱ�� */
  }
  if (led->GPIOx == GPIOE) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); /* ʹ��GPIOEʱ�� */
  }

  GPIO_Initure.GPIO_Pin = led->GPIO;
  GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP; /* ͨ��������� */
  GPIO_Init(led->GPIOx, &GPIO_Initure);

  ioctl(led, 0); /* ��LED */
}

static void ioctl(led_t *const led, const uint8_t logic) {
#if LED_TRUE
  if (logic)
#else
  if (!logic)
#endif
  {
    GPIO_SetBits(led->GPIOx, led->GPIO); /* �ߵ�ƽ */
  } else {
    GPIO_ResetBits(led->GPIOx, led->GPIO); /* �͵�ƽ */
  }
}
