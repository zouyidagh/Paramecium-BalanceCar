#include "motor.h"

#define PWM_MAX 7200
#define PWM_MIN -7200
extern TIM_HandleTypeDef htim1;
extern uint8_t stop;
int abs(int p)
{
	if(p>0)
		return p;
	else
		return -p;
}

void Load(int moto1,int moto2)			//-7200~7200
{
	if(moto1<0)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
	}
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,abs(moto1));
	if(moto2<0)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
	}
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,abs(moto2));
}

void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}
void Stop(float *Med_Jiaodu,float *Jiaodu)
{
	if(abs((int)(*Jiaodu-*Med_Jiaodu))>60)
	{
		Load(0,0);
		stop=1;
	}
}
