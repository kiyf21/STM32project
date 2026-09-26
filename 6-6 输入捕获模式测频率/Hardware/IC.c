#include "stm32f10x.h"                  // Device header

void IC_Init()
{
	//打开时钟，定时器内部的所有时序逻辑都需要时钟驱动
	//芯片默认关闭外设时钟以降低功耗，需要时由软件按需开启。
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//配置输出PWM的GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	
	GPIO_InitTypeDef GPIO_Initstructure;

	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	
	
	
	//根据上述配置初始化GPIO。
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	
	
	TIM_InternalClockConfig(TIM3);//使用内部时钟，初始化时基单元
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//设置计数模式为向上计数，即从0计数到ARR值后溢出并重新从0开始。
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//设置自动重装载寄存器（ARR）的值为99。
	//即计数从0到99，共100个计数脉冲，决定PWM周期。
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;      //ARR的值
	//设置预分频器（PSC）的值为71。
	//定时器的实际计数频率 = 72MHz / (PSC+1) = 72MHz / 72 = 1000kHz。
	TIM_TimeBaseInitStructure.TIM_Prescaler =72-1;     //PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	
	//根据上述配置初始化TIM2的时基单元。
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	//初始化输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM3,ENABLE);
	
	
}
	
uint32_t IC_GetFreq(void)
{
	return 1000000/(TIM_GetCapture1(TIM3)+1);
}

