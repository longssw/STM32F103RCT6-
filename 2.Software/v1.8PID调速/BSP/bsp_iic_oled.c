// 2023-1-25 ��������������Init��

#include "bsp_iic_oled.h"
#include "bsp_oled_font.h"
#include <stdarg.h>

static void OLED_WriteCommand_t(const struct OELD_Type *oled_type_t, uint8_t command)
{
    oled_type_t->IIC.IIC_Start(&oled_type_t->IIC);
    oled_type_t->IIC.IIC_Send_Byte(&oled_type_t->IIC, OLED_ADDR); // Slave address,SA0=0
    oled_type_t->IIC.IIC_Wait_Ack(&oled_type_t->IIC);
    oled_type_t->IIC.IIC_Send_Byte(&oled_type_t->IIC, 0x00); // write command
    oled_type_t->IIC.IIC_Wait_Ack(&oled_type_t->IIC);
    oled_type_t->IIC.IIC_Send_Byte(&oled_type_t->IIC, command);
    oled_type_t->IIC.IIC_Wait_Ack(&oled_type_t->IIC);
    oled_type_t->IIC.IIC_Stop(&oled_type_t->IIC);
}

static void OLED_WriteData_t(const struct OELD_Type *oled_type_t, uint8_t data)
{
    oled_type_t->IIC.IIC_Start(&oled_type_t->IIC);
    oled_type_t->IIC.IIC_Send_Byte(&oled_type_t->IIC, OLED_ADDR); // Slave address,SA0=0
    oled_type_t->IIC.IIC_Wait_Ack(&oled_type_t->IIC);
    oled_type_t->IIC.IIC_Send_Byte(&oled_type_t->IIC, 0x40); // write data
    oled_type_t->IIC.IIC_Wait_Ack(&oled_type_t->IIC);
    oled_type_t->IIC.IIC_Send_Byte(&oled_type_t->IIC, data);
    oled_type_t->IIC.IIC_Wait_Ack(&oled_type_t->IIC);
    oled_type_t->IIC.IIC_Stop(&oled_type_t->IIC);
}

static void OLED_WRByte_t(const struct OELD_Type *oled_type_t, uint8_t data, uint8_t command)
{
    if (command)
    {
        /* д���� */
        oled_type_t->OLED_WriteData(oled_type_t, data);
    }
    else
    {
        /* д���� */
        oled_type_t->OLED_WriteCommand(oled_type_t, data);
    }
}

