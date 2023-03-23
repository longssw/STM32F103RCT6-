
#include "task_init.h"

#include "task_msg.h"
#define LOG_TAG "INIT"
#include <elog.h>

/**************************** ������ ********************************/
/*
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */

TaskHandle_t AppTaskCreate_Handle = NULL; /* ���������� */
TaskHandle_t Led_Task_Handle = NULL;      /* LED������ */
TaskHandle_t Motor_Task_Handle = NULL;    /* MOTOR������ */
TaskHandle_t Cmd_Task_Handle = NULL;      /* CMD������ */
TaskHandle_t Adc_Task_Handle = NULL;      /* ADC������ */
/******************************** �ں˶����� ********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 *
 */
QueueHandle_t Task_Queue = NULL; /* ��Ϣ���о�� */

#define TASK_QUEUE_LEN 10 /* ���еĳ��ȣ����ɰ������ٸ���Ϣ */
#define TASK_QUEUE_SIZE \
  sizeof(struct task_msg_pack) /* ������ÿ����Ϣ��С���ֽڣ�*/

/* Application�ļ����е�.c�ļ��µ����� */
extern void led_task(void *parameter);
extern void key_task(void *parameter);
extern void oled_task(void *parameter);
extern void mpu6050_task(void *parameter);
extern void motor_task(void *parameter);
extern void cmd_task(void *parameter);
extern void adc_task(void *parameter);
/***********************************************************************
 * @ ������  �� AppTaskCreate
 * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
 * @ ����    �� ��
 * @ ����ֵ  �� ��
 **********************************************************************/
void AppTaskCreate(void) {
  BaseType_t xReturn = pdPASS; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

  taskENTER_CRITICAL(); /* �����ٽ��� */

  /* ����TASK��Ϣ���� */
  Task_Queue = xQueueCreate((UBaseType_t)TASK_QUEUE_LEN, /* ��Ϣ���еĳ��� */
                            (UBaseType_t)TASK_QUEUE_SIZE); /* ��Ϣ�Ĵ�С */
  if (NULL != Task_Queue) log_i("creat task queue succeed!");
  /* ����LED���� */
  xReturn = xTaskCreate((TaskFunction_t)led_task, /* ������ں��� */
                        (const char *)"led_task", /* �������� */
                        (uint16_t)512,            /* ����ջ��С */
                        (void *)NULL,             /* ������ں������� */
                        (UBaseType_t)3,           /* ��������ȼ� */
                        (TaskHandle_t *)&Led_Task_Handle); /* ������ƿ�ָ�� */
  if (pdPASS == xReturn) log_i("creat led task succeed!");
#if 0
  /* ����KEY���� */
  xReturn = xTaskCreate((TaskFunction_t)key_task,          /* ������ں��� */
                        (const char *)"key_task",          /* �������� */
                        (uint16_t)512,                     /* ����ջ��С */
                        (void *)NULL,                      /* ������ں������� */
                        (UBaseType_t)3,                    /* ��������ȼ� */
                        (TaskHandle_t *)&Led_Task_Handle); /* ������ƿ�ָ�� */
  if (pdPASS == xReturn)
    log_i("creat key task succeed!");
#endif
#if 0
  /* ����OELD���� */
  xReturn = xTaskCreate((TaskFunction_t)oled_task,         /* ������ں��� */
                        (const char *)"oled_task",         /* �������� */
                        (uint16_t)512,                     /* ����ջ��С */
                        (void *)NULL,                      /* ������ں������� */
                        (UBaseType_t)3,                    /* ��������ȼ� */
                        (TaskHandle_t *)&Led_Task_Handle); /* ������ƿ�ָ�� */
  if (pdPASS == xReturn)
    log_i("creat oled task succeed!");
#endif
#if 1
  /* ����MPU6050���� */
  xReturn = xTaskCreate((TaskFunction_t)mpu6050_task,      /* ������ں��� */
                        (const char *)"mpu6050_task",      /* �������� */
                        (uint16_t)512,                     /* ����ջ��С */
                        (void *)NULL,                      /* ������ں������� */
                        (UBaseType_t)3,                    /* ��������ȼ� */
                        (TaskHandle_t *)&Led_Task_Handle); /* ������ƿ�ָ�� */
  if (pdPASS == xReturn)
    log_i("creat cmd task succeed!\n");
#endif
#if 1
  /* ����Motor���� */
  xReturn =
      xTaskCreate((TaskFunction_t)motor_task, /* ������ں��� */
                  (const char *)"motor_task", /* �������� */
                  (uint16_t)512,              /* ����ջ��С */
                  (void *)NULL,               /* ������ں������� */
                  (UBaseType_t)3,             /* ��������ȼ� */
                  (TaskHandle_t *)&Motor_Task_Handle); /* ������ƿ�ָ�� */
  if (pdPASS == xReturn) log_i("creat mortor task succeed!");
#endif
  /* ����Cmd���� */
  xReturn = xTaskCreate((TaskFunction_t)cmd_task, /* ������ں��� */
                        (const char *)"cmd_task", /* �������� */
                        (uint16_t)512,            /* ����ջ��С */
                        (void *)NULL,             /* ������ں������� */
                        (UBaseType_t)3,           /* ��������ȼ� */
                        (TaskHandle_t *)&Adc_Task_Handle); /* ������ƿ�ָ�� */
  if (pdPASS == xReturn) log_i("creat cmd task succeed!");
  /* ����Adc���� */
  xReturn = xTaskCreate((TaskFunction_t)adc_task, /* ������ں��� */
                        (const char *)"adc_task", /* �������� */
                        (uint16_t)512,            /* ����ջ��С */
                        (void *)NULL,             /* ������ں������� */
                        (UBaseType_t)3,           /* ��������ȼ� */
                        (TaskHandle_t *)&Cmd_Task_Handle); /* ������ƿ�ָ�� */
  if (pdPASS == xReturn) log_i("creat adc task succeed!");
  vTaskDelete(AppTaskCreate_Handle); /* ɾ��AppTaskCreate���� */

  taskEXIT_CRITICAL(); /* �˳��ٽ��� */
}
