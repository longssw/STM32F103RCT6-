#ifndef BSP_TB6612_H
#define BSP_TB6612_H
#include "stm32f10x.h"

enum motor_rotation_direction { M_STOP, M_CLOCKWISE, M_ANTICLOCKWISE };
enum tb6612_motor_id { MOTORA, MOTORB, MOTOR_SUM };

typedef struct tb6612 {
  //  Ù–‘
  GPIO_TypeDef *GPIOx_A1;
  GPIO_TypeDef *GPIOx_A2;
  GPIO_TypeDef *GPIOx_B1;
  GPIO_TypeDef *GPIOx_B2;
  uint32_t GPIO_A1;
  uint32_t GPIO_A2;
  uint32_t GPIO_B1;
  uint32_t GPIO_B2;
} tb6612_t;

void tb6612_init(tb6612_t *tb6612);
void tb6612_setup(tb6612_t *const this, const enum tb6612_motor_id id,
                  const enum motor_rotation_direction direction);
#endif /* BSP_TB6612_H */
