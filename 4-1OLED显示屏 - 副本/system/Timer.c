#include "stm32f10x.h"                  // Device header

/*
 初始化 TIM4 为 1µs 计数模式（系统时钟 72MHz）
*/
void Timer_Init(void)
{
    // 使能 TIM4 时钟（TIM4 位于 APB1 总线上）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // 预分频：72-1 = 71，得到 1MHz 计数时钟（1µs 递增一次）
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;      // 最大计数值 65535
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // 启动定时器
    TIM_Cmd(TIM4, ENABLE);
}

/*
获取当前微秒计数值
*/
uint32_t Timer_GetMicros(void)
{
    return TIM_GetCounter(TIM4);
}
