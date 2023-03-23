/*****************************************************************************
 * copyleft (c) 2023 �� ˼ ά
 *
 * �� �� ��:   motor_task.c
 * ��    ��:   ����ٶȿ���
 * ��    ��:   ��˼ά
 * ��    ��:   2023-02-28 20:53:30
 * ��    ��:   v1.0.2
 * ---------------------------------------------------------------------------
 * �ļ��޸���ʷ:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-02-28  |   v1.0.0    |    ssw     |     �����ļ�
 *  2023-03-3   |   v1.0.1    |    ssw     |     ���ӱ�����
 *  2023-03-4   |   v1.0.2    |    ssw     |     ����PID
 * ---------------------------------------------------------------------------
 ******************************************************************************/
/*================================ ͷ �� �� ==================================*/
#include "bsp_encoder.h" /* ��������� */
#include "bsp_pid.h"     /* PID���� */
#include "bsp_pwm.h"     /* ���PWM���� */
#include "bsp_tb6612.h"  /* ���ת����� */
#include "task_init.h"   /* RTOS���� */
#include "task_msg.h"    /* ��Ϣ���нṹ�� */
#define LOG_TAG "MOTOR"
#include <elog.h>/* ��־ */
/*=========================== �� �� �� / �� �� �� ============================*/
enum motor_cmd { SET_PWM = 0 };
enum motor_print { PRINT_ORIGINAL = 0, PRINT_SPEED, PRINT_PID_OUT };
enum motor_id { M1 = 0, M2, M3, M4, MSUM };
struct encoder {
  short int original; /* ������ԭʼֵ */
  float speed;        /* ת������ٶ�ֵ m/s */
};

struct motor {
  struct tb6612 tb6612[2];
  struct pwm pwm;
  struct encoder encoder[MSUM];
};
/*============================== ˽ �� �� �� ================================*/
static void motor_init(void);
static void motor4_speed_setup(struct motor *this, float speed);
static void motor3_speed_setup(struct motor *this, float speed);
static void motor2_speed_setup(struct motor *this, float speed);
static void motor1_speed_setup(struct motor *this, float speed);
static void motor_wait_msg(void);      /* ������Ϣ���� */
static void motor_print(const u8 tag); /* ��ӡ��Ϣ */
static void encoder_update(void);      /* ���±�����ֵ */
static void motor_speed_update(void);
static void motor_msg_handle(const struct task_msg_pack pkg);
/*=============================== �� �� �� =================================*/
#define MOTOR_SPEED(id, speed) (pmotor_speed[id](&motors, speed))
/*============================== ȫ �� �� �� ================================*/
static struct motor motors;
struct location_pid pid;
static void (*pmotor_speed[MSUM])(struct motor *const this,
                                  const float speed) = {
    motor1_speed_setup, motor2_speed_setup, motor3_speed_setup,
    motor4_speed_setup};
/*============================== �� �� �� �� ================================*/

void motor_task(void *parameter) {
  EncoderInit();
  motor_init();
  while (1) {
    motor_wait_msg();
    motor_print(PRINT_ORIGINAL);
    motor_speed_update();
		MOTOR_SPEED(M1,0.2);
		MOTOR_SPEED(M2,0.2);
		MOTOR_SPEED(M3,0.2);
		MOTOR_SPEED(M4,0.2);
    vTaskDelay(100);
  }
}

static void encoder_update(void) {
  short int tmp;
  for (u8 i = 0; i < MSUM; i++) {
    tmp = encoder_read(i + 2);
    motors.encoder[i].original = tmp;
    motors.encoder[i].speed = (float)(tmp / (110 * 0.1) / 10 * 0.2041);
  }
}

static void motor_speed_update(void) { encoder_update(); }

static void motor_print(const u8 tag) {
#if 0
  switch (tag) {
    case PRINT_ORIGINAL:
      log_i("M1[%d] M2[%d] M3[%d] M4[%d]", /* ��ӡԭʼ����ٶ� */
            motors.encoder[M1].original, motors.encoder[M2].original,
            motors.encoder[M3].original, motors.encoder[M4].original);
      break;

    default:
      break;
  }
#endif
}

