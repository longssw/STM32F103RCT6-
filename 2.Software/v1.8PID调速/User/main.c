/*
 *************************************************************************
 *                             包含的头文件
 *************************************************************************
 */
/* FreeRTOS头文件 */
#include "task_init.h"
/* 开发板硬件bsp头文件 */
#include <elog.h>

#include "bsp_usart.h"

static TaskHandle_t AppTaskCreate_Handle = NULL; /* 创建任务句柄 */
static void BSP_Init(void); /* 用于初始化板载相关资源 */
/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/
int main(void) {
  BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值，默认为pdPASS */

  /* 开发板硬件初始化 */
  BSP_Init();
  easylogger_init();

  /* 创建AppTaskCreate任务 */

  xReturn =
      xTaskCreate((TaskFunction_t)AppTaskCreate, /* 任务入口函数 */
                  (const char *)"AppTaskCreate", /* 任务名字 */
                  (uint16_t)512,                 /* 任务栈大小 */
                  (void *)NULL,                  /* 任务入口函数参数 */
                  (UBaseType_t)1,                /* 任务的优先级 */
                  (TaskHandle_t *)&AppTaskCreate_Handle); /* 任务控制块指针 */
  /* 启动任务调度 */
  if (pdPASS == xReturn)
    vTaskStartScheduler(); /* 启动任务，开启调度 */
  else
    return -1;

  while (1)
    ; /* 正常不会执行到这里 */
}

/***********************************************************************
 * @ 函数名  ： BSP_Init
 * @ 功能说明： 板级外设初始化，所有板子上的初始化均可放在这个函数里面
 * @ 参数    ：
 * @ 返回值  ： 无
 *********************************************************************/

void usrt1_cbs(u8 ch);

static void BSP_Init(void) {
  /*
   * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
   * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
   * 都统一用这个优先级分组，千万不要再分组，切忌。
   */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* 串口初始化	*/
  usart1_init(115200);
}

/********************************END OF FILE****************************/
