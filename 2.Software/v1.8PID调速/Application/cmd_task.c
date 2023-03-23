/*****************************************************************************
 * copyleft (c) 2023 �� ˼ ά
 *
 * �� �� ��:   cmd_task.c
 * ��    ��:   ���������е���
 * ��    ��:   ��˼ά
 * ��    ��:   2023-03-01 23:06:45
 * ��    ��:   v1.0.0
 * ---------------------------------------------------------------------------
 * �ļ��޸���ʷ:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-03-01  |   v1.0.0    |    ssw     |     �����ļ�
 * ---------------------------------------------------------------------------
 ******************************************************************************/
/*================================ ͷ �� �� ==================================*/
#include <string.h>/* strstr */

#include "bsp_buffer.h" /* �ж������黺�� */
#include "bsp_cmd.h"    /* �ַ��������� */
#include "bsp_usart.h"  /* �ַ�����ȡ�� */
#include "task_init.h"  /* RTOS���� */
#include "task_msg.h"   /* ��Ϣ���нṹ�� */
#define LOG_TAG "CMD"
#include <elog.h>/* ��־ */
/*============================== ˽ �� �� �� ================================*/
static void cmd_help(char *str);
static void cmd_led(char *str);
static void cmd_motor(char *str);
static void cmd_adc(char *str);
static void usrt1_cbs(u8 ch);
static void msg_pkg_send(const QueueHandle_t target,
                         const struct task_msg_pack *pkg);
/*=========================== �� �� �� / �� �� �� ============================*/
/*=============================== �� �� �� =================================*/
#define SEND_PKG(pkg) msg_pkg_send(Task_Queue, pkg)
/*============================== ȫ �� �� �� ================================*/
static buffer_t *pbuffer;
static cmd_t *pcmd;
static struct cmd_table table[] = {
    {"help", cmd_help},
    {"led", cmd_led},
    {"motor", cmd_motor},
    {"adc", cmd_adc},
};
/*============================== �� �� �� �� ================================*/
void cmd_task(void *parameter) {
  u8 ch;
  pbuffer = buffer_create(512);
  if (NULL == pbuffer) {
    log_e("Buffer create fail");
    /* ��������û�з���ɹ�,��˲���free */
    goto err;
  }
  pcmd = cmd_create(table, sizeof(table) / sizeof(struct cmd_table), 512);
  if (NULL == pcmd) {
    log_e("Cmd create fail");
    buffer_destroy(pbuffer);
    goto err;
  }
  log_i("Cmd init!\r\n");
  usrt_receive_register(USART1, usrt1_cbs);
  vTaskDelay(10);
  cmd_help("NULL");

  while (1) {
    if (buffer_output(pbuffer, &ch) == 0) {
      cmd_proc(pcmd, ch);
    }
    vTaskDelay(10);
  }

err:
  taskENTER_CRITICAL();         /* �����ٽ��� */
  vTaskDelete(Cmd_Task_Handle); /* ɾ��AppTaskCreate���� */
  log_e("cmd_task was deleted!");
  taskEXIT_CRITICAL(); /* �˳��ٽ��� */
}

static void usrt1_cbs(u8 ch) { buffer_input(pbuffer, ch); }

static void cmd_help(char *str) {
  u16 i;

  log_v("You can use the cmd:");
  for (i = 0; i < sizeof(table) / sizeof(struct cmd_table); i++) {
    log_v("[cmd%d]: %s ", i, table[i].name);
  }
}

static void cmd_led(char *str) {
  struct task_msg_pack msg; /* ���͸�led����Ľṹ�� */
  int ret;                  /* ����ֵ */
  msg.msg_pkg_type = PKG_LED;
  if (strstr(str, "set")) {
    ret = sscanf(str, "set id %d state %d", &msg.info.led_pkg.id,
                 &msg.info.led_pkg.state);
    if (!ret) return;
    
    log_i("Set led[%d] state[%d] \r\n", msg.info.led_pkg.id,
          msg.info.led_pkg.state);
    SEND_PKG(&msg);
  } else {
    log_v("Set led state, format: [led set id 1 state 1] \r\n");
  }
}

static void cmd_motor(char *str) {
  struct task_msg_pack msg; /* ���͸�led����Ľṹ�� */
  int ret;                  /* ����ֵ */
  int id;
  float speed;
  msg.msg_pkg_type = PKG_MOTOR;
  if (strstr(str, "set")) {
    ret = sscanf(str, "set id %d speed %f", &id, &speed);
    if (!ret) return;
    msg.info.motor_pkg.id = (u8)id;
    msg.info.motor_pkg.speed = speed;

    log_i("Set motor[%d] speed[%f] \r\n", msg.info.motor_pkg.id,
          msg.info.motor_pkg.speed);

    SEND_PKG(&msg);
  } else {
    log_v("Set motor speed, format: [motor set id 1 speed 0.2]");
    log_v("The parameter is: id[1~4] speed[-1.00 ~ 1.00]\r\n");
  }
}

static void cmd_adc(char *str) {
  struct task_msg_pack msg;
  msg.msg_pkg_type = PKG_ADC;
  if (strstr(str, "show")) {
    msg.info.adc_pkg.cmd = 1;
    SEND_PKG(&msg);
  } else if (strstr(str, "exit")) {
    msg.info.adc_pkg.cmd = 0;
    SEND_PKG(&msg);
  } else {
    log_v("Show battery voltage, format: [adc show/exit]");
  }
}

static void msg_pkg_send(const QueueHandle_t target,
                         const struct task_msg_pack *pkg) {
  BaseType_t xReturn = pdPASS;
  xReturn = xQueueSend(target, (void *)pkg, 0);
  if (xReturn == xReturn) log_d("send pkg[%d] succeed!", pkg->msg_pkg_type);
  return;
}
