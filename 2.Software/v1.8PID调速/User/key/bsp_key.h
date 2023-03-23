#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f10x.h"

/*!
 * \brief   调试宏
 * \param 1 输出日志
 * \param 0 不输出日志
 */
#define BSP_KEY_DEBUG 0
#if BSP_KEY_DEBUG
#include "bsp_log.h"
#define KEY_LOG log
#else
#define KEY_LOG(tag, ...)
#endif /* BSP_KEY_DEBUG */

/*!
 * \brief   按键按下后电平
 * \param 1 连接VCC 按下后为高电平
 * \param 0 连接GND 按下后为低电平
 */
#define KEY_PRESS_LEVEL 0
#if KEY_PRESS_LEVEL
// 下拉输入
#define KEY_INPUT_MODE GPIO_Mode_IPD
#else
// 上拉输入
#define KEY_INPUT_MODE GPIO_Mode_IPU
#endif /* KEY_PRESS_LEVEL */

/*!
 * \brief   消抖时间 
 */
#define KEY_SHAKE_TIME 1
/*!
 * \brief   按键长按时间
 */
#define KEY_PRESS_LONG_TIME 10
/*!
 * \brief   持续长按间隔时间
 */
#define KEY_PRESS_CONTINUE_TIME 5

/* 按键事件 */
enum key_trigger_event
{
    KEY_EVENT_IDLE,                  /* 按键空闲 */
    KEY_EVENT_CLICK,                 /* 单击确认 */
    KEY_EVENT_CLICK_RELEASE,         /* 单击释放 */
    KEY_EVENT_LONG_PRESS,            /* 长按确认 */
    KEY_EVENT_LONG_PRESS_CONTINUOUS, /* 长按持续 */
    KEY_EVENT_LONG_PRESS_RELEASE,    /* 长按释放 */
    KEY_EVENT_SUM
};

typedef void (*key_event_callback)(enum key_trigger_event);

typedef struct key
{
    // 属性
    GPIO_TypeDef *GPIOx; // 所属的GPIO组(如：GPIOA)
    uint32_t GPIO;       // IO引脚(如：GPIO_Pin_0)

    uint8_t step;                 /* 检测步骤 */
    uint16_t click_time;          /* 单击时间 */
    uint16_t long_press_time;     /* 长按时间 */
    uint16_t continue_press_time; /* 持续长按时间 */

    key_event_callback func; /* 按键回调函数指针 */
} key_t;

void key_init(key_t *key);
void key_proc(struct key *key);

#endif /* __KEY_H */
