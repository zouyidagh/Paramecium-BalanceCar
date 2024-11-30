#ifndef __CHASSIS_TASK_H
#define __CHASSIS_TASK_H

#include "alg_pid.h"

typedef struct
{
    int Speed_L;
    int Speed_R;
    pid_type_def pid_speed;
    pid_type_def pid_vertical;
    pid_type_def pid_turn;
} chassisControl_str_t;

void chassisControlTask(void *argument);

#endif  // __CHASSIS_TASK_H