#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f10x.h"

/*!
 * \brief   ���Ժ�
 * \param 1 �����־
 * \param 0 �������־
 */
#define BSP_KEY_DEBUG 0
#if BSP_KEY_DEBUG
#include "bsp_log.h"
#define KEY_LOG log
#else
#define KEY_LOG(tag, ...)
#endif /* BSP_KEY_DEBUG */

/*!
 * \brief   �������º��ƽ
 * \param 1 ����VCC ���º�Ϊ�ߵ�ƽ
 * \param 0 ����GND ���º�Ϊ�͵�ƽ
 */
#define KEY_PRESS_LEVEL 0
#if KEY_PRESS_LEVEL
// ��������
#define KEY_INPUT_MODE GPIO_Mode_IPD
#else
// ��������
#define KEY_INPUT_MODE GPIO_Mode_IPU
#endif /* KEY_PRESS_LEVEL */

/*!
 * \brief   ����ʱ�� 
 */
#define KEY_SHAKE_TIME 1
/*!
 * \brief   ��������ʱ��
 */
#define KEY_PRESS_LONG_TIME 10
/*!
 * \brief   �����������ʱ��
 */
#define KEY_PRESS_CONTINUE_TIME 5

/* �����¼� */
enum key_trigger_event
{
    KEY_EVENT_IDLE,                  /* �������� */
    KEY_EVENT_CLICK,                 /* ����ȷ�� */
    KEY_EVENT_CLICK_RELEASE,         /* �����ͷ� */
    KEY_EVENT_LONG_PRESS,            /* ����ȷ�� */
    KEY_EVENT_LONG_PRESS_CONTINUOUS, /* �������� */
    KEY_EVENT_LONG_PRESS_RELEASE,    /* �����ͷ� */
    KEY_EVENT_SUM
};

typedef void (*key_event_callback)(enum key_trigger_event);

typedef struct key
{
    // ����
    GPIO_TypeDef *GPIOx; // ������GPIO��(�磺GPIOA)
    uint32_t GPIO;       // IO����(�磺GPIO_Pin_0)

    uint8_t step;                 /* ��ⲽ�� */
    uint16_t click_time;          /* ����ʱ�� */
    uint16_t long_press_time;     /* ����ʱ�� */
    uint16_t continue_press_time; /* ��������ʱ�� */

    key_event_callback func; /* �����ص�����ָ�� */
} key_t;

void key_init(key_t *key);
void key_proc(struct key *key);

#endif /* __KEY_H */
