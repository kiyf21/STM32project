#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint16_t keyCount = 0;

void Exti_Key_Init(void)
{
	//此按键引脚接在了pb0
	//故使用PB0对应的中断线和中断处理函数：EXTI0_IRQHandler。
	//初始化代码：GPIO配置、EXTI配置、NVIC配置。
	
	//初始化GPIO配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//开启AFIO时钟
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;//由于是外部中断所以配置为上拉输入，按键接GND，按下为低电平
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
	
	//初始化EXIT配置
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);//将 PB0 连接到 EXTI0
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	//初始化配置NVIC
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组设置为2）
	
	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



/*			void EXTI0_IRQHandler(void)
				{
					if (EXTI_GetITStatus(EXTI_Line0) != RESET)
					{ 
						// 假设你有毫秒级阻塞延时函数
					   
						keyCount++;

						
						EXTI_ClearITPendingBit(EXTI_Line0);
					}
				}

				uint16_t Get_KeyCount(void)
				{
					return keyCount;
				}

*/

