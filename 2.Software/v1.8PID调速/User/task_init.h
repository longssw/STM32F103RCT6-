#ifndef __TASK_INIT_H_
#define __TASK_INIT_H_
/* 接口集成 (注意该方法是强耦合) */
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
/* 消息队列头文件 */
#include "queue.h"
/*!
 * 任务句柄
 */
extern TaskHandle_t AppTaskCreate_Handle; /* 创建任务句柄 */
extern TaskHandle_t Led_Task_Handle;      /* LED任务句柄 */
extern TaskHandle_t Cmd_Task_Handle;      /* CMD任务句柄 */
extern TaskHandle_t Motor_Task_Handle;    /* MOTOR任务句柄 */
extern TaskHandle_t Adc_Task_Handle;      /* ADC任务句柄 */
/*!
 * 消息队列句柄
 */
extern QueueHandle_t Task_Queue;
void AppTaskCreate(void);

#endif /* __TASK_INIT_H_ */
