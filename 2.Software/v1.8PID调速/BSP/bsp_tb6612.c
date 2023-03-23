#include "bsp_tb6612.h"

static uint8_t ioctl(tb6612_t *tb6612, uint32_t gpio, u8 value);

static void open_gpiox_clock(GPIO_TypeDef *gpiox) {
  // 根据GPIO组初始化GPIO时钟
  if (gpiox == GPIOA) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // 使能GPIOA时钟
  }
  if (gpiox == GPIOB) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // 使能GPIOB时钟
  }
  if (gpiox == GPIOC) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // 使能GPIOC时钟
  }
  if (gpiox == GPIOD) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  // 使能GPIOD时钟
  }
  if (gpiox == GPIOE) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  // 使能GPIOE时钟
  }
}

static void gpio_init(tb6612_t *tb6612) {
  GPIO_InitTypeDef GPIO_Initure;

  /* 开启时钟 */
  open_gpiox_clock(tb6612->GPIOx_A1);
  open_gpiox_clock(tb6612->GPIOx_A2);
  open_gpiox_clock(tb6612->GPIOx_B1);
  open_gpiox_clock(tb6612->GPIOx_B2);

  // GPIO_LED初始化设置
  GPIO_Initure.GPIO_Pin = tb6612->GPIO_A1;
  GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;  // 通用推挽输出
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
    case 0: /* 低电平 */
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
    case 1: /* 高电平 */
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
      return 1; /* 参数错误 */
  }
  return 0;
}



void tb6612_init(tb6612_t *tb6612) {
  gpio_init(tb6612);
}

/*!
 * \brief   TB6612真值表
 *          | 左电机 | | 右电机 | | 小车状态 |
 *          | AIN1 | | AIN2 | | PWMA | | A电机 |
 *          |   0  | |   0  | |   H  | | 停 止 |
 *          |   0  | |   1  | |   H  | | 正 转 |
 *          |   1  | |   0  | |   H  | | 反 转 |
 *          |   X  | |   X  | |   L  | | 停 止 |
 *
 * \param[in] tb6612 操作的对象
 * \param[in] id 操作的电机
 * \param[in] speed 电机的速度
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
