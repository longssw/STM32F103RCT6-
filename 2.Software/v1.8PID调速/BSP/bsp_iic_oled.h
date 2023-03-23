#ifndef __BSP_IIC_OLED_H
#define __BSP_IIC_OLED_H

#include "stm32f10x.h"
#include <stdio.h>

/* ����IIC�ӿ� */
#include "bsp_iic.h"

/*!
 * \brief OLED��IIC�ӻ���ַ(�ɱ��溸�ӵĵ������)
 */
#define OLED_ADDR 0x78
/*!
 * \brief ����й��
 */
#define OLED_MAX_COLUMN 128
/*!
 * \brief ���ȵ��� 0~255 Խ��Խ��
 */
#define OLED_BRIGHTNESS 0xFF

#define OLED_CMD 0  /* д���� */
#define OLED_DATA 1 /* д���� */

// ����OELD��
typedef struct OELD_Type
{
  
  IIC_TypeDef IIC; // IIC����
  /* �����Ĵ��� */
  void (*OLED_WriteCommand)(const struct OELD_Type *, uint8_t);
  void (*OLED_WriteData)(const struct OELD_Type *, uint8_t);
  void (*OLED_WRByte)(const struct OELD_Type *, uint8_t, uint8_t);
  /* �м�� */
  void (*OLED_SetPos)(const struct OELD_Type *, uint8_t, uint8_t);
  void (*OLED_ShowChar)(const struct OELD_Type *, uint8_t, uint8_t, uint8_t, uint8_t);
  /* Ӧ�ò�API */
  void (*OLED_Init)(const struct OELD_Type *);
  void (*OLED_Clear)(const struct OELD_Type *);
  void (*OLED_DrawBMP)(const struct OELD_Type *, uint8_t,
                       uint8_t, uint8_t, uint8_t, uint8_t[]);
  void (*OLED_ShowString)(const struct OELD_Type *, uint8_t, uint8_t, uint8_t *, uint8_t);
  void (*OELD_ShowCHinese)(const struct OELD_Type *, uint8_t, uint8_t, uint8_t);
  uint8_t (*OLED_Print)(const struct OELD_Type *, uint8_t, uint8_t, const char *, ...);
} OELD_TypeDef;

/* ���캯�� */
void oled_init(struct OELD_Type *oled_type_t);


#endif /* __BSP_IIC_OLED_H */
