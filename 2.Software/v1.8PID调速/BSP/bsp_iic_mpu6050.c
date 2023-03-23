#include "bsp_iic_mpu6050.h"
mpu6050_t MPU6050;

/*!
 * \brief   为了使用DMP库而做的适配器
 */
u8 dmp_iic_write(u8 addr, u8 reg, u8 len, u8 *buf)
{
    return MPU6050.MPU_WriteLen(&MPU6050, addr, reg, len, buf);
}

u8 dmp_iic_read(u8 addr, u8 reg, u8 len, u8 *buf)
{
    return MPU6050.MPU_ReadLen(&MPU6050, addr, reg, len, buf);
}

/*!
 * \brief   向指定寄存器读取一个字节
 *
 * \param[in] reg 寄存器地址
 * \param[out] return 读到的数据
 */
uint8_t MPU_ReadByte_t(const struct mpu6050 *mpuxx_type_t, uint8_t reg)
{
    uint8_t data;
    /* 发送起始信号 */
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    /* 发送器件地址+写命令 */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (MPU_ADDR << 1) | 0);
    /* 等待应答 */
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    /* 写寄存器地址 */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, reg);
    /* 等待应答 */
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    /* 发送起始信号 */
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    /* 发送器件地址+读命令 */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (MPU_ADDR << 1) | 1);
    /* 等待应答 */
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    /* 读取数据 发送NACK*/
    data = mpuxx_type_t->IIC.IIC_Read_Byte(&mpuxx_type_t->IIC, 0);
    mpuxx_type_t->IIC.IIC_NAck(&mpuxx_type_t->IIC);
    /* 发送停止信号 */
    mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);

    return data;
}

/*!
 * \brief   向指定寄存器写入一个字节
 *
 * \param[in] reg 寄存器地址
 * \param[in] data 写入数据
 * \param[out] return 0 正常 1 错误
 */
uint8_t MPU_WriteByte_t(const struct mpu6050 *mpuxx_type_t, uint8_t reg, uint8_t data)
{
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (MPU_ADDR << 1) | 0);

    /* 等待应答 */
    if (mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC))
    {
        mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
        return 1;
    }
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, reg);
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    /* 发送数据 */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, data);
    /* 等待应答 */
    if (!mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC))
    {
        mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
        return 1;
    }
    mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);

    return 0;
}

/*!
 * \brief   连续读寄存器中的值
 *
 * \param[in] addr 器件地址
 * \param[in] reg  要读取的寄存器地址
 * \param[in] len  要读取的长度
 * \param[in] buf  读取到的数据存储区
 * \param[out] return 0 正常 1 错误
 */
uint8_t MPU_ReadLen_t(const struct mpu6050 *mpuxx_type_t,
                      uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
#if 0
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    /* 发送器件地址+写命令 */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (MPU_ADDR << 1) | 0);

    /* 等待应答 */
    if (mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC))
    {
        mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
        return 1;
    }
    /* 写寄存器地址 */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, reg);
    /* 等待应答 */
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);

    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    /* 发送器件地址+读命令 */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (addr << 1) | 1);
    /* 等待应答 */
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);

    while (len)
    {
        if (len == 1)
        {
            /* 读数据,发送nACK  */
            mpuxx_type_t->IIC.IIC_Read_Byte(&mpuxx_type_t->IIC, 0);
            mpuxx_type_t->IIC.IIC_NAck(&mpuxx_type_t->IIC);
        }

        //*buf = MPU_IIC_Read_Byte(0); // 读数据,发送nACK
        else
        {
            /* 读数据,发送ACK  */
            mpuxx_type_t->IIC.IIC_Read_Byte(&mpuxx_type_t->IIC, 1);
            mpuxx_type_t->IIC.IIC_Ack(&mpuxx_type_t->IIC);
        }

        //*buf = MPU_IIC_Read_Byte(1); // 读数据,发送ACK
        len--;
        buf++;
    }
    /* 发送停止信号 */
    mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);

    return 0;
