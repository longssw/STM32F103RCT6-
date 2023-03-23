#ifndef __TASK_INIT_H_
#define __TASK_INIT_H_
/* �ӿڼ��� (ע��÷�����ǿ���) */
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
/* ��Ϣ����ͷ�ļ� */
#include "queue.h"
/*!
 * ������
 */
extern TaskHandle_t AppTaskCreate_Handle; /* ���������� */
extern TaskHandle_t Led_Task_Handle;      /* LED������ */
extern TaskHandle_t Cmd_Task_Handle;      /* CMD������ */
extern TaskHandle_t Motor_Task_Handle;    /* MOTOR������ */
extern TaskHandle_t Adc_Task_Handle;      /* ADC������ */
/*!
 * ��Ϣ���о��
 */
extern QueueHandle_t Task_Queue;
void AppTaskCreate(void);

#endif /* __TASK_INIT_H_ */
