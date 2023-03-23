#include "bsp_iic.h"

static void IIC_Init_t(const struct IIC_Type *IIC_Type_t);
static void IIC_SCL(const struct IIC_Type *IIC_Type_t, int n);
static void IIC_SDA(const struct IIC_Type *IIC_Type_t, int n);
static uint8_t READ_SDA(const struct IIC_Type *IIC_Type_t);
static void IIC_Start_t(const struct IIC_Type *IIC_Type_t);
static void IIC_Stop_t(const struct IIC_Type *IIC_Type_t);
static uint8_t IIC_Wait_Ack_t(const struct IIC_Type *IIC_Type_t);
static void IIC_Ack_t(const struct IIC_Type *IIC_Type_t);
static void IIC_NAck_t(const struct IIC_Type *IIC_Type_t);
static void IIC_Send_Byte_t(const struct IIC_Type *IIC_Type_t, uint8_t txd);
static uint8_t IIC_Read_Byte_t(const struct IIC_Type *IIC_Type_t, uint8_t ack);
static void SDA_IN(const struct IIC_Type *IIC_Type_t);
static void SDA_OUT(const struct IIC_Type *IIC_Type_t);

static void SDA_IN(const struct IIC_Type *IIC_Type_t)
{
#if 0
    // uint8_t io_num = 0;
    // switch (IIC_Type_t->GPIO_SDA)
    // {
    // case GPIO_Pin_0:
    //     IIC_Type_t->GPIOx_SDA->CRL &= 0XFFFFFFF0;
    //     io_num = 0;
    //     break;
    // case GPIO_Pin_1:
    //     IIC_Type_t->GPIOx_SDA->CRL &= 0XFFFFFF0F;
    //     io_num = 1;
    //     break;
    // case GPIO_Pin_2:
    //     IIC_Type_t->GPIOx_SDA->CRL &= 0XFFFFF0FF;
    //     io_num = 2;
    //     break;
    // case GPIO_Pin_3:
    //     IIC_Type_t->GPIOx_SDA->CRL &= 0XFFFF0FFF;
    //     io_num = 3;
    //     break;
    // case GPIO_Pin_4:
    //     IIC_Type_t->GPIOx_SDA->CRL &= 0XFFF0FFFF;
    //     io_num = 4;
    //     break;
    // case GPIO_Pin_5:
    //     IIC_Type_t->GPIOx_SDA->CRL &= 0XFF0FFFFF;
    //     io_num = 5;
    //     break;
    // case GPIO_Pin_6:
    //     IIC_Type_t->GPIOx_SDA->CRL &= 0XF0FFFFFF;
    //     io_num = 6;
    //     break;
    // case GPIO_Pin_7:
    //     IIC_Type_t->GPIOx_SDA->CRL &= 0X0FFFFFFF;
    //     io_num = 7;
    //     break;
    // case GPIO_Pin_8:
    //     IIC_Type_t->GPIOx_SDA->CRH &= 0XFFFFFFF0;
    //     io_num = 8;
    //     break;
    // case GPIO_Pin_9:
    //     IIC_Type_t->GPIOx_SDA->CRH &= 0XFFFFFF0F;
    //     io_num = 9;
    //     break;
    // case GPIO_Pin_10:
    //     IIC_Type_t->GPIOx_SDA->CRH &= 0XFFFFF0FF;
    //     io_num = 10;
    //     break;
    // case GPIO_Pin_11:
    //     IIC_Type_t->GPIOx_SDA->CRH &= 0XFFFF0FFF;
    //     io_num = 11;
    //     break;
    // case GPIO_Pin_12:
    //     IIC_Type_t->GPIOx_SDA->CRH &= 0XFFF0FFFF;
    //     io_num = 12;
    //     break;
    // case GPIO_Pin_13:
    //     IIC_Type_t->GPIOx_SDA->CRH &= 0XFF0FFFFF;
    //     io_num = 13;
    //     break;
    // case GPIO_Pin_14:
    //     IIC_Type_t->GPIOx_SDA->CRH &= 0XF0FFFFFF;
    //     io_num = 14;
    //     break;
    // case GPIO_Pin_15:
    //     IIC_Type_t->GPIOx_SDA->CRH &= 0X0FFFFFFF;
    //     io_num = 15;
    //     break;
    // }
    // if (io_num < 8)
    // {
    //     IIC_Type_t->GPIOx_SDA->CRL |= (8 << (io_num * 4));
    // }
    // else
    // {
    //     io_num = io_num - 7;
    //     IIC_Type_t->GPIOx_SDA->CRH |= 8 << (io_num * 4);
    // }
#endif

    switch (IIC_Type_t->GPIO_SDA)
    {
    case GPIO_Pin_7:
        {GPIOA->CRL&=0X0FFFFFFF;GPIOA->CRL|=(u32)(8<<28);} 
        break;
    }
}

