#include "ps2_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

extern osMessageQueueId_t ps2DataQueueHandle;

void ps2ControllerTask(void *argument)
{
    TickType_t wakeTime = xTaskGetTickCount();
    ps2_init();
    rc_ps2_t *p_rc_ps2 = &rc_ps2;
    while(1)
    {
        ps2_update(p_rc_ps2);
        osMessageQueuePut(ps2DataQueueHandle, &p_rc_ps2, 0, osWaitForever);
        vTaskDelayUntil(&wakeTime, 5);
    }
}