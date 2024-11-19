#ifndef __CHASSIS_TASK_H
#define __CHASSIS_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

void chassisControlTask(void *argument);

#endif  // __CHASSIS_TASK_H