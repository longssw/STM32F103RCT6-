#ifndef __BSP_PID_H
#define	__BSP_PID_H
#include "stm32f10x.h"

struct location_pid {
  float kp;
  float ki;
  float kd;
  u32 bias;          /* ƫ�� */
  u32 bias_last;     /* �ϴ�ƫ�� */
  u32 bias_integral; /* ƫ����� */
};

struct location_pid location_pid_static_create(const float kp, const float ki,
                                          const float kd);
void location_pid_setup(struct location_pid *const this, const float kp,
                        const float ki, const float kd);
float location_pid_calc(struct location_pid *const this, const u16 target,
                         const u16 current);

#endif /* __USART_H */
