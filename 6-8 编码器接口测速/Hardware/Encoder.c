#include "stm32f10x.h"                  // Device header


void Encoder_Init(void)
{
	//打开时钟，定时器内部的所有时序逻辑都需要时钟驱动
	//芯片默认关闭外设时钟以降低功耗，需要时由软件按需开启。
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//配置输出PWM的GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	
	GPIO_InitTypeDef GPIO_Initstructure;

	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	
	
	
	//根据上述配置初始化GPIO。
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	

	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//设置计数模式为向上计数，即从0计数到ARR值后溢出并重新从0开始。
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//设置自动重装载寄存器（ARR）的值为99。
	//即计数从0到99，共100个计数脉冲，决定PWM周期。
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;      //ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler =1-1;     //PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	//根据上述配置初始化TIM2的时基单元。
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	
	//初始化输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);

	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3,ENABLE);
}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return Temp;
}
