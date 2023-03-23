
/*****************************************************************************
* 文 件 名:   bsp_key.c
* 日    期:   2023-02-07 20:54:02
* 作    者:   善思维
* 版    本:   1.1
* 功    能:   精简接口
*******************************************************************************/
#include "bsp_key.h"
#include <stdio.h>

/* 状态机步骤 */
enum key_detection_step
{
	KEY_STEP_WAIT = 0,		   /* 等待按键 */
	KEY_STEP_CLICK,			   /* 按键按下 */
	KEY_STEP_LONG_PRESS,	   /* 按键长按 */
	KEY_STEP_CONTINUOUS_PRESS, /* 持续长按 */
};

/*!
 * \brief   返回当前按键的电平
 */
static uint8_t read_io(const struct key *key)
{
	return (GPIO_ReadInputDataBit(key->GPIOx, key->GPIO));
}

/*!
 * \brief   初使化按键引脚
 */
static void init(const struct key *key)
{
	GPIO_InitTypeDef GPIO_Initure;

	// 根据GPIO组初始化GPIO时钟
	if (key->GPIOx == GPIOA)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能GPIOA时钟
	}
	if (key->GPIOx == GPIOB)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能GPIOB时钟
	}
	if (key->GPIOx == GPIOC)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 使能GPIOC时钟
	}
	if (key->GPIOx == GPIOD)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // 使能GPIOD时钟
	}
	if (key->GPIOx == GPIOE)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); // 使能GPIOE时钟
	}

	GPIO_Initure.GPIO_Pin = key->GPIO;
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initure.GPIO_Mode = KEY_INPUT_MODE; // 上拉输入
	GPIO_Init(key->GPIOx, &GPIO_Initure);
}

/*!
 * \brief   按键扫描
 * \brief   应当放在10ms执行一次的定时器中
 * \brief   当有按键事件触发时会执行回调函数?
 */
void key_proc(struct key *key)
{
	uint8_t key_level;
	enum key_trigger_event event;

	key_level = read_io(key);
	event = KEY_EVENT_IDLE; /* 按键空闲 */

	/* 检测按键 */
	switch (key->step)
	{
	case KEY_STEP_WAIT: /* 按键等待 */
		if (key_level == KEY_PRESS_LEVEL)
		{
			KEY_LOG(DEBUG, "[KEY_STEP_WAIT]");
			key->step = KEY_STEP_CLICK; /* [状态机]:按键???? */
		}
		break;
	case KEY_STEP_CLICK: /* 按键单击 */
		if (key_level == KEY_PRESS_LEVEL)
		{
			if (!(--key->click_time)) /* 达到消抖时间 */
			{
				KEY_LOG(DEBUG, "[KEY_CLICK] success!");
				key->click_time = KEY_SHAKE_TIME; /* [重置]:消抖时间 */
				event = KEY_EVENT_CLICK;		  /* [按键事件]:单击按下 */
				key->step = KEY_STEP_LONG_PRESS;  /* [状态机]:按键长按 */
			}
		}
		else /* 未达到消抖时间 */
		{
			key->click_time = KEY_SHAKE_TIME; /* [重置]:消抖时间 */
			key->step = KEY_STEP_WAIT;		  /* [状态机]:按键等待 */
		}
		break;
	case KEY_STEP_LONG_PRESS: /* 按键长按 */
		if (key_level == KEY_PRESS_LEVEL)
		{
			if (!(--key->long_press_time)) /* 达到长按时间 */
			{
				KEY_LOG(DEBUG, "[KEY_LONG_PRESS] success!");
				key->long_press_time = KEY_PRESS_LONG_TIME; /* [重置]:长按时间 */
				event = KEY_EVENT_LONG_PRESS;				/* [按键事件]:按键长按 */
				key->step = KEY_STEP_CONTINUOUS_PRESS;		/* [状态机]:按键持续长按 */
			}
		}
		else
		{
			KEY_LOG(DEBUG, "[KEY_LONG_PRESS] fail! the time remaining is [%d]", key->long_press_time);
			key->long_press_time = KEY_PRESS_LONG_TIME; /* [重置]:长按时间 */
			key->step = KEY_STEP_WAIT;					/* [状态机]:按键等待 */
			event = KEY_EVENT_CLICK_RELEASE;			/* [按键事件]:单击释放 */
		}
		break;
	case KEY_STEP_CONTINUOUS_PRESS: /* 按键持续长按 */
		if (key_level == KEY_PRESS_LEVEL)
		{
			if (!(--key->continue_press_time)) /* 达到持续长按间隔时间 */
			{
				KEY_LOG(DEBUG, "[KEY_STEP_CONTINUOUS_PRESS] success!");
				event = KEY_EVENT_LONG_PRESS_CONTINUOUS; /* [按键事件]:持续长按 */
				/*!
				 * \brief   这里由于一直在重置时间所以长按持续事件只会调度一次?
				 */
				key->continue_press_time = KEY_PRESS_CONTINUE_TIME; /* [重置]:持续长按时间 */
			}
		}
		else
		{
			KEY_LOG(DEBUG, "[KEY_STEP_CONTINUOUS_PRESS] fail! TIME == %d", key->continue_press_time);
			key->step = KEY_STEP_WAIT;							/* [状态机]:按键等待 */
			key->continue_press_time = KEY_PRESS_CONTINUE_TIME; /* [重置]:持续长按时间 */
			event = KEY_EVENT_LONG_PRESS_RELEASE;				/* [按键事件]:长按释放 */
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
	key->step = KEY_STEP_WAIT;							/* 等待按键按下 */
	key->click_time = KEY_SHAKE_TIME;					/* 消抖时间 */
	key->long_press_time = KEY_PRESS_LONG_TIME;			/* 按键长按时间 */
	key->continue_press_time = KEY_PRESS_CONTINUE_TIME; /* 持续长按间隔时间 */

	init(key);
}
