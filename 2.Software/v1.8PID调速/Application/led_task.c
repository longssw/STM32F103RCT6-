
/*****************************************************************************
 * copyleft (c) 2023 善 思 维
 *
 * 文 件 名:   led_task.c
 * 功    能:   LED闪烁控制
 * 作    者:   善思维
 * 日    期:   2023-03-01 23:00:11
 * 版    本:   v1.0.0
 * ---------------------------------------------------------------------------
 * 文件修改历史:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-03-01  |   v1.0.0    |    ssw     |     添加注释
 * ---------------------------------------------------------------------------
 ******************************************************************************/
/*================================ 头 文 件 ==================================*/
#include <stdbool.h>/* bool类型 */

#include "bsp_led.h"   /* LED硬件控制 */
#include "task_init.h" /* RTOS操作 */
#include "task_msg.h"  /* 消息队列结构体 */
#define LOG_TAG "LED"
#include <elog.h>/* 日志 */
/*============================== 私 有 函 数 ================================*/
static void led_wait_msg(void);
static bool led_msg_check(const int id, const int state);
/*=========================== 结 构 体 / 联 合 体 ============================*/
enum led_state_id {
  DARK = 0, /* 灭 */
  LIGHT,    /* 亮 */
  BLINK1,   /* 闪烁 */
  BLINK2,
  BLINK3,
  BLINK4,
  STATE_SUM
};
enum led_id {
  LED_BULE = 0,
  LED_GREEN,
  LED_SUM,
};
/*=============================== 宏 定 义 =================================*/
#define LED_ID_MAX (LED_SUM - 1)
#define LED_STATE_MAX (STATE_SUM - 1)
/*============================== 全 局 变 量 ================================*/
const static u16 dark_buf[] = {0, 10, LED_EFFECT_END}; /* LED灯状态 */
const static u16 light_buf[] = {1, 10, LED_EFFECT_END};
const static u16 blink1_buf[] = {1, 50, 0, 50, 1, 50, 0, 50, LED_EFFECT_END};
const static u16 blink2_buf[] = {1, 10, 0, 10, 1, 10, 0, 210, LED_EFFECT_AGN,
                                 4};
const static u16 blink3_buf[] = {1, 10, 0, 10, LED_EFFECT_AGN, 2};
const static u16 blink4_buf[] = {1, 20, 0, 20, LED_EFFECT_END};
const static u16 *pbuf[STATE_SUM] = {dark_buf,   light_buf,
                                     blink1_buf, blink2_buf,
                                     blink3_buf, blink4_buf}; /* 闪烁状态 */
struct led leds[LED_SUM];                                     /* LED对象 */
/*============================== 接 口 函 数 ================================*/
void led_task(void *parameter) {
  leds[LED_BULE] = led_stiact_create(GPIOD, GPIO_Pin_2); /* LED 初始化 */
  leds[LED_GREEN] = led_stiact_create(GPIOC, GPIO_Pin_12);
  led_setup(&leds[LED_GREEN], pbuf[BLINK4]); /* LED 状态初始化 */
  led_setup(&leds[LED_BULE], pbuf[BLINK4]);

  log_i("Led1 init!\r\n");
  while (1) {
    led_wait_msg();
    for (u8 i = 0; i < LED_SUM; i++) {
      led_proc(&leds[i]);
    }
    vTaskDelay(10);
  }
}

static void led_wait_msg(void) {
  BaseType_t xReturn = pdTRUE; /* 定义一个创建信息返回值，默认为pdTRUE */
  struct task_msg_pack msg; /* 定义一个接收消息的变量 */

  /* 接收但不删除 */
  xReturn = xQueuePeek(Task_Queue, /* 消息队列的句柄 */
                       &msg,       /* 接收的消息内容 */
                       0);         /* 等待时间 */
  if (pdTRUE == xReturn) {
    if (msg.msg_pkg_type != PKG_LED) return;
    /* 接收并删除 */
    xReturn = xQueueReceive(Task_Queue, &msg, 0);
    log_d("本次接收到的数据是 id = %d,state = %d \n\n", msg.info.led_pkg.id,
          msg.info.led_pkg.state);
    if (led_msg_check(msg.info.led_pkg.id, msg.info.led_pkg.state) == true) {
      led_setup(&leds[msg.info.led_pkg.id], pbuf[msg.info.led_pkg.state]);
    }
  }
}

static bool led_msg_check(const int id, const int state) {
  if (id > LED_ID_MAX || id < 0) {
    log_i("The id value in 0 ~ %d", LED_ID_MAX);
    return false;
  }
  if (state > LED_STATE_MAX || id < 0) {
    log_i("The state value in 0 ~ %d", LED_STATE_MAX);
    return false;
  }
  return true;
}
