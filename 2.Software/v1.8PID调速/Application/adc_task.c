/*****************************************************************************
 * copyleft (c) 2023 �� ˼ ά
 *
 * �� �� ��:   adc_task.c
 * ��    ��:   ����ص�ѹ
 * ��    ��:   ��˼ά
 * ��    ��:   2023-03-02 23:43:27
 * ��    ��:   v1.0.0
 * ---------------------------------------------------------------------------
 * �ļ��޸���ʷ:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-03-02  |   v1.0.0    |    ssw     |     �����ļ�
 * ---------------------------------------------------------------------------
 ******************************************************************************/
/*================================ ͷ �� �� ==================================*/
#include "bsp_adc.h"   /* ��ȡADCֵ */
#include "task_init.h" /* RTOS���� */
#include "task_msg.h"  /* ��Ϣ���нṹ�� */
#define LOG_TAG "ADC"
#include <elog.h>/* ��־ */
/*============================== ˽ �� �� �� ================================*/
static void battery_voltage_print(void);
static void adc_msg_handle(struct task_msg_pack msg);
static void adc_wait_msg(void);
/*=========================== �� �� �� / �� �� �� ============================*/
enum adc_cmd { PRINT_OFF = 0, PRINT_ON };
/*=============================== �� �� �� =================================*/
/*============================== ȫ �� �� �� ================================*/
enum adc_cmd flag = PRINT_OFF;
/*============================== �� �� �� �� ================================*/
void adc_task(void *parameter) {
  adc_init();
  log_i("ADC Init\r\n");
  while (1) {
    adc_wait_msg();
    battery_voltage_print();
    vTaskDelay(100);
  }
}

static void battery_voltage_print(void) {
  if (flag == PRINT_ON)
    log_i("battery voltage = %fV", ((float)AD_Value[0] / 4096 * 3.3 * 11));
}

static void adc_wait_msg(void) {
  BaseType_t xReturn = pdTRUE; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdTRUE */
  struct task_msg_pack msg;
  xReturn = xQueuePeek(Task_Queue, /* ��Ϣ���еľ�� */
                       &msg,       /* ���յ���Ϣ���� */
                       0);         /* �ȴ�ʱ�� */
  if (pdTRUE == xReturn) {
    log_d("���ν��յ������������� type = %d\n\n", msg.msg_pkg_type);
    if (msg.msg_pkg_type != PKG_ADC) return;
    xReturn = xQueueReceive(Task_Queue, &msg, 0);
    adc_msg_handle(msg);
  }
}

static void adc_msg_handle(struct task_msg_pack msg) {
  if (msg.msg_pkg_type != PKG_ADC) return;
  switch (msg.info.adc_pkg.cmd) {
    case PRINT_OFF:
      flag = PRINT_OFF;
      break;
    case PRINT_ON:
      flag = PRINT_ON;
      break;
    default:
      log_d("Accepted data does not conform to format!");
      break;
  }
}
