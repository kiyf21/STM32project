#include "stm32f10x.h"                  // Device header
// 定时器4 初始化 (1µs 计数)
void Timer_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // 系统时钟 72MHz, 预分频 72-1 = 71 -> 1MHz 计数 (1µs)
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;      // 最大计数值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    
    TIM_Cmd(TIM4, ENABLE);   // 启动定时器
}

// 获取当前微秒数
uint32_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM4);
}

