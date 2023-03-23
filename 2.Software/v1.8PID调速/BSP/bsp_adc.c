#include "bsp_adc.h"

#define ADC1_DR_Address ((u32)0x4001244C) /* 基地址 */
u16 AD_Value[1];                          /* 采用后值存放的位置 */
// 对应引脚 [0] A3 [1] A6 [2] B0 [3] B1

static void Adc_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能ADC1时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ADC配置
    ADC_DeInit(ADC1);                                                   // 复位ADC1
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // 独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       // 多通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // 多次转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             // ADC通道转换4

    // 初始化ADC
    ADC_Init(ADC1, &ADC_InitStructure);
    // 配置ADC 通道的转换顺序和采样时间
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);

    // 使能 DMA 通道
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);      // 使能指定的ADC1
    ADC_ResetCalibration(ADC1); // 使能复位校准

    while (ADC_GetResetCalibrationStatus(ADC1))
        ; // 等待复位校准结束

    ADC_StartCalibration(ADC1); // 开启AD校准

    while (ADC_GetCalibrationStatus(ADC1))
        ; // 等待校准结束

    // 使能指定的ADC1的软件转换启动功能
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// DMA配置
static void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // 使能DMA传输

    DMA_DeInit(DMA1_Channel1); // 将DMA的通道1寄存器重设为缺省值

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;                  // DMA外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;                      // DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //
    DMA_InitStructure.DMA_BufferSize = ADC_CHANNEL_SUM;                         // DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // 内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 数据宽度为16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // 数据宽度为16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // 工作	循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         // DMA通道 拥有高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                // 根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

void adc_init(void)
{
    Adc_Init();
    DMA_Config();
}

#if 0
/* 多路ADC Demo */
#define ADC1_DR_Address ((u32)0x4001244C) /* 基地址 */
u16 AD_Value[4];                          /* 采用后值存放的位置 */
// 对应引脚 [0] A3 [1] A6 [2] B0 [3] B1

void Adc_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M


    // A3 A6 B0 B1 配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ADC配置
    ADC_DeInit(ADC1);                                                   // 复位ADC1
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // 独立模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                        // 多通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // 多次转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 4;                             // ADC通道转换4

    // 初始化ADC
    ADC_Init(ADC1, &ADC_InitStructure);
    // 配置ADC 通道的转换顺序和采样时间
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5); // ADC1,ADC通道,采样时间为55.5周期
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_55Cycles5);
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_55Cycles5);

    // 使能 DMA 通道
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);      // 使能指定的ADC1
    ADC_ResetCalibration(ADC1); // 使能复位校准

    while (ADC_GetResetCalibrationStatus(ADC1))
        ; // 等待复位校准结束

    ADC_StartCalibration(ADC1); // 开启AD校准

    while (ADC_GetCalibrationStatus(ADC1))
        ; // 等待校准结束

    // 使能指定的ADC1的软件转换启动功能
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// DMA配置
void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // 使能DMA传输

    DMA_DeInit(DMA1_Channel1); // 将DMA的通道1寄存器重设为缺省值

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;                  // DMA外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;                      // DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //
    DMA_InitStructure.DMA_BufferSize = 4;                                       // DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // 内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 数据宽度为16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // 数据宽度为16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // 工作	循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         // DMA通道 拥有高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                // 根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
    DMA_Cmd(DMA1_Channel1, ENABLE);
}
#endif
