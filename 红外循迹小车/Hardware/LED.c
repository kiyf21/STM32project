//.c文件存放驱动程序的主体代码
#include "stm32f10x.h"                  // Device header

void LED_Init(void)//只要调用一个LED_Init函数，LED的两个GPIO口就直接初始化好了
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}

void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}

void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA ,GPIO_Pin_11);
}


//按键按下led状态取反
void LED1_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_11) == 0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
	}
	else
	{
		GPIO_ResetBits(GPIOA ,GPIO_Pin_11);
	}
}
	





