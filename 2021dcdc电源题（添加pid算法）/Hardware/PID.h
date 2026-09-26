#ifndef __PID_H
#define __PID_H

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

void PID_Init(PID_Controller* pid, float Kp, float Ki, float Kd, float max, float min);
float PID_Calculate(PID_Controller* pid, float setpoint, float feedback);

#endif
