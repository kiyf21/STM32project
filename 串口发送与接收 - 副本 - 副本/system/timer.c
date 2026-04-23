#include "stm32f10x.h" 



void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE );
	
	
	TIM_InternalClockConfig(TIM2 );
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = ;
	TIM_TimeBaseInitStructure.TIM_CounterMode = ;
	TIM_TimeBaseInitStructure.TIM_Prescaler = ;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = ;
	TIM_TimeBaseInit(TIM2 ,&TIM_TimeBaseInitStructure);
	
	
	
	
}
