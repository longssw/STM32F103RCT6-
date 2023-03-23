#include "bsp_pid.h"

struct location_pid location_pid_static_create(const float kp, const float ki,
                                               const float kd) {
  struct location_pid this;
  this.kp = kp;
  this.ki = ki;
  this.kd = kd;
  this.bias = 0;
  this.bias_last = 0;
  this.bias_integral = 0;
  return this;
}

void location_pid_setup(struct location_pid *const this, const float kp,
                        const float ki, const float kd) {
  this->kp = kp;
  this->ki = ki;
  this->kd = kd;
}

float location_pid_calc(struct location_pid *const this, const u16 target,
                        const u16 current) {
  float out;
  /* ����ƫ�� */
  this->bias = target - current;
  /* �ۼ�ƫ�� */
  this->bias_integral += this->bias;
  /* �����޷� */
  // TODO
  /* ������� */
  out = this->kd * this->bias +          /* ���� */
        this->ki * this->bias_integral + /* ���� */
        this->kd * this->bias_last;      /* ΢�� */
  /* ��¼�ϴ�ƫ�� */
  this->bias_last = this->bias;
  /* ����޷� */
  // TODO
  return out;
}
