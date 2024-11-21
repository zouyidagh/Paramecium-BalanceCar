#include "main.h"
#include "mpu6050_task.h"
#include "MPU6050.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "oled_task.h"
#include "queue.h"

extern osMessageQueueId_t oledDisplayQueueHandle;
extern osMessageQueueId_t mpu6050DataQueueHandle;

void mpu6050Task(void *argument)
{
    int ErrorCode;
    MPU6050_Data_t data;
    OLED_Message_t msg;
    
    ErrorCode = MPU6050_DMP_Init();
    if(ErrorCode != 0)
    {
        msg.command = printf;
        msg.Line = 1;
        msg.Column = 1;
        msg.Data.Format = "DMP Init ERR %.0f";
        msg.Format_float_value = ErrorCode;
        osMessageQueuePut(oledDisplayQueueHandle, &msg, 0, 0);
        
        while(ErrorCode)
        {
            switch (ErrorCode)
            {
                case -1:
                    msg.command = showString;
                    msg.Line = 2;
                    msg.Column = 1;
                    msg.Data.String = "MPU6050";
                    osMessageQueuePut(oledDisplayQueueHandle, &msg, 0, 0);
                    
                    msg.Line = 3;
                    msg.Data.String = "Disconnected";
                    osMessageQueuePut(oledDisplayQueueHandle, &msg, 0, 0);
                    break;
                case -9:
                    msg.command = showString;
                    msg.Line = 2;
                    msg.Column = 1;
                    msg.Data.String = "Sensor Not Level";
                    osMessageQueuePut(oledDisplayQueueHandle, &msg, 0, 0);
                    break;
                default:
                    msg.command = showString;
                    msg.Line = 2;
                    msg.Column = 1;
                    msg.Data.String = "Unknown Error";
                    osMessageQueuePut(oledDisplayQueueHandle, &msg, 0, 0);
                    break;
            }
            vTaskDelay(500);
            ErrorCode = MPU6050_DMP_Init();
        }
        msg.command = clear;
        osMessageQueuePut(oledDisplayQueueHandle, &msg, 0, 0);
    }
    
    while(1)
    {
        if(MPU6050_DMP_Get_Data(&data.pitch, &data.roll, &data.yaw))
        {
            xQueueOverwrite((QueueHandle_t)mpu6050DataQueueHandle, &data);

            msg.command = printf;
            msg.Column = 1;
            
            msg.Line = 1;
            msg.Data.Format = "roll: %.1f   ";
            msg.Format_float_value = data.roll;
            osMessageQueuePut(oledDisplayQueueHandle, &msg, 0, 0);
            
            msg.Line = 2;
            msg.Data.Format = "yaw: %.1f   ";
            msg.Format_float_value = data.yaw;
            osMessageQueuePut(oledDisplayQueueHandle, &msg, 0, 0);
        }
    }
}