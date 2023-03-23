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
  /* 计算偏差 */
  this->bias = target - current;
  /* 累计偏差 */
  this->bias_integral += this->bias;
  /* 积分限幅 */
  // TODO
  /* 计算输出 */
  out = this->kd * this->bias +          /* 比例 */
        this->ki * this->bias_integral + /* 积分 */
        this->kd * this->bias_last;      /* 微分 */
  /* 记录上次偏差 */
  this->bias_last = this->bias;
  /* 输出限幅 */
  // TODO
  return out;
}