#else
    // MPU_IIC_Start();
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    // MPU_IIC_Send_Byte((addr << 1) | 0); // 发送器件地址+写命令
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (MPU_ADDR << 1) | 0);
    // if (MPU_IIC_Wait_Ack())             // 等待应答
    //  {
    //      MPU_IIC_Stop();
    //      return 1;
    //  }
    if (mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC))
    {
        mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
        // MPU_IIC_Stop();
        return 1;
    }

    // MPU_IIC_Send_Byte(reg); // 写寄存器地址
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, reg);
    // mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, reg);
    // MPU_IIC_Wait_Ack(); // 等待应答
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    // mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    // MPU_IIC_Start();
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    // MPU_IIC_Send_Byte((addr << 1) | 1); // 发送器件地址+读命令
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (addr << 1) | 1);
    // MPU_IIC_Wait_Ack();                 // 等待应答
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    while (len)
    {
        if (len == 1)
        {
            //*buf = MPU_IIC_Read_Byte(0); // 读数据,发送nACK
            *buf = mpuxx_type_t->IIC.IIC_Read_Byte(&mpuxx_type_t->IIC, 0);
            mpuxx_type_t->IIC.IIC_NAck(&mpuxx_type_t->IIC);
        }

        else
        {
            //*buf = MPU_IIC_Read_Byte(1); // 读数据,发送ACK
            *buf = mpuxx_type_t->IIC.IIC_Read_Byte(&mpuxx_type_t->IIC, 1);
            mpuxx_type_t->IIC.IIC_Ack(&mpuxx_type_t->IIC);
            // MPU_IIC_Ack();
        }

        len--;
        buf++;
    }
    // MPU_IIC_Stop(); // 产生一个停止条件
    mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
    return 0;
#endif
}

/*!
 * \brief   将值连续写入寄存器
 *
 * \param[in] addr 器件地址
 * \param[in] reg  要写入的寄存器地址
 * \param[in] len  要写入的长度
 * \param[in] buf  数据存储区
 * \param[out] return 0 正常 1 错误
 */
uint8_t MPU_WriteLen_t(const struct mpu6050 *mpuxx_type_t,
                       uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    uint8_t i;
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (MPU_ADDR << 1) | 0);

    /* 等待应答 */
    if (mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC))
    {
        mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
        return 1;
    }
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, reg);
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);

    for (i = 0; i < len; i++)
    {
        mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, buf[i]); // 发送数据
        if (mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC))      // 等待ACK
        {
            mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
            return 1;
        }
    }
    mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);

    return 0;
}

/*!
 * \brief   设置陀螺仪满量程范围
 *
 * \param[in] fsr [0]: ±250dps [1]: ±500dps [2]: ±1000dps [3]: ±2000dps
 * \param[out] 0 设置成功
 * \param[out] 1 设置失败
 */
uint8_t MPU_SetGyroFsr_t(const struct mpu6050 *mpuxx_type_t, uint8_t fsr)
{
    return mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_GYRO_CFG_REG, fsr << 3);
}

/*!
 * \brief   设置加速度传感器满量程范围
 *
 * \param[in] fsr [0]: ±2g [1]: ±4g [2]: ±8g [3]: ±16g
 * \param[out] 0 设置成功
 * \param[out] 1 设置失败
 */
uint8_t MPU_SetAccelFsr_t(const struct mpu6050 *mpuxx_type_t, uint8_t fsr)
{
    return mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_ACCEL_CFG_REG, fsr << 3);
}

/*!
 * \brief   设置数字低通滤波器
 *
 * \param[in] lpf 数字低通滤波频率(Hz)
 * \param[out] 0 设置成功
 * \param[out] 1 设置失败
 */
uint8_t MPU_SetLPF_t(const struct mpu6050 *mpuxx_type_t, uint16_t lpf)
{
    uint8_t data = 0;
    if (lpf >= 188)
        data = 1;
    else if (lpf >= 98)
        data = 2;
    else if (lpf >= 42)
        data = 3;
    else if (lpf >= 20)
        data = 4;
    else if (lpf >= 10)
        data = 5;
    else
        data = 6;
    return mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_CFG_REG, data);
}

/*!
 * \brief   设置采样率(假定Fs=1KHz)
 *
 * \param[in] rate 4~1000(Hz)
 * \param[out] 0 设置成功
 * \param[out] 1 设置失败
 */
uint8_t MPU_SetRate_t(const struct mpu6050 *mpuxx_type_t, uint16_t rate)
{
    uint8_t data = 0;
    if (rate > 1000)
        rate = 1000;
    if (rate < 4)
        rate = 4;
    data = 1000 / rate - 1;
    /* 设置数字低通滤波器 */
    data = mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_SAMPLE_RATE_REG, data);
    /* 自动设置LPF为采样率的一半 */
    return mpuxx_type_t->MPU_SetLPF(mpuxx_type_t, rate / 2);
}