static void OLED_SetPos_t(const struct OELD_Type *oled_type_t, uint8_t x, uint8_t y)
{
    oled_type_t->OLED_WRByte(oled_type_t, 0xb0 + y, OLED_CMD);
    oled_type_t->OLED_WRByte(oled_type_t, ((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    oled_type_t->OLED_WRByte(oled_type_t, (x & 0x0f), OLED_CMD);
}

static void OLED_Init_t(const struct OELD_Type *oled_type_t)
{
    /* ��ʼ��IIC���� */
    oled_type_t->IIC.IIC_Init(&oled_type_t->IIC);
    /* ��ʱ10ms �ȴ�oled��λ��� */
    oled_type_t->IIC.delay_us(10000);

    oled_type_t->OLED_WRByte(oled_type_t, 0xAE, OLED_CMD);            // �ر���ʾ
    oled_type_t->OLED_WRByte(oled_type_t, 0x00, OLED_CMD);            // ������ʾλ�á��е͵�ַ
    oled_type_t->OLED_WRByte(oled_type_t, 0x10, OLED_CMD);            // ������ʾλ�á��иߵ�ַ
    oled_type_t->OLED_WRByte(oled_type_t, 0x40, OLED_CMD);            // ������ʾ��ʼ�� [5:0],����.
    oled_type_t->OLED_WRByte(oled_type_t, 0xB0, OLED_CMD);            //--set page address
    oled_type_t->OLED_WRByte(oled_type_t, 0x81, OLED_CMD);            // �Աȶ�����
    oled_type_t->OLED_WRByte(oled_type_t, OLED_BRIGHTNESS, OLED_CMD); // 1~255;Ĭ��0X7F (��������,Խ��Խ��)
    oled_type_t->OLED_WRByte(oled_type_t, 0xA1, OLED_CMD);            // set segment remap
    oled_type_t->OLED_WRByte(oled_type_t, 0xA6, OLED_CMD);            // ������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
    oled_type_t->OLED_WRByte(oled_type_t, 0xA8, OLED_CMD);            // ��������·��
    oled_type_t->OLED_WRByte(oled_type_t, 0x3F, OLED_CMD);            // Ĭ��0X3F(1/64)
    oled_type_t->OLED_WRByte(oled_type_t, 0xC8, OLED_CMD);            // Com scan direction
    oled_type_t->OLED_WRByte(oled_type_t, 0xD3, OLED_CMD);            // ������ʾƫ��
    oled_type_t->OLED_WRByte(oled_type_t, 0x00, OLED_CMD);            // Ĭ��Ϊ0

    oled_type_t->OLED_WRByte(oled_type_t, 0xD5, OLED_CMD); // ����ʱ�ӷ�Ƶ����,��Ƶ��
    oled_type_t->OLED_WRByte(oled_type_t, 0x80, OLED_CMD); //[3:0],��Ƶ����;[7:4],��Ƶ��

    oled_type_t->OLED_WRByte(oled_type_t, 0xD8, OLED_CMD); // set area color mode off
    oled_type_t->OLED_WRByte(oled_type_t, 0x05, OLED_CMD);

    oled_type_t->OLED_WRByte(oled_type_t, 0xD9, OLED_CMD); // ����Ԥ�������
    oled_type_t->OLED_WRByte(oled_type_t, 0xF1, OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;

    oled_type_t->OLED_WRByte(oled_type_t, 0xDA, OLED_CMD); // set com pin configuartion
    oled_type_t->OLED_WRByte(oled_type_t, 0x12, OLED_CMD); //

    oled_type_t->OLED_WRByte(oled_type_t, 0xDB, OLED_CMD); // set Vcomh
    oled_type_t->OLED_WRByte(oled_type_t, 0x30, OLED_CMD);

    oled_type_t->OLED_WRByte(oled_type_t, 0x8D, OLED_CMD); // ��ɱ�����
    oled_type_t->OLED_WRByte(oled_type_t, 0x14, OLED_CMD); // bit2������/�ر�

    oled_type_t->OLED_WRByte(oled_type_t, 0xAF, OLED_CMD); // ������ʾ
    /* ���� */
    oled_type_t->OLED_Clear(oled_type_t);
}

static void OLED_Clear_t(const struct OELD_Type *oled_type_t)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        oled_type_t->OLED_WRByte(oled_type_t, 0xb0 + i, OLED_CMD); // ����ҳ��ַ��0~7��
        oled_type_t->OLED_WRByte(oled_type_t, 0x00, OLED_CMD);     // ������ʾλ�á��е͵�ַ
        oled_type_t->OLED_WRByte(oled_type_t, 0x10, OLED_CMD);     // ������ʾλ�á��иߵ�ַ
        for (n = 0; n < 128; n++)
            oled_type_t->OLED_WRByte(oled_type_t, 0, OLED_DATA);
    }
}

/*!
 * \brief   ��ʾ��ʾBMPͼƬ128��64��ʼ������(x,y)
 *
 * \param[in] x �ķ�Χ0��127
 * \param[in] y Ϊҳ�ķ�Χ0��7
 */
static void OLED_DrawBMP_t(const struct OELD_Type *oled_type_t,
                           uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
{
    uint32_t j = 0;
    uint8_t x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++)
    {
        oled_type_t->OLED_SetPos(oled_type_t, x0, y);
        for (x = x0; x < x1; x++)
        {
            oled_type_t->OLED_WRByte(oled_type_t, BMP[j++], OLED_DATA);
        }
    }
}

/*!
 * \brief   ��ָ��λ����ʾһ���ַ�,���������ַ�
 *
 * \param[in] x 0~127
 * \param[in] y 0~63
 * \param[in] size ѡ������ 16/12
 */
static void OLED_ShowChar_t(const struct OELD_Type *oled_type_t,
                            uint8_t x, uint8_t y, uint8_t chr, uint8_t char_size)
{
    uint8_t c = 0, i = 0;
    c = chr - ' '; // �õ�ƫ�ƺ��ֵ
    if (x > OLED_MAX_COLUMN - 1)
    {
        x = 0;
        y = y + 2;
    }
    if (char_size == 16)
    {
        oled_type_t->OLED_SetPos(oled_type_t, x, y);

        for (i = 0; i < 8; i++)
            oled_type_t->OLED_WRByte(oled_type_t, F8X16[c * 16 + i], OLED_DATA);

        oled_type_t->OLED_SetPos(oled_type_t, x, y + 1);
        for (i = 0; i < 8; i++)
            oled_type_t->OLED_WRByte(oled_type_t, F8X16[c * 16 + i + 8], OLED_DATA);
    }
    else
    {
        oled_type_t->OLED_SetPos(oled_type_t, x, y);

        for (i = 0; i < 6; i++)
            oled_type_t->OLED_WRByte(oled_type_t, F6x8[c][i], OLED_DATA);
    }
}

// ��ʾһ���ַ��Ŵ�
static void OLED_ShowString_t(const struct OELD_Type *oled_type_t,
                              uint8_t x, uint8_t y, uint8_t *chr, uint8_t char_size)
{
    while (*chr != '\0')
    {
        oled_type_t->OLED_ShowChar(oled_type_t, x, y, *chr, char_size);
        x += 8;
        if (x > 120)
        {
            x = 0;
            y += 2;
        }
        chr++;
    }
}

/*!
 * \brief   ��ʾ����
 *
 * \param[in] x 0-112 ��16�ݽ�
 * \param[in] y 0-6 ��2�ݽ�
 * \param[in] id Hzk[]�е��ֵ�ID
 */
static void OELD_ShowCHinese_t(const struct OELD_Type *oled_type_t,
                               uint8_t x, uint8_t y, uint8_t id)
{
    uint8_t t, adder = 0;
    oled_type_t->OLED_SetPos(oled_type_t, x, y);
    for (t = 0; t < 16; t++)
    {
        oled_type_t->OLED_WRByte(oled_type_t, Hzk[2 * id][t], OLED_DATA);
        adder += 1;
    }
    oled_type_t->OLED_SetPos(oled_type_t, x, y + 1);
    for (t = 0; t < 16; t++)
    {
        oled_type_t->OLED_WRByte(oled_type_t, Hzk[2 * id + 1][t], OLED_DATA);
        adder += 1;
    }
}

/*!
 * \brief   OLEDר��print����
 *
 *          [1]ʹ�õķ�ʽ��printf����.
 *          [2]��Ϊʹ�õ���ģΪ F8X16 ��Ϊ8 ��Ϊ16 ������128*64����Ļ��ֻ����ʾ4��16��.
 *          [3]��һ����ʾ��ʱ���Զ�����.
 *          [4]������Ĳ���������Χʱ���˳�.
 *          [5]��֧��������ʾ
 *
 * \param[in] row �� 0~15
 * \param[in] col �� 0~3
 * \param[out] return 0 ���� 1 ��������
 */
static uint8_t OLED_Print_t(const struct OELD_Type *oled_type_t,
                            uint8_t row, uint8_t col, const char *fmt, ...)
{
    static uint8_t LCD_BUF[128] = {0};
    static va_list ap;
    uint8_t remain_size = 0;

    if ((row > 16) || (col > 4))
    {
        return 1;
    }

    /* �����ֿ��С�޸� */
    row = row * 8;
    col = col * 2;

    va_start(ap, fmt);

    vsprintf((char *)LCD_BUF, fmt, ap);

    va_end(ap);

    remain_size = 21 - col;

    LCD_BUF[remain_size] = '\0';

    oled_type_t->OLED_ShowString(oled_type_t, row, col, LCD_BUF, 16);

    return 0;
}


void oled_init(struct OELD_Type *oled_type_t)
{
    /* ʵ����IIC���� */
    create_iic(&oled_type_t->IIC);
    
    oled_type_t->OLED_WriteCommand = OLED_WriteCommand_t;
    oled_type_t->OLED_WriteData = OLED_WriteData_t;
    oled_type_t->OLED_WRByte = OLED_WRByte_t;
    oled_type_t->OLED_SetPos = OLED_SetPos_t;
    oled_type_t->OLED_Init = OLED_Init_t;
    oled_type_t->OLED_Clear = OLED_Clear_t;
    oled_type_t->OLED_DrawBMP = OLED_DrawBMP_t;
    oled_type_t->OLED_ShowChar = OLED_ShowChar_t;
    oled_type_t->OLED_ShowString = OLED_ShowString_t;
    oled_type_t->OELD_ShowCHinese = OELD_ShowCHinese_t;
    oled_type_t->OLED_Print = OLED_Print_t;
    
    /* ��ʼ��OELD */
    oled_type_t->OLED_Init(oled_type_t);
}

#ifdef BSP_IIC_OLED_UNITEST
/*!
 * \brief   ��Ԫ���Գ���
 *
 * \param[in] id ѡ���������
 *               [TEST_PICTURE]:����ͼƬ
 *               [TEST_PRINT]:����OLED_Print����
 *               [TEST_CHINESE]:������ʾ����
 */

void bsp_iic_oled_unitest(OLED_TEST_ID_T id)
{
    OELD_TypeDef UNITEST_OLED_096;
    float kp = 0.0345;

    /* ��ʼ��OLEDʹ�õ�Ӳ����Դ */
    UNITEST_OLED_096.IIC.GPIOx_SCL = GPIOA;
    UNITEST_OLED_096.IIC.GPIOx_SDA = GPIOA;
    UNITEST_OLED_096.IIC.GPIO_SCL = GPIO_Pin_5;
    UNITEST_OLED_096.IIC.GPIO_SDA = GPIO_Pin_6;
    //UNITEST_OLED_096.IIC.delay_us = delay_us;

    /* ʵ����OLED���� */
    oled_init(&UNITEST_OLED_096);

    /* ��ʼ��OLED */
    UNITEST_OLED_096.OLED_Init(&UNITEST_OLED_096);

    switch (id)
    {
    case TEST_PICTURE:
        /* ��ʾͼƬ */
        UNITEST_OLED_096.OLED_DrawBMP(&UNITEST_OLED_096, 28, 0, 92, 7, school_badge);
        break;
    case TEST_PRINT:
        /* ��ʾӢ�� */
        UNITEST_OLED_096.OLED_Print(&UNITEST_OLED_096, 0, 0, "OLED Unitest");
        /* ��ʾ������ */
        UNITEST_OLED_096.OLED_Print(&UNITEST_OLED_096, 0, 1, "kp = [%f]", kp);
        /* ���Գ������Զ����� */
        UNITEST_OLED_096.OLED_Print(&UNITEST_OLED_096, 0, 2, "123456789ABCDEFGHIJK");
        break;
    case TEST_CHINESE:
        /* ��ʾ���� */
        UNITEST_OLED_096.OELD_ShowCHinese(&UNITEST_OLED_096, 0, 0, 14);  /* �� */
        UNITEST_OLED_096.OELD_ShowCHinese(&UNITEST_OLED_096, 16, 2, 15); /* Ŀ */
        break;
    }
}
#endif /* BSP_IIC_OLED_UNITEST */