static void SDA_OUT(const struct IIC_Type *IIC_Type_t)
{
    #if 0
    uint8_t io_num = 0;
    switch (IIC_Type_t->GPIO_SDA)
    {
    case GPIO_Pin_0:
        IIC_Type_t->GPIOx_SDA->CRL &= 0XFFFFFFF0;
        io_num = 0;
        break;
    case GPIO_Pin_1:
        IIC_Type_t->GPIOx_SDA->CRL &= 0XFFFFFF0F;
        io_num = 1;
        break;
    case GPIO_Pin_2:
        IIC_Type_t->GPIOx_SDA->CRL &= 0XFFFFF0FF;
        io_num = 2;
        break;
    case GPIO_Pin_3:
        IIC_Type_t->GPIOx_SDA->CRL &= 0XFFFF0FFF;
        io_num = 3;
        break;
    case GPIO_Pin_4:
        IIC_Type_t->GPIOx_SDA->CRL &= 0XFFF0FFFF;
        io_num = 4;
        break;
    case GPIO_Pin_5:
        IIC_Type_t->GPIOx_SDA->CRL &= 0XFF0FFFFF;
        io_num = 5;
        break;
    case GPIO_Pin_6:
        IIC_Type_t->GPIOx_SDA->CRL &= 0XF0FFFFFF;
        io_num = 6;
        break;
    case GPIO_Pin_7:
        IIC_Type_t->GPIOx_SDA->CRL &= 0X0FFFFFFF;
        io_num = 7;
        break;
    case GPIO_Pin_8:
        IIC_Type_t->GPIOx_SDA->CRH &= 0XFFFFFFF0;
        io_num = 8;
        break;
    case GPIO_Pin_9:
        IIC_Type_t->GPIOx_SDA->CRH &= 0XFFFFFF0F;
        io_num = 9;
        break;
    case GPIO_Pin_10:
        IIC_Type_t->GPIOx_SDA->CRH &= 0XFFFFF0FF;
        io_num = 10;
        break;
    case GPIO_Pin_11:
        IIC_Type_t->GPIOx_SDA->CRH &= 0XFFFF0FFF;
        io_num = 11;
        break;
    case GPIO_Pin_12:
        IIC_Type_t->GPIOx_SDA->CRH &= 0XFFF0FFFF;
        io_num = 12;
        break;
    case GPIO_Pin_13:
        IIC_Type_t->GPIOx_SDA->CRH &= 0XFF0FFFFF;
        io_num = 13;
        break;
    case GPIO_Pin_14:
        IIC_Type_t->GPIOx_SDA->CRH &= 0XF0FFFFFF;
        io_num = 14;
        break;
    case GPIO_Pin_15:
        IIC_Type_t->GPIOx_SDA->CRH &= 0X0FFFFFFF;
        io_num = 15;
        break;
    }
    if (io_num < 8)
    {
        IIC_Type_t->GPIOx_SDA->CRL |= (3 << (io_num * 4));
    }
    else
    {
        io_num = io_num - 7;
        IIC_Type_t->GPIOx_SDA->CRH |= 3 << (io_num * 4);
    }
    #endif
    switch (IIC_Type_t->GPIO_SDA)
    {
    case GPIO_Pin_7:
        GPIOA->CRL&=0X0FFFFFFF;GPIOA->CRL|=3<<28;
        break;
    }
}

