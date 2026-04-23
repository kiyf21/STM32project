#include "stm32f10x.h"                  // Device header
#include "PWM.h"


void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	PWM_Init();
	
}

void LMotor_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);

		GPIO_ResetBits(GPIOA,GPIO_Pin_5);

		PWM_SetCompare3(Speed);
	}
}

void RMotor_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)
	{
		
		GPIO_SetBits(GPIOA,GPIO_Pin_6);//
		
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);//
		
		PWM_SetCompare4(Speed);
	}
}
