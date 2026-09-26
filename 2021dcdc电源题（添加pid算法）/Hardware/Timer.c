#include "stm32f10x.h"

void TIM1_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 开启 TIM1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // 2. 配置时基
    TIM_TimeBaseStructure.TIM_Period = 3600 - 1;        // ARR = 999
    TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1;       // PSC = 71，得到 1MHz 计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // 3. 使能 TIM1 更新中断
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    // 4. 配置 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;          // TIM1 更新中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 5. 启动定时器
    TIM_Cmd(TIM1, ENABLE);
}