// 初始化IIC引脚
static void IIC_Init_t(const struct IIC_Type *IIC_Type_t)
{
    GPIO_InitTypeDef GPIO_Initure;

    // 根据GPIO组初始化GPIO时钟
    if (IIC_Type_t->GPIOx_SCL == GPIOA || IIC_Type_t->GPIOx_SDA == GPIOA)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能GPIOA时钟
    }
    if (IIC_Type_t->GPIOx_SCL == GPIOB || IIC_Type_t->GPIOx_SDA == GPIOB)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能GPIOB时钟
    }
    if (IIC_Type_t->GPIOx_SCL == GPIOC || IIC_Type_t->GPIOx_SDA == GPIOC)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 使能GPIOC时钟
    }
    if (IIC_Type_t->GPIOx_SCL == GPIOD || IIC_Type_t->GPIOx_SDA == GPIOD)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // 使能GPIOD时钟
    }
    if (IIC_Type_t->GPIOx_SCL == GPIOE || IIC_Type_t->GPIOx_SDA == GPIOE)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); // 使能GPIOE时钟
    }

    // GPIO_SCL初始化设置
    GPIO_Initure.GPIO_Pin = IIC_Type_t->GPIO_SCL;
    GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_OD; // 开漏输出
    // GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_Init(IIC_Type_t->GPIOx_SCL, &GPIO_Initure);

    // GPIO_SDA初始化设置
    GPIO_Initure.GPIO_Pin = IIC_Type_t->GPIO_SDA;
    GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_OD; // 开漏输出
    // GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_Init(IIC_Type_t->GPIOx_SDA, &GPIO_Initure);

    // SCL与SDA的初始化均为高电平
    GPIO_SetBits(IIC_Type_t->GPIOx_SCL, IIC_Type_t->GPIO_SCL);
    GPIO_SetBits(IIC_Type_t->GPIOx_SDA, IIC_Type_t->GPIO_SDA);
    // log(INFO,"IIC init");
    //  IIC_SCL(IIC_Type_t, 1);
    //  IIC_SDA(IIC_Type_t, 1);
    /* 野火 */
    /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
    // IIC_Type_t->IIC_Stop(IIC_Type_t);
}

// 设置SCL电平
static void IIC_SCL(const struct IIC_Type *IIC_Type_t, int n)
{
    if (n == 1)
    {
        // 设置SCL为高电平
        GPIO_SetBits(IIC_Type_t->GPIOx_SCL, IIC_Type_t->GPIO_SCL);
    }
    else
    {
        // 设置SCL为低电平
        GPIO_ResetBits(IIC_Type_t->GPIOx_SCL, IIC_Type_t->GPIO_SCL);
    }
}

// 设置SDA电平
static void IIC_SDA(const struct IIC_Type *IIC_Type_t, int n)
{
    if (n == 1)
    {
        // 设置SDA为高电平
        GPIO_SetBits(IIC_Type_t->GPIOx_SDA, IIC_Type_t->GPIO_SDA);
    }
    else
    {
        // 设置SDA为低电平
        GPIO_ResetBits(IIC_Type_t->GPIOx_SDA, IIC_Type_t->GPIO_SDA);
    }
}

// 读取SDA电平
static uint8_t READ_SDA(const struct IIC_Type *IIC_Type_t)
{
    // return GPIO_ReadInputDataBit(IIC_Type_t->GPIOx_SDA, IIC_Type_t->GPIO_SDA); // 读取SDA电平
    /* 野火 */
    return ((IIC_Type_t->GPIOx_SDA->IDR & IIC_Type_t->GPIO_SDA) != 0);
}

// IIC Start
static void IIC_Start_t(const struct IIC_Type *IIC_Type_t)
{
#if 0
    SDA_OUT(IIC_Type_t); // sda线输出
    IIC_SDA(IIC_Type_t, 1);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(4);
    IIC_SDA(IIC_Type_t, 0); // START:when CLK is high,DATA change form high to low
    IIC_Type_t->delay_us(4);
    IIC_SCL(IIC_Type_t, 0); // 钳住I2C总线，准备发送或接收数据
#endif
    /* 野火 */
    SDA_OUT(IIC_Type_t);
    IIC_SDA(IIC_Type_t, 1);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(1);
    IIC_SDA(IIC_Type_t, 0); // START:when CLK is high,DATA change form high to low
    IIC_Type_t->delay_us(1);
    IIC_SCL(IIC_Type_t, 0); // 钳住I2C总线，准备发送或接收数据
    IIC_Type_t->delay_us(1);
}