uint8_t MPU_Init_t(const struct mpu6050 *mpuxx_type_t)
{
    uint8_t mpu_id;

    /* 初始化IIC总线 */
    mpuxx_type_t->IIC.IIC_Init(&mpuxx_type_t->IIC);
    /* 复位MPU6050 */
    mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_PWR_MGMT1_REG, 0X80);
    /* 延时等待复位完成 */
    mpuxx_type_t->IIC.delay_us(100000);
    /* 唤醒MPU6050 */
    mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_PWR_MGMT1_REG, 0X00);
    /* 设置陀螺仪量程 ±2000dps */
    mpuxx_type_t->MPU_SetGyroFsr(mpuxx_type_t, 3);
    /* 设置加速度量程 ±8g */
    mpuxx_type_t->MPU_SetAccelFsr(mpuxx_type_t, 2);
    /* 设置采样率50Hz */
    mpuxx_type_t->MPU_SetRate(mpuxx_type_t, 200);
    /* 关闭所有中断 */
    mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_INT_EN_REG, 0X00);
    /* I2C主模式关闭 */
    mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_USER_CTRL_REG, 0X00);
    /* 关闭FIFO */
    mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_FIFO_EN_REG, 0X00);
    /* INT引脚低电平有效 */
    mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_INTBP_CFG_REG, 0X80);
    /* 读取器件ID */
    mpu_id = mpuxx_type_t->MPU_ReadByte(mpuxx_type_t, MPU_DEVICE_ID_REG);

    /* 器件ID正确 */
    if (mpu_id == MPU_ADDR)
    {
        /* 设置CLKSEL,PLL X轴为参考 */
        mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_PWR_MGMT1_REG, 0X01);
        /* 加速度与陀螺仪都工作 */
        mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_PWR_MGMT2_REG, 0X00);
        /* 设置采样率为50Hz ps:此处代码存疑 */
        mpuxx_type_t->MPU_SetRate(mpuxx_type_t, 100);
    }
    return mpu_id;
}

uint8_t MPU_GetGyroscope_t(const struct mpu6050 *mpuxx_type_t, short *gx, short *gy, short *gz)
{
    uint8_t buf[6], res;
    res = mpuxx_type_t->MPU_ReadLen(mpuxx_type_t, MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
    if (res == 0)
    {
        *gx = ((u16)buf[0] << 8) | buf[1];
        *gy = ((u16)buf[2] << 8) | buf[3];
        *gz = ((u16)buf[4] << 8) | buf[5];
    }
    return res;
}

uint8_t read_accel(const struct mpu6050 *mpuxx_type_t, short *ax, short *ay, short *az)
{
    uint8_t buf[6], res;
    res = mpuxx_type_t->MPU_ReadLen(mpuxx_type_t, MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
    if (res == 0)
    {
        *ax = ((u16)buf[0] << 8) | buf[1];
        *ay = ((u16)buf[2] << 8) | buf[3];
        *az = ((u16)buf[4] << 8) | buf[5];
    }
    return res;
}

uint8_t mpu6050_init(struct mpu6050 *mpuxx_type_t)
{
    create_iic(&mpuxx_type_t->IIC);

    mpuxx_type_t->MPU_ReadByte = MPU_ReadByte_t;
    mpuxx_type_t->MPU_WriteByte = MPU_WriteByte_t;
    mpuxx_type_t->MPU_ReadLen = MPU_ReadLen_t;
    mpuxx_type_t->MPU_WriteLen = MPU_WriteLen_t;

    mpuxx_type_t->MPU_SetGyroFsr = MPU_SetGyroFsr_t;
    mpuxx_type_t->MPU_SetAccelFsr = MPU_SetAccelFsr_t;
    mpuxx_type_t->MPU_SetLPF = MPU_SetLPF_t;
    mpuxx_type_t->MPU_SetRate = MPU_SetRate_t;

    mpuxx_type_t->init = MPU_Init_t;
    mpuxx_type_t->read_gyro = MPU_GetGyroscope_t;
    mpuxx_type_t->read_accel = read_accel;
    /* 初始化陀螺仪 */
    return mpuxx_type_t->init(mpuxx_type_t);
}
