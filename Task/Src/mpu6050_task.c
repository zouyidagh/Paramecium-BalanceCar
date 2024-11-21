#include "mpu6050_task.h"
#include "MPU6050.h"

void mpu6050Task(void *argument)
{
    int ErrorCode;
    struct MPU6050_Data{
        float pitch;
        float roll;
        float yaw;
    } data;
    // 初始化MPU6050
    // OLED_Init();
	ErrorCode = MPU6050_DMP_Init();
    // 如果初始化失败，显示错误信息
	if(ErrorCode != 0)
	{
    // OLED_Printf(1,1,"DMP Init ERR %d", ErrorCode);
		while(ErrorCode)
		{
      switch (ErrorCode)
      {
        case -1:
        //   OLED_ShowString(2,1,"MPU6050");
        //   OLED_ShowString(3,1,"Disconnected");
          break;
        case -9:
        //   OLED_ShowString(2,1,"Sensor Not Level");
          break;
        default:
        //   OLED_ShowString(2,1,"Unknown Error");
          break;
      }
			vTaskDelay(500);
			ErrorCode = MPU6050_DMP_Init();
		}
    // OLED_Clear();
	}
    while(1)
    {
        // 轮询MPU6050数据
        if(MPU6050_DMP_Get_Data(&data.pitch, &data.roll, &data.yaw))
        {
            // 显示数据
            // OLED_Printf(1, 1, "pitch: %.1f   ", data.pitch);
            // OLED_Printf(2, 1, "roll: %.1f   ", data.roll);
            // OLED_Printf(3, 1, "yaw: %.1f   ", data.yaw);
        }
    }
}