// IIC Stop
static void IIC_Stop_t(const struct IIC_Type *IIC_Type_t)
{
#if 0
    SDA_OUT(IIC_Type_t); // sda线输出
    IIC_SCL(IIC_Type_t, 0);
    IIC_SDA(IIC_Type_t, 0); // STOP:when CLK is high DATA change form low to high
    IIC_Type_t->delay_us(4);
    IIC_SCL(IIC_Type_t, 1);
    IIC_SDA(IIC_Type_t, 1); // 发送I2C总线结束信号
    IIC_Type_t->delay_us(4);
#else /* 野火的 */
    SDA_OUT(IIC_Type_t);
    IIC_SDA(IIC_Type_t, 0);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(2);
    IIC_SDA(IIC_Type_t, 1);
#endif
}

// IIC_Wait_ack 返回0表示wait成功，返回1表示wait失败
static uint8_t IIC_Wait_Ack_t(const struct IIC_Type *IIC_Type_t) // IIC_Wait_ack,返回wait失败或是成功
{
#if 0
    uint8_t ucErrTime = 0;
    //SDA_IN(IIC_Type_t);      //SDA设置为输入
    IIC_SDA(IIC_Type_t, 1);
    IIC_Type_t->delay_us(1);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(1);
    while (READ_SDA(IIC_Type_t))
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_Type_t->IIC_Stop(IIC_Type_t);
            return 1;
        }
    }
    IIC_SCL(IIC_Type_t, 0); // 时钟输出0
    return 0;
#endif
#if 1
    /* 野火 */
    uint8_t re;
    SDA_IN(IIC_Type_t);
    IIC_SDA(IIC_Type_t, 1);
    IIC_Type_t->delay_us(1);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(1);
    if (READ_SDA(IIC_Type_t))
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    IIC_SCL(IIC_Type_t, 0);
    IIC_Type_t->delay_us(1);
    return re;
#endif
#if 0
    /* 原子 */
    u8 ucErrTime = 0;
    MPU_SDA_IN(); // SDA设置为输入
    MPU_IIC_SDA = 1;
    MPU_IIC_Delay();
    MPU_IIC_SCL = 1;
    MPU_IIC_Delay();
    while (MPU_READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    MPU_IIC_SCL = 0; // 时钟输出0
    return 0;
#endif

    // uint8_t re;
    // u8 ucErrTime = 0;
    // // SDA_IN(IIC_Type_t);
    // GPIOA->CRL &= 0X0FFFFFFF;
    // GPIOA->CRL |= 8 << 28;
    // IIC_SDA(IIC_Type_t, 1);
    // IIC_Type_t->delay_us(2);
    // IIC_SCL(IIC_Type_t, 1);
    // IIC_Type_t->delay_us(2);
    // while (READ_SDA(IIC_Type_t))
    // {
    //     ucErrTime++;
    //     if (ucErrTime > 250)
    //     {
    //         IIC_Type_t->IIC_Stop(IIC_Type_t);
    //         return 1;
    //     }
    // }
    // IIC_SCL(IIC_Type_t, 0);
    // return 0;
    // IIC_Type_t->delay_us(2);
    //  if (READ_SDA(IIC_Type_t))
    //  {
    //      re = 1;
    //  }
    //  else
    //  {
    //      re = 0;
    //  }
}

// 产生ACK应答
static void IIC_Ack_t(const struct IIC_Type *IIC_Type_t)
{
#if 0
    IIC_SCL(IIC_Type_t, 0);
    SDA_OUT(IIC_Type_t);
    IIC_SDA(IIC_Type_t, 0);
    IIC_Type_t->delay_us(2);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(2);
    IIC_SCL(IIC_Type_t, 0);
#endif
    /* 野火 */
    SDA_OUT(IIC_Type_t);
    IIC_SDA(IIC_Type_t, 0);
    IIC_Type_t->delay_us(2);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(2);
    IIC_SCL(IIC_Type_t, 0);
    IIC_Type_t->delay_us(2);
    IIC_SDA(IIC_Type_t, 1);
}

// 产生NACK应答
static void IIC_NAck_t(const struct IIC_Type *IIC_Type_t)
{
#if 0
    IIC_SCL(IIC_Type_t, 0);
    SDA_OUT(IIC_Type_t);
    IIC_SDA(IIC_Type_t, 1);
    IIC_Type_t->delay_us(2);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(2);
    IIC_SCL(IIC_Type_t, 0);
#endif
    /* 野火 */
    IIC_SDA(IIC_Type_t, 1);
    IIC_Type_t->delay_us(1);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(1);
    IIC_SCL(IIC_Type_t, 0);
    IIC_Type_t->delay_us(1);
}

