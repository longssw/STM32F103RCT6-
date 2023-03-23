#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#include "stm32f10x.h"

// ����IIC��
typedef struct IIC_Type
{
    // ����
    GPIO_TypeDef *GPIOx_SCL; // GPIO_SCL������GPIO��(�磺GPIOA)
    GPIO_TypeDef *GPIOx_SDA; // GPIO_SDA������GPIO��(�磺GPIOA)
    uint32_t GPIO_SCL;       // GPIO_SCL��IO����(�磺GPIO_PIN_0)
    uint32_t GPIO_SDA;       // GPIO_SDA��IO����(�磺GPIO_PIN_0)
    // ����
    void (*IIC_Init)(const struct IIC_Type *);  // IIC_Init
    void (*IIC_Start)(const struct IIC_Type *); // IIC_Start
    void (*IIC_Stop)(const struct IIC_Type *);  // IIC_Stop
    /*!
     * \param[out] return 0��ʾ��ȷӦ��1��ʾ��������Ӧ
     */
    uint8_t (*IIC_Wait_Ack)(const struct IIC_Type *);
    void (*IIC_Ack)(const struct IIC_Type *);                   // IIC_Ack,IIC����ACK�ź�
    void (*IIC_NAck)(const struct IIC_Type *);                  // IIC_NAck,IIC����NACK�ź�
    void (*IIC_Send_Byte)(const struct IIC_Type *, uint8_t);    // IIC_Send_Byte����ڲ���ΪҪ���͵��ֽ�
    /*!
     * \brief   0����NCK 1����ACK
     */
    uint8_t (*IIC_Read_Byte)(const struct IIC_Type *, uint8_t); // IIC_Send_Byte����ڲ���Ϊ�Ƿ�Ҫ����ACK�ź�
    void (*delay_us)(uint32_t);                                 // us��ʱ
} IIC_TypeDef;

/* ����IIC�� */
void create_iic(struct IIC_Type *IIC_Type_t);
#endif /* __BSP_IIC_H */
