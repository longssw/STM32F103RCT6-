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

// ��ʼ��IIC����
static void IIC_Init_t(const struct IIC_Type *IIC_Type_t)
{
    GPIO_InitTypeDef GPIO_Initure;

    // ����GPIO���ʼ��GPIOʱ��
    if (IIC_Type_t->GPIOx_SCL == GPIOA || IIC_Type_t->GPIOx_SDA == GPIOA)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // ʹ��GPIOAʱ��
    }
    if (IIC_Type_t->GPIOx_SCL == GPIOB || IIC_Type_t->GPIOx_SDA == GPIOB)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ʹ��GPIOBʱ��
    }
    if (IIC_Type_t->GPIOx_SCL == GPIOC || IIC_Type_t->GPIOx_SDA == GPIOC)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // ʹ��GPIOCʱ��
    }
    if (IIC_Type_t->GPIOx_SCL == GPIOD || IIC_Type_t->GPIOx_SDA == GPIOD)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // ʹ��GPIODʱ��
    }
    if (IIC_Type_t->GPIOx_SCL == GPIOE || IIC_Type_t->GPIOx_SDA == GPIOE)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); // ʹ��GPIOEʱ��
    }

    // GPIO_SCL��ʼ������
    GPIO_Initure.GPIO_Pin = IIC_Type_t->GPIO_SCL;
    GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_OD; // ��©���
    // GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
    GPIO_Init(IIC_Type_t->GPIOx_SCL, &GPIO_Initure);

    // GPIO_SDA��ʼ������
    GPIO_Initure.GPIO_Pin = IIC_Type_t->GPIO_SDA;
    GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_OD; // ��©���
    // GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
    GPIO_Init(IIC_Type_t->GPIOx_SDA, &GPIO_Initure);

    // SCL��SDA�ĳ�ʼ����Ϊ�ߵ�ƽ
    GPIO_SetBits(IIC_Type_t->GPIOx_SCL, IIC_Type_t->GPIO_SCL);
    GPIO_SetBits(IIC_Type_t->GPIOx_SDA, IIC_Type_t->GPIO_SDA);
    // log(INFO,"IIC init");
    //  IIC_SCL(IIC_Type_t, 1);
    //  IIC_SDA(IIC_Type_t, 1);
    /* Ұ�� */
    /* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
    // IIC_Type_t->IIC_Stop(IIC_Type_t);
}

// ����SCL��ƽ
static void IIC_SCL(const struct IIC_Type *IIC_Type_t, int n)
{
    if (n == 1)
    {
        // ����SCLΪ�ߵ�ƽ
        GPIO_SetBits(IIC_Type_t->GPIOx_SCL, IIC_Type_t->GPIO_SCL);
    }
    else
    {
        // ����SCLΪ�͵�ƽ
        GPIO_ResetBits(IIC_Type_t->GPIOx_SCL, IIC_Type_t->GPIO_SCL);
    }
}

// ����SDA��ƽ
static void IIC_SDA(const struct IIC_Type *IIC_Type_t, int n)
{
    if (n == 1)
    {
        // ����SDAΪ�ߵ�ƽ
        GPIO_SetBits(IIC_Type_t->GPIOx_SDA, IIC_Type_t->GPIO_SDA);
    }
    else
    {
        // ����SDAΪ�͵�ƽ
        GPIO_ResetBits(IIC_Type_t->GPIOx_SDA, IIC_Type_t->GPIO_SDA);
    }
}

// ��ȡSDA��ƽ
static uint8_t READ_SDA(const struct IIC_Type *IIC_Type_t)
{
    // return GPIO_ReadInputDataBit(IIC_Type_t->GPIOx_SDA, IIC_Type_t->GPIO_SDA); // ��ȡSDA��ƽ
    /* Ұ�� */
    return ((IIC_Type_t->GPIOx_SDA->IDR & IIC_Type_t->GPIO_SDA) != 0);
}

// IIC Start
static void IIC_Start_t(const struct IIC_Type *IIC_Type_t)
{
#if 0
    SDA_OUT(IIC_Type_t); // sda�����
    IIC_SDA(IIC_Type_t, 1);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(4);
    IIC_SDA(IIC_Type_t, 0); // START:when CLK is high,DATA change form high to low
    IIC_Type_t->delay_us(4);
    IIC_SCL(IIC_Type_t, 0); // ǯסI2C���ߣ�׼�����ͻ��������
#endif
    /* Ұ�� */
    SDA_OUT(IIC_Type_t);
    IIC_SDA(IIC_Type_t, 1);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(1);
    IIC_SDA(IIC_Type_t, 0); // START:when CLK is high,DATA change form high to low
    IIC_Type_t->delay_us(1);
    IIC_SCL(IIC_Type_t, 0); // ǯסI2C���ߣ�׼�����ͻ��������
    IIC_Type_t->delay_us(1);
}

