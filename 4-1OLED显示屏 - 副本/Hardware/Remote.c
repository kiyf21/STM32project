#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "Delay.h"
#include "OLED.h"
#include "LED.h"

//我的遥控接在PB0口，使用外部中断
void EXTI0_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能 GPIOB 和 AFIO 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    // PB0 上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 将 PB0 连接到 EXTI0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);

    // EXTI0 配置：下降沿触发（红外信号下降沿开始）
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // NVIC 配置（优先级分组需在 main 中设置）
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


// 全局变量（需要加 volatile，因为在中断和主循环中都被访问）
volatile uint32_t cnt = 0;
volatile int16_t sign = 0;   // 用 int16_t 可以返回 -1 或计数值

void EXTI0_IRQHandler(void)
{
	LED1_Turn();
    if (EXTI_GetITStatus(EXTI_Line0) == RESET) return;

    cnt = 0;   // 每次下降沿开始，重新计数

    // 等待引脚变为高电平（即低电平结束）
    while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)   // 每次循环都重新读取引脚
    {
        Delay_us(10);   // 延时 10µs
        cnt++;
        if (cnt > 1000)   // 超过 10ms（1000 * 10µs = 10ms）
        {
            sign = -1;    // 超时标志
            EXTI_ClearITPendingBit(EXTI_Line0);
            return;       // 直接退出中断，不再继续等待
        }
    }

    // 正常退出循环（引脚已变高），cnt 即为低电平持续的时间（单位 10µs）
    sign = cnt;   // 例如 cnt = 900 表示低电平持续了 9ms（引导码）

    EXTI_ClearITPendingBit(EXTI_Line0);
}




