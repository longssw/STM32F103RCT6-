#include "bsp_tb6612.h"

static uint8_t ioctl(tb6612_t *tb6612, uint32_t gpio, u8 value);

static void open_gpiox_clock(GPIO_TypeDef *gpiox) {
  // ����GPIO���ʼ��GPIOʱ��
  if (gpiox == GPIOA) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // ʹ��GPIOAʱ��
  }
  if (gpiox == GPIOB) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // ʹ��GPIOBʱ��
  }
  if (gpiox == GPIOC) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // ʹ��GPIOCʱ��
  }
  if (gpiox == GPIOD) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  // ʹ��GPIODʱ��
  }
  if (gpiox == GPIOE) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  // ʹ��GPIOEʱ��
  }
}

static void gpio_init(tb6612_t *tb6612) {
  GPIO_InitTypeDef GPIO_Initure;

  /* ����ʱ�� */
  open_gpiox_clock(tb6612->GPIOx_A1);
  open_gpiox_clock(tb6612->GPIOx_A2);
  open_gpiox_clock(tb6612->GPIOx_B1);
  open_gpiox_clock(tb6612->GPIOx_B2);

  // GPIO_LED��ʼ������
  GPIO_Initure.GPIO_Pin = tb6612->GPIO_A1;
  GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;  // ͨ���������
  GPIO_Init(tb6612->GPIOx_A1, &GPIO_Initure);

  GPIO_Initure.GPIO_Pin = tb6612->GPIO_A2;
  GPIO_Init(tb6612->GPIOx_A2, &GPIO_Initure);

  GPIO_Initure.GPIO_Pin = tb6612->GPIO_B1;
  GPIO_Init(tb6612->GPIOx_B1, &GPIO_Initure);

  GPIO_Initure.GPIO_Pin = tb6612->GPIO_B2;
  GPIO_Init(tb6612->GPIOx_B2, &GPIO_Initure);

  ioctl(tb6612, tb6612->GPIO_A1, 1);
  ioctl(tb6612, tb6612->GPIO_A2, 1);
  ioctl(tb6612, tb6612->GPIO_B1, 1);
  ioctl(tb6612, tb6612->GPIO_B2, 1);
}

static uint8_t ioctl(tb6612_t *tb6612, uint32_t gpio, u8 value) {
  switch (value) {
    case 0: /* �͵�ƽ */
      if (gpio == tb6612->GPIO_A1) {
        GPIO_ResetBits(tb6612->GPIOx_A1, tb6612->GPIO_A1);
      }
      if (gpio == tb6612->GPIO_A2) {
        GPIO_ResetBits(tb6612->GPIOx_A2, tb6612->GPIO_A2);
      }
      if (gpio == tb6612->GPIO_B1) {
        GPIO_ResetBits(tb6612->GPIOx_B1, tb6612->GPIO_B1);
      }
      if (gpio == tb6612->GPIO_B2) {
        GPIO_ResetBits(tb6612->GPIOx_B2, tb6612->GPIO_B2);
      }
      break;
    case 1: /* �ߵ�ƽ */
      if (gpio == tb6612->GPIO_A1) {
        GPIO_SetBits(tb6612->GPIOx_A1, tb6612->GPIO_A1);
      }
      if (gpio == tb6612->GPIO_A2) {
        GPIO_SetBits(tb6612->GPIOx_A2, tb6612->GPIO_A2);
      }
      if (gpio == tb6612->GPIO_B1) {
        GPIO_SetBits(tb6612->GPIOx_B1, tb6612->GPIO_B1);
      }
      if (gpio == tb6612->GPIO_B2) {
        GPIO_SetBits(tb6612->GPIOx_B2, tb6612->GPIO_B2);
      }
      break;
    default:
      return 1; /* �������� */
  }
  return 0;
}



void tb6612_init(tb6612_t *tb6612) {
  gpio_init(tb6612);
}

/*!
 * \brief   TB6612��ֵ��
 *          | ���� | | �ҵ�� | | С��״̬ |
 *          | AIN1 | | AIN2 | | PWMA | | A��� |
 *          |   0  | |   0  | |   H  | | ͣ ֹ |
 *          |   0  | |   1  | |   H  | | �� ת |
 *          |   1  | |   0  | |   H  | | �� ת |
 *          |   X  | |   X  | |   L  | | ͣ ֹ |
 *
 * \param[in] tb6612 �����Ķ���
 * \param[in] id �����ĵ��
 * \param[in] speed ������ٶ�
 */
void tb6612_setup(tb6612_t *const this, const enum tb6612_motor_id id,
                  const enum motor_rotation_direction direction) {
  switch (id) {
    case MOTORA:
      switch (direction) {
        case M_CLOCKWISE:
          ioctl(this, this->GPIO_A1, 1);
          ioctl(this, this->GPIO_A2, 0);
          break;
        case M_ANTICLOCKWISE:
          ioctl(this, this->GPIO_A1, 0);
          ioctl(this, this->GPIO_A2, 1);
          break;
        case M_STOP:
          ioctl(this, this->GPIO_A1, 0);
          ioctl(this, this->GPIO_A2, 0);
          break;
      }
      break;
    case MOTORB:
      switch (direction) {
        case M_CLOCKWISE:
          ioctl(this, this->GPIO_B1, 1);
          ioctl(this, this->GPIO_B2, 0);
          break;
        case M_ANTICLOCKWISE:
          ioctl(this, this->GPIO_B1, 0);
          ioctl(this, this->GPIO_B2, 1);
          break;
        case M_STOP:
          ioctl(this, this->GPIO_B1, 0);
          ioctl(this, this->GPIO_B2, 0);
          break;
      }
      break;
  }
}
