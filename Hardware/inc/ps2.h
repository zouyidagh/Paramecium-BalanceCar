#ifndef __PS2_H__
#define __PS2_H__

#include "main.h"
#include "gpio.h"
#include "delayus.h"

/*******PS2相关指令表*******/
#define START_CMD 0x01
#define ASK_DAT_CMD 0x42

/*******PS2模式数据表*******/
#define PS2_MODE_GRN 0x41
#define PS2_MODE_RED 0x73

/*******PS2按键检测表*******/
#define PS2_CONNECT !(ps2_buf[1] & 0x73) ? 1 : 0
#define PS2_LEFT_UP !(ps2_buf[3] & 0x10) ? 1 : 0
#define PS2_LEFT_RIGHT !(ps2_buf[3] & 0x20) ? 1 : 0
#define PS2_LEFT_DOWN !(ps2_buf[3] & 0x40) ? 1 : 0
#define PS2_LEFT_LEFT !(ps2_buf[3] & 0x80) ? 1 : 0

#define PS2_SELECT !(ps2_buf[3] & 0x01) ? 1 : 0
#define PS2_START !(ps2_buf[3] & 0x08) ? 1 : 0

#define PS2_RIGHT_UP !(ps2_buf[4] & 0x10) ? 1 : 0
#define PS2_RIGHT_RIGHT !(ps2_buf[4] & 0x20) ? 1 : 0
#define PS2_RIGHT_DOWN !(ps2_buf[4] & 0x40) ? 1 : 0
#define PS2_RIGHT_LEFT !(ps2_buf[4] & 0x80) ? 1 : 0

#define PS2_LEFT_2 !(ps2_buf[4] & 0x01) ? 1 : 0
#define PS2_RIGHT_2 !(ps2_buf[4] & 0x02) ? 1 : 0
#define PS2_LEFT_1 !(ps2_buf[4] & 0x04) ? 1 : 0
#define PS2_RIGHT_1 !(ps2_buf[4] & 0x08) ? 1 : 0

#define PS2_RIGHT_X (int)(ps2_buf[5] - 0x7f)
#define PS2_RIGHT_Y (int)(ps2_buf[6] - 0x80)
#define PS2_LEFT_X (int)(ps2_buf[7] - 0x7f)
#define PS2_LEFT_Y (int)(ps2_buf[8] - 0x80)

/*******PS2管脚映射表*******/
#define PS2_DAT HAL_GPIO_ReadPin(GPIOA, DAT_Pin)
#define PS2_CMD(x) HAL_GPIO_WritePin(GPIOA, CMD_Pin, (GPIO_PinState)x)
#define PS2_ATT(x) HAL_GPIO_WritePin(GPIOB, ATT_Pin, (GPIO_PinState)x)
#define PS2_CLK(x) HAL_GPIO_WritePin(GPIOB, CLK_Pin, (GPIO_PinState)x)

/*******PS2结构体定义*******/
typedef __packed struct
{
	int8_t LEFT_UP;
	int8_t LEFT_RIGHT;
	int8_t LEFT_DOWN;
	int8_t LEFT_LEFT;

	int8_t SELECT;
	int8_t START;

	int8_t RIGHT_UP;
	int8_t RIGHT_RIGHT;
	int8_t RIGHT_DOWN;
	int8_t RIGHT_LEFT;

	int8_t LEFT_2;
	int8_t RIGHT_2;
	int8_t LEFT_1;
	int8_t RIGHT_1;

	int RIGHT_X;
	int RIGHT_Y;
	int LEFT_X;
	int LEFT_Y;
} rc_ps2_t;

// 增加extern全局变量声明，防止重定义
extern uint8_t ps2_buf[];
extern uint8_t ps2_mode;
extern rc_ps2_t rc_ps2;
extern rc_ps2_t rc_ps2;

/*******PS2相关函数声明*******/
uint8_t ps2_transfer(unsigned char dat);
void ps2_write_read(unsigned char *get_buf);
void ps2_init(void);
void ps2_update(rc_ps2_t *rc_ps2);
rc_ps2_t *get_remote_control_point(void);

#endif
