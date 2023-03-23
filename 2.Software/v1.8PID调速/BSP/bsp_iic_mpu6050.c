#include "bsp_iic_mpu6050.h"
mpu6050_t MPU6050;

/*!
 * \brief   Ϊ��ʹ��DMP�������������
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
 * \brief   ��ָ���Ĵ�����ȡһ���ֽ�
 *
 * \param[in] reg �Ĵ�����ַ
 * \param[out] return ����������
 */
uint8_t MPU_ReadByte_t(const struct mpu6050 *mpuxx_type_t, uint8_t reg)
{
    uint8_t data;
    /* ������ʼ�ź� */
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    /* ����������ַ+д���� */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (MPU_ADDR << 1) | 0);
    /* �ȴ�Ӧ�� */
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    /* д�Ĵ�����ַ */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, reg);
    /* �ȴ�Ӧ�� */
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    /* ������ʼ�ź� */
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    /* ����������ַ+������ */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (MPU_ADDR << 1) | 1);
    /* �ȴ�Ӧ�� */
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    /* ��ȡ���� ����NACK*/
    data = mpuxx_type_t->IIC.IIC_Read_Byte(&mpuxx_type_t->IIC, 0);
    mpuxx_type_t->IIC.IIC_NAck(&mpuxx_type_t->IIC);
    /* ����ֹͣ�ź� */
    mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);

    return data;
}

/*!
 * \brief   ��ָ���Ĵ���д��һ���ֽ�
 *
 * \param[in] reg �Ĵ�����ַ
 * \param[in] data д������
 * \param[out] return 0 ���� 1 ����
 */
uint8_t MPU_WriteByte_t(const struct mpu6050 *mpuxx_type_t, uint8_t reg, uint8_t data)
{
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (MPU_ADDR << 1) | 0);

    /* �ȴ�Ӧ�� */
    if (mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC))
    {
        mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
        return 1;
    }
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, reg);
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    /* �������� */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, data);
    /* �ȴ�Ӧ�� */
    if (!mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC))
    {
        mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
        return 1;
    }
    mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);

    return 0;
}

/*!
 * \brief   �������Ĵ����е�ֵ
 *
 * \param[in] addr ������ַ
 * \param[in] reg  Ҫ��ȡ�ļĴ�����ַ
 * \param[in] len  Ҫ��ȡ�ĳ���
 * \param[in] buf  ��ȡ�������ݴ洢��
 * \param[out] return 0 ���� 1 ����
 */
uint8_t MPU_ReadLen_t(const struct mpu6050 *mpuxx_type_t,
                      uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
#if 0
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    /* ����������ַ+д���� */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (MPU_ADDR << 1) | 0);

    /* �ȴ�Ӧ�� */
    if (mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC))
    {
        mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
        return 1;
    }
    /* д�Ĵ�����ַ */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, reg);
    /* �ȴ�Ӧ�� */
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);

    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    /* ����������ַ+������ */
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (addr << 1) | 1);
    /* �ȴ�Ӧ�� */
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);

    while (len)
    {
        if (len == 1)
        {
            /* ������,����nACK  */
            mpuxx_type_t->IIC.IIC_Read_Byte(&mpuxx_type_t->IIC, 0);
            mpuxx_type_t->IIC.IIC_NAck(&mpuxx_type_t->IIC);
        }

        //*buf = MPU_IIC_Read_Byte(0); // ������,����nACK
        else
        {
            /* ������,����ACK  */
            mpuxx_type_t->IIC.IIC_Read_Byte(&mpuxx_type_t->IIC, 1);
            mpuxx_type_t->IIC.IIC_Ack(&mpuxx_type_t->IIC);
        }

        //*buf = MPU_IIC_Read_Byte(1); // ������,����ACK
        len--;
        buf++;
    }
    /* ����ֹͣ�ź� */
    mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);

    return 0;
#else
    // MPU_IIC_Start();
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    // MPU_IIC_Send_Byte((addr << 1) | 0); // ����������ַ+д����
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (MPU_ADDR << 1) | 0);
    // if (MPU_IIC_Wait_Ack())             // �ȴ�Ӧ��
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

    // MPU_IIC_Send_Byte(reg); // д�Ĵ�����ַ
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, reg);
    // mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, reg);
    // MPU_IIC_Wait_Ack(); // �ȴ�Ӧ��
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    // mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    // MPU_IIC_Start();
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    // MPU_IIC_Send_Byte((addr << 1) | 1); // ����������ַ+������
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (addr << 1) | 1);
    // MPU_IIC_Wait_Ack();                 // �ȴ�Ӧ��
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);
    while (len)
    {
        if (len == 1)
        {
            //*buf = MPU_IIC_Read_Byte(0); // ������,����nACK
            *buf = mpuxx_type_t->IIC.IIC_Read_Byte(&mpuxx_type_t->IIC, 0);
            mpuxx_type_t->IIC.IIC_NAck(&mpuxx_type_t->IIC);
        }

        else
        {
            //*buf = MPU_IIC_Read_Byte(1); // ������,����ACK
            *buf = mpuxx_type_t->IIC.IIC_Read_Byte(&mpuxx_type_t->IIC, 1);
            mpuxx_type_t->IIC.IIC_Ack(&mpuxx_type_t->IIC);
            // MPU_IIC_Ack();
        }

        len--;
        buf++;
    }
    // MPU_IIC_Stop(); // ����һ��ֹͣ����
    mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
    return 0;
