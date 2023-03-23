#include "bsp_adc.h"

#define ADC1_DR_Address ((u32)0x4001244C) /* ����ַ */
u16 AD_Value[1];                          /* ���ú�ֵ��ŵ�λ�� */
// ��Ӧ���� [0] A3 [1] A6 [2] B0 [3] B1

static void Adc_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ��ADC1ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // ģ������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ADC����
    ADC_DeInit(ADC1);                                                   // ��λADC1
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // ����ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       // ��ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // ���ת��
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // �����ⲿ��������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // �Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             // ADCͨ��ת��4

    // ��ʼ��ADC
    ADC_Init(ADC1, &ADC_InitStructure);
    // ����ADC ͨ����ת��˳��Ͳ���ʱ��
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);

    // ʹ�� DMA ͨ��
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);      // ʹ��ָ����ADC1
    ADC_ResetCalibration(ADC1); // ʹ�ܸ�λУ׼

    while (ADC_GetResetCalibrationStatus(ADC1))
        ; // �ȴ���λУ׼����

    ADC_StartCalibration(ADC1); // ����ADУ׼

    while (ADC_GetCalibrationStatus(ADC1))
        ; // �ȴ�У׼����

    // ʹ��ָ����ADC1�����ת����������
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// DMA����
static void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // ʹ��DMA����

    DMA_DeInit(DMA1_Channel1); // ��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;                  // DMA�������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;                      // DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //
    DMA_InitStructure.DMA_BufferSize = ADC_CHANNEL_SUM;                         // DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // �����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // �ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // ���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // ���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // ����	ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         // DMAͨ�� ӵ�и����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                // ����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

void adc_init(void)
{
    Adc_Init();
    DMA_Config();
}

#if 0
/* ��·ADC Demo */
#define ADC1_DR_Address ((u32)0x4001244C) /* ����ַ */
u16 AD_Value[4];                          /* ���ú�ֵ��ŵ�λ�� */
// ��Ӧ���� [0] A3 [1] A6 [2] B0 [3] B1

void Adc_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M


    // A3 A6 B0 B1 ����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // ģ������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // ģ������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ADC����
    ADC_DeInit(ADC1);                                                   // ��λADC1
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // ����ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                        // ��ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // ���ת��
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // �����ⲿ��������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // �Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 4;                             // ADCͨ��ת��4

    // ��ʼ��ADC
    ADC_Init(ADC1, &ADC_InitStructure);
    // ����ADC ͨ����ת��˳��Ͳ���ʱ��
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5); // ADC1,ADCͨ��,����ʱ��Ϊ55.5����
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_55Cycles5);
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_55Cycles5);

    // ʹ�� DMA ͨ��
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);      // ʹ��ָ����ADC1
    ADC_ResetCalibration(ADC1); // ʹ�ܸ�λУ׼

    while (ADC_GetResetCalibrationStatus(ADC1))
        ; // �ȴ���λУ׼����

    ADC_StartCalibration(ADC1); // ����ADУ׼

    while (ADC_GetCalibrationStatus(ADC1))
        ; // �ȴ�У׼����

    // ʹ��ָ����ADC1�����ת����������
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// DMA����
void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // ʹ��DMA����

    DMA_DeInit(DMA1_Channel1); // ��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;                  // DMA�������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;                      // DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //
    DMA_InitStructure.DMA_BufferSize = 4;                                       // DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // �����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // �ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // ���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // ���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // ����	ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         // DMAͨ�� ӵ�и����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                // ����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
    DMA_Cmd(DMA1_Channel1, ENABLE);
}
#endif
