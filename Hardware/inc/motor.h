#ifndef _MOTOR_H
#define _MOTOR_H

#include "main.h"
#include "tim.h"

void Load(int moto1,int moto2);
void Limit(int *motoA,int *motoB);
void motor_init(void);

#endif