#endif
}

/*!
 * \brief   ��ֵ����д��Ĵ���
 *
 * \param[in] addr ������ַ
 * \param[in] reg  Ҫд��ļĴ�����ַ
 * \param[in] len  Ҫд��ĳ���
 * \param[in] buf  ���ݴ洢��
 * \param[out] return 0 ���� 1 ����
 */
uint8_t MPU_WriteLen_t(const struct mpu6050 *mpuxx_type_t,
                       uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    uint8_t i;
    mpuxx_type_t->IIC.IIC_Start(&mpuxx_type_t->IIC);
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, (MPU_ADDR << 1) | 0);

    /* �ȴ�Ӧ�� */
    if (mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC))
    {
        mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
        return 1;
    }
    mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, reg);
    mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC);

    for (i = 0; i < len; i++)
    {
        mpuxx_type_t->IIC.IIC_Send_Byte(&mpuxx_type_t->IIC, buf[i]); // ��������
        if (mpuxx_type_t->IIC.IIC_Wait_Ack(&mpuxx_type_t->IIC))      // �ȴ�ACK
        {
            mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);
            return 1;
        }
    }
    mpuxx_type_t->IIC.IIC_Stop(&mpuxx_type_t->IIC);

    return 0;
}

/*!
 * \brief   ���������������̷�Χ
 *
 * \param[in] fsr [0]: ��250dps [1]: ��500dps [2]: ��1000dps [3]: ��2000dps
 * \param[out] 0 ���óɹ�
 * \param[out] 1 ����ʧ��
 */
uint8_t MPU_SetGyroFsr_t(const struct mpu6050 *mpuxx_type_t, uint8_t fsr)
{
    return mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_GYRO_CFG_REG, fsr << 3);
}

/*!
 * \brief   ���ü��ٶȴ����������̷�Χ
 *
 * \param[in] fsr [0]: ��2g [1]: ��4g [2]: ��8g [3]: ��16g
 * \param[out] 0 ���óɹ�
 * \param[out] 1 ����ʧ��
 */
uint8_t MPU_SetAccelFsr_t(const struct mpu6050 *mpuxx_type_t, uint8_t fsr)
{
    return mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_ACCEL_CFG_REG, fsr << 3);
}

/*!
 * \brief   �������ֵ�ͨ�˲���
 *
 * \param[in] lpf ���ֵ�ͨ�˲�Ƶ��(Hz)
 * \param[out] 0 ���óɹ�
 * \param[out] 1 ����ʧ��
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
 * \brief   ���ò�����(�ٶ�Fs=1KHz)
 *
 * \param[in] rate 4~1000(Hz)
 * \param[out] 0 ���óɹ�
 * \param[out] 1 ����ʧ��
 */
uint8_t MPU_SetRate_t(const struct mpu6050 *mpuxx_type_t, uint16_t rate)
{
    uint8_t data = 0;
    if (rate > 1000)
        rate = 1000;
    if (rate < 4)
        rate = 4;
    data = 1000 / rate - 1;
    /* �������ֵ�ͨ�˲��� */
    data = mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_SAMPLE_RATE_REG, data);
    /* �Զ�����LPFΪ�����ʵ�һ�� */
    return mpuxx_type_t->MPU_SetLPF(mpuxx_type_t, rate / 2);
}

uint8_t MPU_Init_t(const struct mpu6050 *mpuxx_type_t)
{
    uint8_t mpu_id;

    /* ��ʼ��IIC���� */
    mpuxx_type_t->IIC.IIC_Init(&mpuxx_type_t->IIC);
    /* ��λMPU6050 */
    mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_PWR_MGMT1_REG, 0X80);
    /* ��ʱ�ȴ���λ��� */
    mpuxx_type_t->IIC.delay_us(100000);
    /* ����MPU6050 */
    mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_PWR_MGMT1_REG, 0X00);
    /* �������������� ��2000dps */
    mpuxx_type_t->MPU_SetGyroFsr(mpuxx_type_t, 3);
    /* ���ü��ٶ����� ��8g */
    mpuxx_type_t->MPU_SetAccelFsr(mpuxx_type_t, 2);
    /* ���ò�����50Hz */
    mpuxx_type_t->MPU_SetRate(mpuxx_type_t, 200);
    /* �ر������ж� */
    mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_INT_EN_REG, 0X00);
    /* I2C��ģʽ�ر� */
    mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_USER_CTRL_REG, 0X00);
    /* �ر�FIFO */
    mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_FIFO_EN_REG, 0X00);
    /* INT���ŵ͵�ƽ��Ч */
    mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_INTBP_CFG_REG, 0X80);
    /* ��ȡ����ID */
    mpu_id = mpuxx_type_t->MPU_ReadByte(mpuxx_type_t, MPU_DEVICE_ID_REG);

    /* ����ID��ȷ */
    if (mpu_id == MPU_ADDR)
    {
        /* ����CLKSEL,PLL X��Ϊ�ο� */
        mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_PWR_MGMT1_REG, 0X01);
        /* ���ٶ��������Ƕ����� */
        mpuxx_type_t->MPU_WriteByte(mpuxx_type_t, MPU_PWR_MGMT2_REG, 0X00);
        /* ���ò�����Ϊ50Hz ps:�˴�������� */
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
    /* ��ʼ�������� */
    return mpuxx_type_t->init(mpuxx_type_t);
}