static void motor_init(void) {
  motors.tb6612[0].GPIOx_A1 = GPIOC;
  motors.tb6612[0].GPIO_A1 = GPIO_Pin_0;
  motors.tb6612[0].GPIOx_A2 = GPIOC;
  motors.tb6612[0].GPIO_A2 = GPIO_Pin_1;
  motors.tb6612[0].GPIOx_B1 = GPIOC;
  motors.tb6612[0].GPIO_B1 = GPIO_Pin_2;
  motors.tb6612[0].GPIOx_B2 = GPIOC;
  motors.tb6612[0].GPIO_B2 = GPIO_Pin_3;

  motors.tb6612[1].GPIOx_A1 = GPIOC;
  motors.tb6612[1].GPIO_A1 = GPIO_Pin_4;
  motors.tb6612[1].GPIOx_A2 = GPIOC;
  motors.tb6612[1].GPIO_A2 = GPIO_Pin_5;
  motors.tb6612[1].GPIOx_B1 = GPIOB;
  motors.tb6612[1].GPIO_B1 = GPIO_Pin_14;
  motors.tb6612[1].GPIOx_B2 = GPIOB;
  motors.tb6612[1].GPIO_B2 = GPIO_Pin_15;

  motors.pwm = pwm_static_create(TIM8, 10000);
  tb6612_init(&motors.tb6612[0]);
  tb6612_init(&motors.tb6612[1]);
  log_i("Motor init!\r\n");
}

static void motor1_speed_setup(struct motor *const this, const float speed) {
  /* ע�������ת����ݵ����װλ�õ��� */
  if (speed > 0) {
    tb6612_setup(&this->tb6612[0], MOTORA, M_ANTICLOCKWISE);
    pwm_setup(&this->pwm, CH1, speed);
  } else {
    tb6612_setup(&this->tb6612[0], MOTORA, M_CLOCKWISE);
    pwm_setup(&this->pwm, CH1, speed);
  }
}

static void motor2_speed_setup(struct motor *const this, const float speed) {
  if (speed > 0) {
    tb6612_setup(&this->tb6612[0], MOTORB, M_ANTICLOCKWISE);
    pwm_setup(&this->pwm, CH2, speed);
  } else {
    tb6612_setup(&this->tb6612[0], MOTORB, M_CLOCKWISE);
    pwm_setup(&this->pwm, CH2, speed);
  }
}

static void motor3_speed_setup(struct motor *const this, const float speed) {
  if (speed > 0) {
    tb6612_setup(&this->tb6612[1], MOTORA, M_CLOCKWISE);
    pwm_setup(&this->pwm, CH3, speed);
  } else {
    tb6612_setup(&this->tb6612[1], MOTORA, M_ANTICLOCKWISE);
    pwm_setup(&this->pwm, CH3, speed);
  }
}

static void motor4_speed_setup(struct motor *const this, const float speed) {
  if (speed > 0) {
    tb6612_setup(&this->tb6612[1], MOTORB, M_CLOCKWISE);
    pwm_setup(&this->pwm, CH4, speed);
  } else {
    tb6612_setup(&this->tb6612[1], MOTORB, M_ANTICLOCKWISE);
    pwm_setup(&this->pwm, CH4, speed);
  }
}

static void motor_wait_msg(void) {
  BaseType_t xReturn = pdTRUE; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdTRUE */
  struct task_msg_pack msg; /* ����һ��������Ϣ�ı��� */

  xReturn = xQueuePeek(Task_Queue, /* ��Ϣ���еľ�� */
                       &msg,       /* ���յ���Ϣ���� */
                       0);         /* �ȴ�ʱ�� */
  if (pdTRUE == xReturn) {
    if (msg.msg_pkg_type != PKG_MOTOR) return;
    xQueueReceive(Task_Queue, &msg, 0);
    if (msg.info.motor_pkg.id > 0 && msg.info.motor_pkg.id <= MSUM) {
      MOTOR_SPEED(msg.info.motor_pkg.id - 1, msg.info.motor_pkg.speed);
    }
  }
}

static void motor_msg_handle(const struct task_msg_pack pkg) {
  switch (pkg.info.motor_pkg.cmd) {
    case SET_PWM:
      if (pkg.info.motor_pkg.id > 0 && pkg.info.motor_pkg.id <= MSUM) {
        MOTOR_SPEED(pkg.info.motor_pkg.id - 1, pkg.info.motor_pkg.speed);
      }
      break;
  }
}
