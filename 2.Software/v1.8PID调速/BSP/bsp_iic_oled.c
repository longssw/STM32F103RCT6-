// 2023-1-25 将清屏函数加入Init中

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
        /* 写数据 */
        oled_type_t->OLED_WriteData(oled_type_t, data);
    }
    else
    {
        /* 写命令 */
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
    /* 初始化IIC操作 */
    oled_type_t->IIC.IIC_Init(&oled_type_t->IIC);
    /* 延时10ms 等待oled复位完成 */
    oled_type_t->IIC.delay_us(10000);

    oled_type_t->OLED_WRByte(oled_type_t, 0xAE, OLED_CMD);            // 关闭显示
    oled_type_t->OLED_WRByte(oled_type_t, 0x00, OLED_CMD);            // 设置显示位置—列低地址
    oled_type_t->OLED_WRByte(oled_type_t, 0x10, OLED_CMD);            // 设置显示位置—列高地址
    oled_type_t->OLED_WRByte(oled_type_t, 0x40, OLED_CMD);            // 设置显示开始行 [5:0],行数.
    oled_type_t->OLED_WRByte(oled_type_t, 0xB0, OLED_CMD);            //--set page address
    oled_type_t->OLED_WRByte(oled_type_t, 0x81, OLED_CMD);            // 对比度设置
    oled_type_t->OLED_WRByte(oled_type_t, OLED_BRIGHTNESS, OLED_CMD); // 1~255;默认0X7F (亮度设置,越大越亮)
    oled_type_t->OLED_WRByte(oled_type_t, 0xA1, OLED_CMD);            // set segment remap
    oled_type_t->OLED_WRByte(oled_type_t, 0xA6, OLED_CMD);            // 设置显示方式;bit0:1,反相显示;0,正常显示
    oled_type_t->OLED_WRByte(oled_type_t, 0xA8, OLED_CMD);            // 设置驱动路数
    oled_type_t->OLED_WRByte(oled_type_t, 0x3F, OLED_CMD);            // 默认0X3F(1/64)
    oled_type_t->OLED_WRByte(oled_type_t, 0xC8, OLED_CMD);            // Com scan direction
    oled_type_t->OLED_WRByte(oled_type_t, 0xD3, OLED_CMD);            // 设置显示偏移
    oled_type_t->OLED_WRByte(oled_type_t, 0x00, OLED_CMD);            // 默认为0

    oled_type_t->OLED_WRByte(oled_type_t, 0xD5, OLED_CMD); // 设置时钟分频因子,震荡频率
    oled_type_t->OLED_WRByte(oled_type_t, 0x80, OLED_CMD); //[3:0],分频因子;[7:4],震荡频率

    oled_type_t->OLED_WRByte(oled_type_t, 0xD8, OLED_CMD); // set area color mode off
    oled_type_t->OLED_WRByte(oled_type_t, 0x05, OLED_CMD);

    oled_type_t->OLED_WRByte(oled_type_t, 0xD9, OLED_CMD); // 设置预充电周期
    oled_type_t->OLED_WRByte(oled_type_t, 0xF1, OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;

    oled_type_t->OLED_WRByte(oled_type_t, 0xDA, OLED_CMD); // set com pin configuartion
    oled_type_t->OLED_WRByte(oled_type_t, 0x12, OLED_CMD); //

    oled_type_t->OLED_WRByte(oled_type_t, 0xDB, OLED_CMD); // set Vcomh
    oled_type_t->OLED_WRByte(oled_type_t, 0x30, OLED_CMD);

    oled_type_t->OLED_WRByte(oled_type_t, 0x8D, OLED_CMD); // 电荷泵设置
    oled_type_t->OLED_WRByte(oled_type_t, 0x14, OLED_CMD); // bit2，开启/关闭

    oled_type_t->OLED_WRByte(oled_type_t, 0xAF, OLED_CMD); // 开启显示
    /* 清屏 */
    oled_type_t->OLED_Clear(oled_type_t);
}

static void OLED_Clear_t(const struct OELD_Type *oled_type_t)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        oled_type_t->OLED_WRByte(oled_type_t, 0xb0 + i, OLED_CMD); // 设置页地址（0~7）
        oled_type_t->OLED_WRByte(oled_type_t, 0x00, OLED_CMD);     // 设置显示位置—列低地址
        oled_type_t->OLED_WRByte(oled_type_t, 0x10, OLED_CMD);     // 设置显示位置—列高地址
        for (n = 0; n < 128; n++)
            oled_type_t->OLED_WRByte(oled_type_t, 0, OLED_DATA);
    }
}

