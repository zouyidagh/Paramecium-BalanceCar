#include "ps2.h"

uint8_t ps2_buf[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t ps2_mode=0;
rc_ps2_t rc_ps2;

/***********************************************
	函数名称：ps2_transfer() 
	功能介绍：读写1个字节
	函数参数：无
	返回值：	无
 ***********************************************/
uint8_t ps2_transfer(unsigned char dat) {
	unsigned char rd_data ,wt_data, i;
	wt_data = dat;
	rd_data = 0;
	for(i = 0;i < 8;i++){
		PS2_CMD((wt_data & (0x01 << i)));
		PS2_CLK(1);
		delay_us(6);
		PS2_CLK(0);
		delay_us(6);
		PS2_CLK(1);
		if(PS2_DAT) {
			rd_data |= 0x01<<i;
		}
	}
	return rd_data;
}
	
/***********************************************
	函数名称：ps2_write_read() 
	功能介绍：读取手柄数据
	函数参数：无
	返回值：	无
 ***********************************************/
void ps2_write_read(uint8_t *ps2_buf) {
	PS2_ATT(0);
	ps2_buf[0] = ps2_transfer(START_CMD);
	ps2_buf[1] = ps2_transfer(ASK_DAT_CMD);
	ps2_buf[2] = ps2_transfer(ps2_buf[0]);
	ps2_buf[3] = ps2_transfer(ps2_buf[0]);
	ps2_buf[4] = ps2_transfer(ps2_buf[0]);
	ps2_buf[5] = ps2_transfer(ps2_buf[0]);
	ps2_buf[6] = ps2_transfer(ps2_buf[0]);
	ps2_buf[7] = ps2_transfer(ps2_buf[0]);
	ps2_buf[8] = ps2_transfer(ps2_buf[0]);
	PS2_ATT(1);
	return;
}

void ps2_init(void)
{
	ps2_write_read(ps2_buf);
}

void ps2_update(rc_ps2_t *rc_ps2)
{
	delay_us(20);
	ps2_write_read(ps2_buf);

	rc_ps2->LEFT_UP     = PS2_LEFT_UP;
	rc_ps2->LEFT_RIGHT  = PS2_LEFT_RIGHT;
	rc_ps2->LEFT_DOWN   = PS2_LEFT_DOWN;
	rc_ps2->LEFT_LEFT   = PS2_LEFT_LEFT;

	rc_ps2->SELECT 	    = PS2_SELECT;
	rc_ps2->START 	    = PS2_START;

	rc_ps2->RIGHT_UP    = PS2_RIGHT_UP;
	rc_ps2->RIGHT_RIGHT = PS2_RIGHT_RIGHT;
	rc_ps2->RIGHT_DOWN  = PS2_RIGHT_DOWN;
	rc_ps2->RIGHT_LEFT  = PS2_RIGHT_LEFT;

	rc_ps2->LEFT_2      = PS2_LEFT_2;
	rc_ps2->RIGHT_2     = PS2_RIGHT_2;
	rc_ps2->LEFT_1      = PS2_LEFT_1;
	rc_ps2->RIGHT_1     = PS2_RIGHT_1;

	rc_ps2->RIGHT_X     = PS2_RIGHT_X;
	rc_ps2->RIGHT_Y     = PS2_RIGHT_Y;
	rc_ps2->LEFT_X      = PS2_LEFT_X;
	rc_ps2->LEFT_Y      = PS2_LEFT_Y;

	return;
}


rc_ps2_t *get_remote_control_point(void)
{
    return &rc_ps2;
}
