
#include "task_init.h"

#include "task_msg.h"
#define LOG_TAG "INIT"
#include <elog.h>

/**************************** 任务句柄 ********************************/
/*
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */

TaskHandle_t AppTaskCreate_Handle = NULL; /* 创建任务句柄 */
TaskHandle_t Led_Task_Handle = NULL;      /* LED任务句柄 */
TaskHandle_t Motor_Task_Handle = NULL;    /* MOTOR任务句柄 */
TaskHandle_t Cmd_Task_Handle = NULL;      /* CMD任务句柄 */
TaskHandle_t Adc_Task_Handle = NULL;      /* ADC任务句柄 */
/******************************** 内核对象句柄 ********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 *
 */
QueueHandle_t Task_Queue = NULL; /* 消息队列句柄 */

#define TASK_QUEUE_LEN 10 /* 队列的长度，最大可包含多少个消息 */
#define TASK_QUEUE_SIZE \
  sizeof(struct task_msg_pack) /* 队列中每个消息大小（字节）*/

/* Application文件夹中的.c文件下的任务 */
extern void led_task(void *parameter);
extern void key_task(void *parameter);
extern void oled_task(void *parameter);
extern void mpu6050_task(void *parameter);
extern void motor_task(void *parameter);
extern void cmd_task(void *parameter);
extern void adc_task(void *parameter);
/***********************************************************************
 * @ 函数名  ： AppTaskCreate
 * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
 * @ 参数    ： 无
 * @ 返回值  ： 无
 **********************************************************************/
void AppTaskCreate(void) {
  BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值，默认为pdPASS */

  taskENTER_CRITICAL(); /* 进入临界区 */

  /* 创建TASK消息队列 */
  Task_Queue = xQueueCreate((UBaseType_t)TASK_QUEUE_LEN, /* 消息队列的长度 */
                            (UBaseType_t)TASK_QUEUE_SIZE); /* 消息的大小 */
  if (NULL != Task_Queue) log_i("creat task queue succeed!");
  /* 创建LED任务 */
  xReturn = xTaskCreate((TaskFunction_t)led_task, /* 任务入口函数 */
                        (const char *)"led_task", /* 任务名字 */
                        (uint16_t)512,            /* 任务栈大小 */
                        (void *)NULL,             /* 任务入口函数参数 */
                        (UBaseType_t)3,           /* 任务的优先级 */
                        (TaskHandle_t *)&Led_Task_Handle); /* 任务控制块指针 */
  if (pdPASS == xReturn) log_i("creat led task succeed!");
#if 0
  /* 创建KEY任务 */
  xReturn = xTaskCreate((TaskFunction_t)key_task,          /* 任务入口函数 */
                        (const char *)"key_task",          /* 任务名字 */
                        (uint16_t)512,                     /* 任务栈大小 */
                        (void *)NULL,                      /* 任务入口函数参数 */
                        (UBaseType_t)3,                    /* 任务的优先级 */
                        (TaskHandle_t *)&Led_Task_Handle); /* 任务控制块指针 */
  if (pdPASS == xReturn)
    log_i("creat key task succeed!");
#endif
#if 0
  /* 创建OELD任务 */
  xReturn = xTaskCreate((TaskFunction_t)oled_task,         /* 任务入口函数 */
                        (const char *)"oled_task",         /* 任务名字 */
                        (uint16_t)512,                     /* 任务栈大小 */
                        (void *)NULL,                      /* 任务入口函数参数 */
                        (UBaseType_t)3,                    /* 任务的优先级 */
                        (TaskHandle_t *)&Led_Task_Handle); /* 任务控制块指针 */
  if (pdPASS == xReturn)
    log_i("creat oled task succeed!");
#endif
#if 1
  /* 创建MPU6050任务 */
  xReturn = xTaskCreate((TaskFunction_t)mpu6050_task,      /* 任务入口函数 */
                        (const char *)"mpu6050_task",      /* 任务名字 */
                        (uint16_t)512,                     /* 任务栈大小 */
                        (void *)NULL,                      /* 任务入口函数参数 */
                        (UBaseType_t)3,                    /* 任务的优先级 */
                        (TaskHandle_t *)&Led_Task_Handle); /* 任务控制块指针 */
  if (pdPASS == xReturn)
    log_i("creat cmd task succeed!\n");
#endif
#if 1
  /* 创建Motor任务 */
  xReturn =
      xTaskCreate((TaskFunction_t)motor_task, /* 任务入口函数 */
                  (const char *)"motor_task", /* 任务名字 */
                  (uint16_t)512,              /* 任务栈大小 */
                  (void *)NULL,               /* 任务入口函数参数 */
                  (UBaseType_t)3,             /* 任务的优先级 */
                  (TaskHandle_t *)&Motor_Task_Handle); /* 任务控制块指针 */
  if (pdPASS == xReturn) log_i("creat mortor task succeed!");
#endif
  /* 创建Cmd任务 */
  xReturn = xTaskCreate((TaskFunction_t)cmd_task, /* 任务入口函数 */
                        (const char *)"cmd_task", /* 任务名字 */
                        (uint16_t)512,            /* 任务栈大小 */
                        (void *)NULL,             /* 任务入口函数参数 */
                        (UBaseType_t)3,           /* 任务的优先级 */
                        (TaskHandle_t *)&Adc_Task_Handle); /* 任务控制块指针 */
  if (pdPASS == xReturn) log_i("creat cmd task succeed!");
  /* 创建Adc任务 */
  xReturn = xTaskCreate((TaskFunction_t)adc_task, /* 任务入口函数 */
                        (const char *)"adc_task", /* 任务名字 */
                        (uint16_t)512,            /* 任务栈大小 */
                        (void *)NULL,             /* 任务入口函数参数 */
                        (UBaseType_t)3,           /* 任务的优先级 */
                        (TaskHandle_t *)&Cmd_Task_Handle); /* 任务控制块指针 */
  if (pdPASS == xReturn) log_i("creat adc task succeed!");
  vTaskDelete(AppTaskCreate_Handle); /* 删除AppTaskCreate任务 */

  taskEXIT_CRITICAL(); /* 退出临界区 */
}
