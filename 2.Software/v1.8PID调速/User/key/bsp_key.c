
/*****************************************************************************
* �� �� ��:   bsp_key.c
* ��    ��:   2023-02-07 20:54:02
* ��    ��:   ��˼ά
* ��    ��:   1.1
* ��    ��:   ����ӿ�
*******************************************************************************/
#include "bsp_key.h"
#include <stdio.h>

/* ״̬������ */
enum key_detection_step
{
	KEY_STEP_WAIT = 0,		   /* �ȴ����� */
	KEY_STEP_CLICK,			   /* �������� */
	KEY_STEP_LONG_PRESS,	   /* �������� */
	KEY_STEP_CONTINUOUS_PRESS, /* �������� */
};

/*!
 * \brief   ���ص�ǰ�����ĵ�ƽ
 */
static uint8_t read_io(const struct key *key)
{
	return (GPIO_ReadInputDataBit(key->GPIOx, key->GPIO));
}

/*!
 * \brief   ��ʹ����������
 */
static void init(const struct key *key)
{
	GPIO_InitTypeDef GPIO_Initure;

	// ����GPIO���ʼ��GPIOʱ��
	if (key->GPIOx == GPIOA)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // ʹ��GPIOAʱ��
	}
	if (key->GPIOx == GPIOB)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ʹ��GPIOBʱ��
	}
	if (key->GPIOx == GPIOC)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // ʹ��GPIOCʱ��
	}
	if (key->GPIOx == GPIOD)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // ʹ��GPIODʱ��
	}
	if (key->GPIOx == GPIOE)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); // ʹ��GPIOEʱ��
	}

	GPIO_Initure.GPIO_Pin = key->GPIO;
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initure.GPIO_Mode = KEY_INPUT_MODE; // ��������
	GPIO_Init(key->GPIOx, &GPIO_Initure);
}

/*!
 * \brief   ����ɨ��
 * \brief   Ӧ������10msִ��һ�εĶ�ʱ����
 * \brief   ���а����¼�����ʱ��ִ�лص�����?
 */
void key_proc(struct key *key)
{
	uint8_t key_level;
	enum key_trigger_event event;

	key_level = read_io(key);
	event = KEY_EVENT_IDLE; /* �������� */

	/* ��ⰴ�� */
	switch (key->step)
	{
	case KEY_STEP_WAIT: /* �����ȴ� */
		if (key_level == KEY_PRESS_LEVEL)
		{
			KEY_LOG(DEBUG, "[KEY_STEP_WAIT]");
			key->step = KEY_STEP_CLICK; /* [״̬��]:����???? */
		}
		break;
	case KEY_STEP_CLICK: /* �������� */
		if (key_level == KEY_PRESS_LEVEL)
		{
			if (!(--key->click_time)) /* �ﵽ����ʱ�� */
			{
				KEY_LOG(DEBUG, "[KEY_CLICK] success!");
				key->click_time = KEY_SHAKE_TIME; /* [����]:����ʱ�� */
				event = KEY_EVENT_CLICK;		  /* [�����¼�]:�������� */
				key->step = KEY_STEP_LONG_PRESS;  /* [״̬��]:�������� */
			}
		}
		else /* δ�ﵽ����ʱ�� */
		{
			key->click_time = KEY_SHAKE_TIME; /* [����]:����ʱ�� */
			key->step = KEY_STEP_WAIT;		  /* [״̬��]:�����ȴ� */
		}
		break;
	case KEY_STEP_LONG_PRESS: /* �������� */
		if (key_level == KEY_PRESS_LEVEL)
		{
			if (!(--key->long_press_time)) /* �ﵽ����ʱ�� */
			{
				KEY_LOG(DEBUG, "[KEY_LONG_PRESS] success!");
				key->long_press_time = KEY_PRESS_LONG_TIME; /* [����]:����ʱ�� */
				event = KEY_EVENT_LONG_PRESS;				/* [�����¼�]:�������� */
				key->step = KEY_STEP_CONTINUOUS_PRESS;		/* [״̬��]:������������ */
			}
		}
		else
		{
			KEY_LOG(DEBUG, "[KEY_LONG_PRESS] fail! the time remaining is [%d]", key->long_press_time);
			key->long_press_time = KEY_PRESS_LONG_TIME; /* [����]:����ʱ�� */
			key->step = KEY_STEP_WAIT;					/* [״̬��]:�����ȴ� */
			event = KEY_EVENT_CLICK_RELEASE;			/* [�����¼�]:�����ͷ� */
		}
		break;
	case KEY_STEP_CONTINUOUS_PRESS: /* ������������ */
		if (key_level == KEY_PRESS_LEVEL)
		{
			if (!(--key->continue_press_time)) /* �ﵽ�����������ʱ�� */
			{
				KEY_LOG(DEBUG, "[KEY_STEP_CONTINUOUS_PRESS] success!");
				event = KEY_EVENT_LONG_PRESS_CONTINUOUS; /* [�����¼�]:�������� */
				/*!
				 * \brief   ��������һֱ������ʱ�����Գ��������¼�ֻ�����һ��?
				 */
				key->continue_press_time = KEY_PRESS_CONTINUE_TIME; /* [����]:��������ʱ�� */
			}
		}
		else
		{
			KEY_LOG(DEBUG, "[KEY_STEP_CONTINUOUS_PRESS] fail! TIME == %d", key->continue_press_time);
			key->step = KEY_STEP_WAIT;							/* [״̬��]:�����ȴ� */
			key->continue_press_time = KEY_PRESS_CONTINUE_TIME; /* [����]:��������ʱ�� */
			event = KEY_EVENT_LONG_PRESS_RELEASE;				/* [�����¼�]:�����ͷ� */
		}
		break;
	}

	if (event)
	{
		KEY_LOG(INFO, "event = %d", event);
		if (key->func != NULL)
		{
			key->func(event);
		}
		else
		{
			KEY_LOG(ERROR, "the func is unregistered");
		}
	}
}

void key_init(struct key *key)
{
	key->step = KEY_STEP_WAIT;							/* �ȴ��������� */
	key->click_time = KEY_SHAKE_TIME;					/* ����ʱ�� */
	key->long_press_time = KEY_PRESS_LONG_TIME;			/* ��������ʱ�� */
	key->continue_press_time = KEY_PRESS_CONTINUE_TIME; /* �����������ʱ�� */

	init(key);
}
