#include "bsp_usart.h"
#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE {
  int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x) { x = x; }
//�ض���fputc����
int fputc(int ch, FILE *f) {
  while ((USART1->SR & 0X40) == 0)
    ;  //ѭ������,ֱ���������
  USART1->DR = (u8)ch;
  return ch;
}
#endif /* printf */

typedef void (*receive_callback_t)(u8 data);

struct usrt {
  receive_callback_t fun;
};

static struct usrt usrts;

void usrt_receive_register(USART_TypeDef * id, void (*fun)(u8)) {
  if (id == USART1) {
    if (NULL == usrts.fun) {
      usrts.fun = fun;
    }
  }
}

void register_usrtx_callback(void (*fun)(u8)) {
  if (NULL == usrts.fun) {
    usrts.fun = fun;
  }
}

void usart1_init(u32 baud) {
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  // �򿪴���GPIO��ʱ��
  DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);

  // �򿪴��������ʱ��
  DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

  // ��USART Tx��GPIO����Ϊ���츴��ģʽ
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

  // ���ô��ڵĹ�������
  // ���ò�����
  USART_InitStructure.USART_BaudRate = baud;
  // ���� �������ֳ�
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  // ����ֹͣλ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  // ����У��λ
  USART_InitStructure.USART_Parity = USART_Parity_No;
  // ����Ӳ��������
  USART_InitStructure.USART_HardwareFlowControl =
      USART_HardwareFlowControl_None;
  // ���ù���ģʽ���շ�һ��
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  // ��ɴ��ڵĳ�ʼ������
  USART_Init(DEBUG_USARTx, &USART_InitStructure);

  // �����ж����ȼ�����
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);

  // ʹ�ܴ��ڽ����ж�
  USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);

  // ʹ�ܴ���
  USART_Cmd(DEBUG_USARTx, ENABLE);
}

// �����жϷ�����
void DEBUG_USART_IRQHandler(void) {
  u8 rev_data;
  if (USART_GetITStatus(DEBUG_USARTx, USART_IT_RXNE) != RESET) {
    rev_data = USART_ReceiveData(DEBUG_USARTx);
    if (NULL != usrts.fun) {
      usrts.fun(rev_data);
    }
  }
}
