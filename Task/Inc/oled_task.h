#ifndef __OLED_TASK_H
#define __OLED_TASK_H

typedef enum
{
    clear = 0,
    showChar,
    showString,
    showNum,
    showSignedNum,
    showHexNum,
    showBinNum,
    printf,
    showAngle,
    invertLine
} OLED_Command_t;

typedef struct
{
    OLED_Command_t command;
    uint8_t Line;
    uint8_t Column;
    union
    {
        char Char;
        char *String;
        uint32_t Number;
        int32_t SignedNumber;
        float Angle;
        const char *Format;
    } Data;
    float Format_float_value;   // 用于printf格式化输出
    uint8_t Length;
} OLED_Message_t;

#endif // __OLED_TASK_H