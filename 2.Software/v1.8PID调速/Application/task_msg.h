/*****************************************************************************
 * copyleft (c) 2023 �� ˼ ά
 *
 * �� �� ��:   task_msg.h
 * ��    ��:   ������Ϣ���д��ݵĽṹ������
 * ��    ��:   ��˼ά
 * ��    ��:   2023-03-03 10:50:55
 * ��    ��:   v1.0.1
 * ---------------------------------------------------------------------------
 * �ļ��޸���ʷ:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-02-27  |   v1.0.0    |    ssw     |     �����ļ�
 *  2023-03-03  |   v1.0.1    |    ssw     |     ���������Ż�
 * ---------------------------------------------------------------------------
 ******************************************************************************/
#ifndef __PARAM_H
#define __PARAM_H

#include "stm32f10x.h" /* ������������ ��u8 */
enum msg_pkg_type {
  PKG_LED = 0,
  PKG_MOTOR,
  PKG_ADC,
};

struct led_task_msg {
  int id;
  int state;
};

struct motor_task_msg {
  u8 cmd;
  u8 id;
  float speed;
};

struct adc_task_msg {
  u8 cmd;
};

struct task_msg_pack {
  enum msg_pkg_type msg_pkg_type;
  union {
    struct led_task_msg led_pkg;
    struct motor_task_msg motor_pkg;
    struct adc_task_msg adc_pkg;
  } info;
};
#endif /* __PARAM_H */