/*!
 * \brief   显示显示BMP图片128×64起始点坐标(x,y)
 *
 * \param[in] x 的范围0～127
 * \param[in] y 为页的范围0～7
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
 * \brief   在指定位置显示一个字符,包括部分字符
 *
 * \param[in] x 0~127
 * \param[in] y 0~63
 * \param[in] size 选择字体 16/12
 */
static void OLED_ShowChar_t(const struct OELD_Type *oled_type_t,
                            uint8_t x, uint8_t y, uint8_t chr, uint8_t char_size)
{
    uint8_t c = 0, i = 0;
    c = chr - ' '; // 得到偏移后的值
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

// 显示一个字符号串
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
 * \brief   显示汉字
 *
 * \param[in] x 0-112 以16递进
 * \param[in] y 0-6 以2递进
 * \param[in] id Hzk[]中的字的ID
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
 * \brief   OLED专用print函数
 *
 *          [1]使用的方式和printf类似.
 *          [2]因为使用的字模为 F8X16 宽为8 高为16 所以在128*64的屏幕上只能显示4行16列.
 *          [3]当一行显示满时会自动换行.
 *          [4]当输入的参数超过范围时会退出.
 *          [5]不支持中文显示
 *
 * \param[in] row 行 0~15
 * \param[in] col 列 0~3
 * \param[out] return 0 正常 1 参数错误
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

    /* 根据字库大小修改 */
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
    /* 实例化IIC驱动 */
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
    
    /* 初始化OELD */
    oled_type_t->OLED_Init(oled_type_t);
}

#ifdef BSP_IIC_OLED_UNITEST
/*!
 * \brief   单元测试程序
 *
 * \param[in] id 选择测试内容
 *               [TEST_PICTURE]:测试图片
 *               [TEST_PRINT]:测试OLED_Print函数
 *               [TEST_CHINESE]:测试显示中文
 */

void bsp_iic_oled_unitest(OLED_TEST_ID_T id)
{
    OELD_TypeDef UNITEST_OLED_096;
    float kp = 0.0345;

    /* 初始化OLED使用的硬件资源 */
    UNITEST_OLED_096.IIC.GPIOx_SCL = GPIOA;
    UNITEST_OLED_096.IIC.GPIOx_SDA = GPIOA;
    UNITEST_OLED_096.IIC.GPIO_SCL = GPIO_Pin_5;
    UNITEST_OLED_096.IIC.GPIO_SDA = GPIO_Pin_6;
    //UNITEST_OLED_096.IIC.delay_us = delay_us;

    /* 实例化OLED对象 */
    oled_init(&UNITEST_OLED_096);

    /* 初始化OLED */
    UNITEST_OLED_096.OLED_Init(&UNITEST_OLED_096);

    switch (id)
    {
    case TEST_PICTURE:
        /* 显示图片 */
        UNITEST_OLED_096.OLED_DrawBMP(&UNITEST_OLED_096, 28, 0, 92, 7, school_badge);
        break;
    case TEST_PRINT:
        /* 显示英文 */
        UNITEST_OLED_096.OLED_Print(&UNITEST_OLED_096, 0, 0, "OLED Unitest");
        /* 显示浮点数 */
        UNITEST_OLED_096.OLED_Print(&UNITEST_OLED_096, 0, 1, "kp = [%f]", kp);
        /* 测试超行数自动换行 */
        UNITEST_OLED_096.OLED_Print(&UNITEST_OLED_096, 0, 2, "123456789ABCDEFGHIJK");
        break;
    case TEST_CHINESE:
        /* 显示汉字 */
        UNITEST_OLED_096.OELD_ShowCHinese(&UNITEST_OLED_096, 0, 0, 14);  /* 题 */
        UNITEST_OLED_096.OELD_ShowCHinese(&UNITEST_OLED_096, 16, 2, 15); /* 目 */
        break;
    }
}
#endif /* BSP_IIC_OLED_UNITEST */
