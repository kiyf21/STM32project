#include "systick.h"

static volatile uint32_t uwTick = 0;

/**
 * @brief TIM3 中断处理函数（直接定义在此，不依赖 stm32f10x_it.c）
 * @note  由于启动文件中已有弱定义，此处的定义会覆盖它，不会产生冲突
 */
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        uwTick++;  // 每 1ms 递增一次
    }
}

/**
 * @brief 初始化 TIM3 为 1ms 中断
 * @note  使用 TIM3 是因为它通常未被占用（TIM2 已用于电机 PWM）
 *        系统时钟 72MHz，APB1 预分频为 /2 时 TIM3 时钟为 72MHz，否则为 36MHz，需根据实际调整
 *        以下代码假设 TIM3 时钟为 72MHz（即 APB1 预分频不为 1）
 */
void SysTick_Init(void)
{
    // 使能 TIM3 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 配置定时器时基：1ms 中断一次
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 72 - 1;          // 计数值 72-1
    TIM_TimeBaseStructure.TIM_Prescaler = 1000 - 1;     // 预分频 1000-1
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 使能更新中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    // 配置 NVIC 中断优先级（使用 NVIC_PriorityGroup_2，需在 main 中先设置分组）
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 启动定时器
    TIM_Cmd(TIM3, ENABLE);
}

uint32_t GetTick(void)
{
    return uwTick;
}

void Delay_NonBlocking_Start(uint32_t *startVar, uint32_t ms)
{
    *startVar = GetTick() + ms;
}

uint8_t Delay_NonBlocking_Check(uint32_t *startVar)
{
    return (GetTick() >= *startVar);
}
