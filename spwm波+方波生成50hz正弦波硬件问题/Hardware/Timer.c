#include "stm32f10x.h"                  // Device header


void Timer_Init(void)
{
    // 1. 开启 TIM3 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 2. 选择内部时钟
    TIM_InternalClockConfig(TIM3);

    // 3. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // 以下参数需根据你需要的 SPWM 频率计算，这里给出示例（目标 50Hz，64 点）
    // 更新频率 = 50 * 64 = 3200Hz，72MHz / (PSC+1) / (ARR+1) = 3200
    // 取 PSC = 0，则 ARR+1 = 72MHz / 3200 = 22500，所以 ARR = 22500-1
    TIM_TimeBaseInitStructure.TIM_Period =22500 - 1;   // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;        // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    // 4. 清除更新标志并开启更新中断
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    // 5. 配置 NVIC（仿照你的写法）
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   // 全局分组，若已配置可省略

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    // 6. 使能 TIM3 计数器
    TIM_Cmd(TIM3, ENABLE);

}