// IIC Stop
static void IIC_Stop_t(const struct IIC_Type *IIC_Type_t)
{
#if 0
    SDA_OUT(IIC_Type_t); // sda�����
    IIC_SCL(IIC_Type_t, 0);
    IIC_SDA(IIC_Type_t, 0); // STOP:when CLK is high DATA change form low to high
    IIC_Type_t->delay_us(4);
    IIC_SCL(IIC_Type_t, 1);
    IIC_SDA(IIC_Type_t, 1); // ����I2C���߽����ź�
    IIC_Type_t->delay_us(4);
#else /* Ұ��� */
    SDA_OUT(IIC_Type_t);
    IIC_SDA(IIC_Type_t, 0);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(2);
    IIC_SDA(IIC_Type_t, 1);
#endif
}

// IIC_Wait_ack ����0��ʾwait�ɹ�������1��ʾwaitʧ��
static uint8_t IIC_Wait_Ack_t(const struct IIC_Type *IIC_Type_t) // IIC_Wait_ack,����waitʧ�ܻ��ǳɹ�
{
#if 0
    uint8_t ucErrTime = 0;
    //SDA_IN(IIC_Type_t);      //SDA����Ϊ����
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
    IIC_SCL(IIC_Type_t, 0); // ʱ�����0
    return 0;
#endif
#if 1
    /* Ұ�� */
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
    /* ԭ�� */
    u8 ucErrTime = 0;
    MPU_SDA_IN(); // SDA����Ϊ����
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
    MPU_IIC_SCL = 0; // ʱ�����0
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

// ����ACKӦ��
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
    /* Ұ�� */
    SDA_OUT(IIC_Type_t);
    IIC_SDA(IIC_Type_t, 0);
    IIC_Type_t->delay_us(2);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(2);
    IIC_SCL(IIC_Type_t, 0);
    IIC_Type_t->delay_us(2);
    IIC_SDA(IIC_Type_t, 1);
}

// ����NACKӦ��
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
    /* Ұ�� */
    IIC_SDA(IIC_Type_t, 1);
    IIC_Type_t->delay_us(1);
    IIC_SCL(IIC_Type_t, 1);
    IIC_Type_t->delay_us(1);
    IIC_SCL(IIC_Type_t, 0);
    IIC_Type_t->delay_us(1);
}

// IIC_Send_Byte����ڲ���ΪҪ���͵��ֽ�
static void IIC_Send_Byte_t(const struct IIC_Type *IIC_Type_t, uint8_t txd)
{
#if 0
    uint8_t t = 0;
    SDA_OUT(IIC_Type_t);
    IIC_SCL(IIC_Type_t, 0); // ����ʱ�ӿ�ʼ���ݴ���
    for (t = 0; t < 8; t++)
    {
        IIC_SDA(IIC_Type_t, (txd & 0x80) >> 7);
        txd <<= 1;
        IIC_Type_t->delay_us(2); // ��TEA5767��������ʱ���Ǳ����
        IIC_SCL(IIC_Type_t, 1);
        IIC_Type_t->delay_us(2);
        IIC_SCL(IIC_Type_t, 0);
        IIC_Type_t->delay_us(2);
    }
#endif
    /* Ұ�� */
    uint8_t i;
    SDA_OUT(IIC_Type_t);
    /* �ȷ����ֽڵĸ�λbit7 */
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
            // EEPROM_I2C_SDA_1(); // �ͷ�����
        }
        txd <<= 1; /* ����һ��bit */
        IIC_Type_t->delay_us(2);
    }
}

// IIC_Send_Byte����ڲ���Ϊ�Ƿ�Ҫ����ACK�ź�
static uint8_t IIC_Read_Byte_t(const struct IIC_Type *IIC_Type_t, uint8_t ack)
{
#if 0
    uint8_t i, receive = 0;
    SDA_IN(IIC_Type_t); // SDA����Ϊ����
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
        IIC_Type_t->IIC_NAck(IIC_Type_t); // ����nACK
    else
        IIC_Type_t->IIC_Ack(IIC_Type_t); // ����ACK
    return receive;
#endif
#if 1
    /* Ұ�� */
    uint8_t i;
    uint8_t value;
    SDA_IN(IIC_Type_t); // SDA����Ϊ����
    /* ������1��bitΪ���ݵ�bit7 */
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
