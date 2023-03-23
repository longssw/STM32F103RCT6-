
/*****************************************************************************
 * copyleft (c) 2023 �� ˼ ά
 *
 * �� �� ��:   led_task.c
 * ��    ��:   LED��˸����
 * ��    ��:   ��˼ά
 * ��    ��:   2023-03-01 23:00:11
 * ��    ��:   v1.0.0
 * ---------------------------------------------------------------------------
 * �ļ��޸���ʷ:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-03-01  |   v1.0.0    |    ssw     |     ���ע��
 * ---------------------------------------------------------------------------
 ******************************************************************************/
/*================================ ͷ �� �� ==================================*/
#include <stdbool.h>/* bool���� */

#include "bsp_led.h"   /* LEDӲ������ */
#include "task_init.h" /* RTOS���� */
#include "task_msg.h"  /* ��Ϣ���нṹ�� */
#define LOG_TAG "LED"
#include <elog.h>/* ��־ */
/*============================== ˽ �� �� �� ================================*/
static void led_wait_msg(void);
static bool led_msg_check(const int id, const int state);
/*=========================== �� �� �� / �� �� �� ============================*/
enum led_state_id {
  DARK = 0, /* �� */
  LIGHT,    /* �� */
  BLINK1,   /* ��˸ */
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
/*=============================== �� �� �� =================================*/
#define LED_ID_MAX (LED_SUM - 1)
#define LED_STATE_MAX (STATE_SUM - 1)
/*============================== ȫ �� �� �� ================================*/
const static u16 dark_buf[] = {0, 10, LED_EFFECT_END}; /* LED��״̬ */
const static u16 light_buf[] = {1, 10, LED_EFFECT_END};
const static u16 blink1_buf[] = {1, 50, 0, 50, 1, 50, 0, 50, LED_EFFECT_END};
const static u16 blink2_buf[] = {1, 10, 0, 10, 1, 10, 0, 210, LED_EFFECT_AGN,
                                 4};
const static u16 blink3_buf[] = {1, 10, 0, 10, LED_EFFECT_AGN, 2};
const static u16 blink4_buf[] = {1, 20, 0, 20, LED_EFFECT_END};
const static u16 *pbuf[STATE_SUM] = {dark_buf,   light_buf,
                                     blink1_buf, blink2_buf,
                                     blink3_buf, blink4_buf}; /* ��˸״̬ */
struct led leds[LED_SUM];                                     /* LED���� */
/*============================== �� �� �� �� ================================*/
void led_task(void *parameter) {
  leds[LED_BULE] = led_stiact_create(GPIOD, GPIO_Pin_2); /* LED ��ʼ�� */
  leds[LED_GREEN] = led_stiact_create(GPIOC, GPIO_Pin_12);
  led_setup(&leds[LED_GREEN], pbuf[BLINK4]); /* LED ״̬��ʼ�� */
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
  BaseType_t xReturn = pdTRUE; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdTRUE */
  struct task_msg_pack msg; /* ����һ��������Ϣ�ı��� */

  /* ���յ���ɾ�� */
  xReturn = xQueuePeek(Task_Queue, /* ��Ϣ���еľ�� */
                       &msg,       /* ���յ���Ϣ���� */
                       0);         /* �ȴ�ʱ�� */
  if (pdTRUE == xReturn) {
    if (msg.msg_pkg_type != PKG_LED) return;
    /* ���ղ�ɾ�� */
    xReturn = xQueueReceive(Task_Queue, &msg, 0);
    log_d("���ν��յ��������� id = %d,state = %d \n\n", msg.info.led_pkg.id,
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
