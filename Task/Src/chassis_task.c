#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "chassis_task.h"
#include "OLED.h"
#include "motor.h"
#include "encoder.h"
#include "alg_pid.h"
#include "mpu6050_task.h"
#include "ps2_task.h"
#include "queue.h"

//速度环PID参数
#define SPEED_KP 0.2f
#define SPEED_KI 0.01f
#define SPEED_KD 0.0f
#define SPEED_MAX_OUT 30.0f
#define SPEED_MAX_IOUT 15.0f
//垂直环PID参数
#define VERTICAL_KP 350.0f
#define VERTICAL_KI 0.0f
#define VERTICAL_KD 450.0f
#define VERTICAL_MAX_OUT 7200.0f
#define VERTICAL_MAX_IOUT 0.0f
//转向环PID参数
#define TURN_KP 10.0f
#define TURN_KI 0.0f
#define TURN_KD 1.0f
#define TURN_MAX_OUT 3000.0f
#define TURN_MAX_IOUT 0.0f
//控制量限幅
#define TARGET_SPEED_MAX 50
#define TARGET_TURN_MAX 1000

//功能宏定义
#define ABS(x) ((x) > 0 ? (x) : -(x))

//变量定义
uint8_t stop;   //紧急停止标志位
chassisControl_str_t chassisControl_str;
MPU6050_Data_t mpuData;
rc_ps2_t *controllerData_p;
int16_t Target_Speed, Target_turn;
int Err_Speed, Err_lowout, Err_lowout_last;
int motor1_out, motor2_out;

//队列句柄声明
extern osMessageQueueId_t mpu6050DataQueueHandle;
extern osMessageQueueId_t ps2DataQueueHandle;

//函数声明
void pid_init(void);
void get_controller_cmd(rc_ps2_t *controllerData_p);
void emergency_stop(float *Angle);

//底盘控制任务
void chassisControlTask(void *argument)
{
    TickType_t wakeTime = xTaskGetTickCount();  //获取启动时间
    motor_init();
    pid_init();
    while(1)
    {
        //获取数据
        xQueuePeek((QueueHandle_t)mpu6050DataQueueHandle, &mpuData, 0); //姿态
        if(osMessageQueueGet(ps2DataQueueHandle, &controllerData_p, 0, 0) == osOK)  //遥控器
        {
            get_controller_cmd(controllerData_p);
        }
        else    //无遥控器数据,停止
        {
            Target_Speed = 0;
            Target_turn = 0;
        }
        chassisControl_str.Speed_L = Read_Speed(&htim2);    //左轮速度
        chassisControl_str.Speed_R = -Read_Speed(&htim4);   //右轮速度

        //PID计算
        Err_Speed = chassisControl_str.Speed_L + chassisControl_str.Speed_R;
        Err_lowout = (1 - 0.7) * Err_Speed + 0.7 * Err_lowout_last; //低通滤波
        Err_lowout_last = Err_lowout;
        PID_calc(&chassisControl_str.pid_speed, (fp32)(Err_lowout), (fp32)Target_Speed); //速度环
        PID_calc(&chassisControl_str.pid_vertical, (fp32)mpuData.roll, -chassisControl_str.pid_speed.out); //垂直环
        motor1_out = -chassisControl_str.pid_vertical.out + Target_turn;
        motor2_out = -chassisControl_str.pid_vertical.out - Target_turn;

        //电机输出
        Limit(&motor1_out, &motor2_out);
        Load(motor1_out, motor2_out);
        emergency_stop(&mpuData.roll);  //倾倒检测

        //任务周期结束
        vTaskDelayUntil(&wakeTime, 10); //任务定周期10ms运行
    }
}

//函数定义
/**
 * @brief          PID参数初始化
 */
void pid_init(void)
{
    const static fp32 pid_speed[3] = {SPEED_KP, SPEED_KI, SPEED_KD};
    const static fp32 pid_vertical[3] = {VERTICAL_KP, VERTICAL_KI, VERTICAL_KD};
    const static fp32 pid_turn[3] = {TURN_KP, TURN_KI, TURN_KD};
    PID_init(&chassisControl_str.pid_speed, PID_POSITION, pid_speed, SPEED_MAX_OUT, SPEED_MAX_IOUT);
    PID_init(&chassisControl_str.pid_vertical, PID_POSITION, pid_vertical, VERTICAL_MAX_OUT, VERTICAL_MAX_IOUT);
    PID_init(&chassisControl_str.pid_turn, PID_POSITION, pid_turn, TURN_MAX_OUT, TURN_MAX_IOUT);
}

void get_controller_cmd(rc_ps2_t *controllerData_p)
{
    static float yaw_last, yaw_delta;
    static uint8_t yaw_lock;    //偏航锁定标志位
    if(ABS(controllerData_p->RIGHT_Y) < 10)
    {
        Target_Speed = 0;
    }
    else
    {
        Target_Speed = controllerData_p->RIGHT_Y * TARGET_SPEED_MAX / 128;
    }
    if(ABS(controllerData_p->RIGHT_X) < 10)
    {
        //如果没有转向指令，开启转向环保持朝向
        if(yaw_lock == 0)
        {
            yaw_last = mpuData.yaw;
            yaw_lock = 1;
        }
        else
        {
            yaw_delta = mpuData.yaw - yaw_last;
            if(yaw_delta > 180)
            {
                yaw_delta -= 360;
            }
            else if(yaw_delta < -180)
            {
                yaw_delta += 360;
            }
            PID_calc(&chassisControl_str.pid_turn, yaw_delta, 0);
            Target_turn = chassisControl_str.pid_turn.out;
        }
    }
    else
    {
        yaw_lock = 0;
        PID_clear(&chassisControl_str.pid_turn);
        Target_turn = controllerData_p->RIGHT_X * TARGET_TURN_MAX / 128;
    }
}

/**
 * @brief 倾倒检测
 * @param Angle 当前倾角
 */
void emergency_stop(float *Angle)
{
	if(ABS(*Angle) > 60)
	{
		Load(0,0);
		stop=1;
        PID_clear(&chassisControl_str.pid_speed);
        PID_clear(&chassisControl_str.pid_vertical);
        PID_clear(&chassisControl_str.pid_turn);
	}
}
