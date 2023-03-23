#include "FreeRTOS.h"
#include "task.h"
#include "bsp_key.h"

#define LOG_TAG "KEY"
#include <elog.h>

static void key1_event_handle(enum key_trigger_event event);
static void key2_event_handle(enum key_trigger_event event);

key_t KEY1 = {
    .GPIOx = GPIOA,
    .GPIO = GPIO_Pin_8,
    .func = key1_event_handle,
};

key_t KEY2 = {
    .GPIOx = GPIOC,
    .GPIO = GPIO_Pin_13,
    .func = key2_event_handle,
};

static void key1_event_handle(enum key_trigger_event event)
{
    log_d("KEY1 [%d]", event);
    switch (event)
    {
    case KEY_EVENT_CLICK:

        break;
    }
}

static void key2_event_handle(enum key_trigger_event event)
{
    log_d("KEY2 [%d]", event);
    switch (event)
    {
    case KEY_EVENT_CLICK:

        break;
    }
}

void key_task(void *parameter)
{
    key_init(&KEY1);
    key_init(&KEY2);
    log_i("KEY init!");

    while (1)
    {
        key_proc(&KEY1);
        key_proc(&KEY2);
        vTaskDelay(100);
    }
}