// IIC_Send_Byte，入口参数为要发送的字节
static void IIC_Send_Byte_t(const struct IIC_Type *IIC_Type_t, uint8_t txd)
{
#if 0
    uint8_t t = 0;
    SDA_OUT(IIC_Type_t);
    IIC_SCL(IIC_Type_t, 0); // 拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        IIC_SDA(IIC_Type_t, (txd & 0x80) >> 7);
        txd <<= 1;
        IIC_Type_t->delay_us(2); // 对TEA5767这三个延时都是必须的
        IIC_SCL(IIC_Type_t, 1);
        IIC_Type_t->delay_us(2);
        IIC_SCL(IIC_Type_t, 0);
        IIC_Type_t->delay_us(2);
    }
#endif
    /* 野火 */
    uint8_t i;
    SDA_OUT(IIC_Type_t);
    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (txd & 0x80)
        {
            IIC_SDA(IIC_Type_t, 1);
            // EEPROM_I2C_SDA_1();
        }
        else
        {
            IIC_SDA(IIC_Type_t, 0);
            // EEPROM_I2C_SDA_0();
        }
        IIC_Type_t->delay_us(2);
        IIC_SCL(IIC_Type_t, 1);
        // EEPROM_I2C_SCL_1();
        IIC_Type_t->delay_us(2);
        IIC_SCL(IIC_Type_t, 0);
        // EEPROM_I2C_SCL_0();
        if (i == 7)
        {
            IIC_SDA(IIC_Type_t, 1);
            // EEPROM_I2C_SDA_1(); // 释放总线
        }
        txd <<= 1; /* 左移一个bit */
        IIC_Type_t->delay_us(2);
    }
}

// IIC_Send_Byte，入口参数为是否要发送ACK信号
static uint8_t IIC_Read_Byte_t(const struct IIC_Type *IIC_Type_t, uint8_t ack)
{
#if 0
    uint8_t i, receive = 0;
    SDA_IN(IIC_Type_t); // SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        IIC_SCL(IIC_Type_t, 0);
        IIC_Type_t->delay_us(2);
        IIC_SCL(IIC_Type_t, 1);
        receive <<= 1;
        if (READ_SDA(IIC_Type_t))
            receive++;
        IIC_Type_t->delay_us(1);
    }
    if (!ack)
        IIC_Type_t->IIC_NAck(IIC_Type_t); // 发送nACK
    else
        IIC_Type_t->IIC_Ack(IIC_Type_t); // 发送ACK
    return receive;
#endif
#if 1
    /* 野火 */
    uint8_t i;
    uint8_t value;
    SDA_IN(IIC_Type_t); // SDA设置为输入
    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        IIC_SCL(IIC_Type_t, 1);
        IIC_Type_t->delay_us(1);
        if (READ_SDA(IIC_Type_t))
        {
            value++;
        }
        IIC_SCL(IIC_Type_t, 0);
        IIC_Type_t->delay_us(1);
    }
    return value;
#endif
#if 0
    uint8_t i;
    uint8_t value;

    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        IIC_SCL(IIC_Type_t, 1);
        IIC_Type_t->delay_us(1);
        //if (EEPROM_I2C_SDA_READ())
        if (READ_SDA(IIC_Type_t))     
        {
            value++;
        }
        IIC_SCL(IIC_Type_t, 0);
        //EEPROM_I2C_SCL_0();
        IIC_Type_t->delay_us(1);
        // i2c_Delay();
    }
    return value;
#endif
}

void create_iic(struct IIC_Type *IIC_Type_t)
{
    IIC_Type_t->IIC_Init = IIC_Init_t;
    IIC_Type_t->IIC_Start = IIC_Start_t;
    IIC_Type_t->IIC_Stop = IIC_Stop_t;
    IIC_Type_t->IIC_Wait_Ack = IIC_Wait_Ack_t;
    IIC_Type_t->IIC_Ack = IIC_Ack_t;
    IIC_Type_t->IIC_NAck = IIC_NAck_t;
    IIC_Type_t->IIC_Send_Byte = IIC_Send_Byte_t;
    IIC_Type_t->IIC_Read_Byte = IIC_Read_Byte_t;
    IIC_Type_t->IIC_Init(IIC_Type_t);
}
