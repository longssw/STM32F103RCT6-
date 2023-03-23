/*****************************************************************************
 * copyleft (c) 2023 善 思 维
 *
 * 文 件 名:   bsp_led.c
 * 功    能:   LED驱动
 * 作    者:   善 思 维
 * 日    期:   2023-02-27 23:55:27
 * 版    本:   v1.0.0
 * --------------------------------------------------------------------------
 * 文件修改历史:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-02-28  |   v1.0.0    |    ssw     |     增加注释
 * --------------------------------------------------------------------------
 *****************************************************************************/
/*=============================== 头 文 件 =================================*/
#include "bsp_led.h"

#include <stdio.h>/* NULL */
/*============================== 私 有 函 数 ================================*/
static void ioctl(led_t *led, uint8_t logic);
static void gpio_init(led_t *led);
/*=========================== 结 构 体 / 联 合 体 ============================*/
/*=============================== 宏 定 义 =================================*/
/*============================== 全 局 变 量 ================================*/
/*============================== 接 口 函 数 ================================*/
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

  if (led->GPIOx == GPIOA) { /* 根据GPIO组初始化GPIO时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); /* 使能GPIOA时钟 */
  }
  if (led->GPIOx == GPIOB) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); /* 使能GPIOB时钟 */
  }
  if (led->GPIOx == GPIOC) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); /* 使能GPIOC时钟 */
  }
  if (led->GPIOx == GPIOD) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); /* 使能GPIOD时钟 */
  }
  if (led->GPIOx == GPIOE) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); /* 使能GPIOE时钟 */
  }

  GPIO_Initure.GPIO_Pin = led->GPIO;
  GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP; /* 通用推挽输出 */
  GPIO_Init(led->GPIOx, &GPIO_Initure);

  ioctl(led, 0); /* 关LED */
}

static void ioctl(led_t *const led, const uint8_t logic) {
#if LED_TRUE
  if (logic)
#else
  if (!logic)
#endif
  {
    GPIO_SetBits(led->GPIOx, led->GPIO); /* 高电平 */
  } else {
    GPIO_ResetBits(led->GPIOx, led->GPIO); /* 低电平 */
  }
}
