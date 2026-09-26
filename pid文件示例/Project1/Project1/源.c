#include <stdio.h>
typedef struct {
    float Kp;            // 比例系数
    float Ki;            // 积分系数
    float Kd;            // 微分系数
    float integral;      // 积分累计
    float last_error;    // 上一次误差
    float output;        // 输出值
    float max_output;    // 最大输出限制
    float min_output;    // 最小输出限制
} PID_Controller;

// PID初始化
void PID_Init(PID_Controller* pid, float Kp, float Ki, float Kd, float max, float min) {
    pid->Kp = Kp;           //Kp,Ki,Kd,为提前调好的常数
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->integral = 0.0f;   // 积分累计
    pid->last_error = 0.0f; // 上一次误差
    pid->output = 0.0f;     // 输出值       //自己设置初始值（？）
    pid->max_output = max;  // 最大输出限制 //自己设置初始值
    pid->min_output = min;  // 最小输出限制 //自己设置初始值
}

// PID计算函数
float PID_Calculate(PID_Controller* pid, float setpoint, float feedback) 
{
    float error = setpoint - feedback;//设定值（目标电压）-反馈值（当前电压） （用于比例系数）
    float derivative = error - pid->last_error;//更新误差差值                 （用于微分系数）
    pid->integral += error;//积分累计由于经过震荡误差，此目标值在理想时为零   （用于积分系数）

    // 限制积分项范围，防止积分饱和
    if (pid->integral > pid->max_output / pid->Ki) {
        pid->integral = pid->max_output / pid->Ki;
    }
    else if (pid->integral < pid->min_output / pid->Ki) {
        pid->integral = pid->min_output / pid->Ki;
    }

    //若上下限都无超出，则设值pid值
    pid->output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;

    // 输出限幅
    if (pid->output > pid->max_output) {
        pid->output = pid->max_output;
    }
    else if (pid->output < pid->min_output) {
        pid->output = pid->min_output;
    }
    pid->last_error = error;

    return pid->output;
}