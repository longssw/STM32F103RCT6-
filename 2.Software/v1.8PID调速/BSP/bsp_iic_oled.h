#ifndef __BSP_IIC_OLED_H
#define __BSP_IIC_OLED_H

#include "stm32f10x.h"
#include <stdio.h>

/* 父类IIC接口 */
#include "bsp_iic.h"

/*!
 * \brief OLED的IIC从机地址(由背面焊接的电阻决定)
 */
#define OLED_ADDR 0x78
/*!
 * \brief 最大行光标
 */
#define OLED_MAX_COLUMN 128
/*!
 * \brief 亮度调节 0~255 越大越亮
 */
#define OLED_BRIGHTNESS 0xFF

#define OLED_CMD 0  /* 写命令 */
#define OLED_DATA 1 /* 写数据 */

// 定义OELD类
typedef struct OELD_Type
{
  
  IIC_TypeDef IIC; // IIC驱动
  /* 操作寄存器 */
  void (*OLED_WriteCommand)(const struct OELD_Type *, uint8_t);
  void (*OLED_WriteData)(const struct OELD_Type *, uint8_t);
  void (*OLED_WRByte)(const struct OELD_Type *, uint8_t, uint8_t);
  /* 中间层 */
  void (*OLED_SetPos)(const struct OELD_Type *, uint8_t, uint8_t);
  void (*OLED_ShowChar)(const struct OELD_Type *, uint8_t, uint8_t, uint8_t, uint8_t);
  /* 应用层API */
  void (*OLED_Init)(const struct OELD_Type *);
  void (*OLED_Clear)(const struct OELD_Type *);
  void (*OLED_DrawBMP)(const struct OELD_Type *, uint8_t,
                       uint8_t, uint8_t, uint8_t, uint8_t[]);
  void (*OLED_ShowString)(const struct OELD_Type *, uint8_t, uint8_t, uint8_t *, uint8_t);
  void (*OELD_ShowCHinese)(const struct OELD_Type *, uint8_t, uint8_t, uint8_t);
  uint8_t (*OLED_Print)(const struct OELD_Type *, uint8_t, uint8_t, const char *, ...);
} OELD_TypeDef;

/* 构造函数 */
void oled_init(struct OELD_Type *oled_type_t);


#endif /* __BSP_IIC_OLED_H */
