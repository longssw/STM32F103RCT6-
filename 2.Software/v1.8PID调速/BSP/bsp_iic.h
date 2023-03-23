#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#include "stm32f10x.h"

// 定义IIC类
typedef struct IIC_Type
{
    // 属性
    GPIO_TypeDef *GPIOx_SCL; // GPIO_SCL所属的GPIO组(如：GPIOA)
    GPIO_TypeDef *GPIOx_SDA; // GPIO_SDA所属的GPIO组(如：GPIOA)
    uint32_t GPIO_SCL;       // GPIO_SCL的IO引脚(如：GPIO_PIN_0)
    uint32_t GPIO_SDA;       // GPIO_SDA的IO引脚(如：GPIO_PIN_0)
    // 操作
    void (*IIC_Init)(const struct IIC_Type *);  // IIC_Init
    void (*IIC_Start)(const struct IIC_Type *); // IIC_Start
    void (*IIC_Stop)(const struct IIC_Type *);  // IIC_Stop
    /*!
     * \param[out] return 0表示正确应答，1表示无器件响应
     */
    uint8_t (*IIC_Wait_Ack)(const struct IIC_Type *);
    void (*IIC_Ack)(const struct IIC_Type *);                   // IIC_Ack,IIC发送ACK信号
    void (*IIC_NAck)(const struct IIC_Type *);                  // IIC_NAck,IIC发送NACK信号
    void (*IIC_Send_Byte)(const struct IIC_Type *, uint8_t);    // IIC_Send_Byte，入口参数为要发送的字节
    /*!
     * \brief   0发送NCK 1发送ACK
     */
    uint8_t (*IIC_Read_Byte)(const struct IIC_Type *, uint8_t); // IIC_Send_Byte，入口参数为是否要发送ACK信号
    void (*delay_us)(uint32_t);                                 // us延时
} IIC_TypeDef;

/* 创建IIC类 */
void create_iic(struct IIC_Type *IIC_Type_t);
#endif /* __BSP_IIC_H */
