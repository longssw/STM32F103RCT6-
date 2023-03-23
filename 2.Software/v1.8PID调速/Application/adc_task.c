/*****************************************************************************
 * copyleft (c) 2023 善 思 维
 *
 * 文 件 名:   adc_task.c
 * 功    能:   检测电池电压
 * 作    者:   善思维
 * 日    期:   2023-03-02 23:43:27
 * 版    本:   v1.0.0
 * ---------------------------------------------------------------------------
 * 文件修改历史:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-03-02  |   v1.0.0    |    ssw     |     创建文件
 * ---------------------------------------------------------------------------
 ******************************************************************************/
/*================================ 头 文 件 ==================================*/
#include "bsp_adc.h"   /* 获取ADC值 */
#include "task_init.h" /* RTOS操作 */
#include "task_msg.h"  /* 消息队列结构体 */
#define LOG_TAG "ADC"
#include <elog.h>/* 日志 */
/*============================== 私 有 函 数 ================================*/
static void battery_voltage_print(void);
static void adc_msg_handle(struct task_msg_pack msg);
static void adc_wait_msg(void);
/*=========================== 结 构 体 / 联 合 体 ============================*/
enum adc_cmd { PRINT_OFF = 0, PRINT_ON };
/*=============================== 宏 定 义 =================================*/
/*============================== 全 局 变 量 ================================*/
enum adc_cmd flag = PRINT_OFF;
/*============================== 接 口 函 数 ================================*/
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
  BaseType_t xReturn = pdTRUE; /* 定义一个创建信息返回值，默认为pdTRUE */
  struct task_msg_pack msg;
  xReturn = xQueuePeek(Task_Queue, /* 消息队列的句柄 */
                       &msg,       /* 接收的消息内容 */
                       0);         /* 等待时间 */
  if (pdTRUE == xReturn) {
    log_d("本次接收到的数据类型是 type = %d\n\n", msg.msg_pkg_type);
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
