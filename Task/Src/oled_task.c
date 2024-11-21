#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "oled_task.h"
#include "OLED.h"

extern osMessageQueueId_t oledDisplayQueueHandle;

void oledDisplayTask(void *argument)
{
    OLED_Message_t msg;
    OLED_Init();
    
    while(1)
    {
        if(osMessageQueueGet(oledDisplayQueueHandle, &msg, NULL, portMAX_DELAY) == osOK)
        {
            switch(msg.command)
            {
                case clear:
                    OLED_Clear();
                    break;
                case showChar:
                    OLED_ShowChar(msg.Line, msg.Column, msg.Data.Char);
                    break;
                case showString:
                    OLED_ShowString(msg.Line, msg.Column, msg.Data.String);
                    break;
                case showNum:
                    OLED_ShowNum(msg.Line, msg.Column, msg.Data.Number, msg.Length);
                    break;
                case showSignedNum:
                    OLED_ShowSignedNum(msg.Line, msg.Column, msg.Data.SignedNumber, msg.Length);
                    break;
                case showHexNum:
                    OLED_ShowHexNum(msg.Line, msg.Column, msg.Data.Number, msg.Length);
                    break;
                case showBinNum:
                    OLED_ShowBinNum(msg.Line, msg.Column, msg.Data.Number, msg.Length);
                    break;
                case printf:
                    OLED_Printf(msg.Line, msg.Column, msg.Data.Format, msg.Format_float_value);
                    break;
                case showAngle:
                    OLED_ShowNum(msg.Line, msg.Column, (uint32_t)(msg.Data.Angle * 100), msg.Length);
                    break;
                case invertLine:
                    OLED_InvertLine(msg.Line);
                    break;
                default:
                    break;
            }
        }
    }
}