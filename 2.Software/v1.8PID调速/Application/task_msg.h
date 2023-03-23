/*****************************************************************************
 * copyleft (c) 2023 善 思 维
 *
 * 文 件 名:   task_msg.h
 * 功    能:   定义消息队列传递的结构体类型
 * 作    者:   善思维
 * 日    期:   2023-03-03 10:50:55
 * 版    本:   v1.0.1
 * ---------------------------------------------------------------------------
 * 文件修改历史:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-02-27  |   v1.0.0    |    ssw     |     创建文件
 *  2023-03-03  |   v1.0.1    |    ssw     |     用联合体优化
 * ---------------------------------------------------------------------------
 ******************************************************************************/
#ifndef __PARAM_H
#define __PARAM_H

#include "stm32f10x.h" /* 基本数据类型 如u8 */
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
