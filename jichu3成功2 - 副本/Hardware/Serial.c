#include "stm32f10x.h"                  // Device header
#include <stdio.h>


uint8_t Serial_TxPacket[2];
uint8_t Serial_RxPacket[2];
uint8_t Serial_RxFlag;

//初始化Serial
void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	
	USART_ITConfig (USART1 ,USART_IT_RXNE,ENABLE );
	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_Init(&NVIC_InitStructure);
	
	
	USART_Cmd(USART1,ENABLE);

}


void Serial_SendByte(uint8_t Byte)
{
	USART_SendData (USART1 ,Byte);
	while (USART_GetFlagStatus (USART1 ,USART_FLAG_TXE )==RESET );
}

// 串口发送字节数组
void Serial_SendArray(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for(i=0;i <Length;i++)
	{
		Serial_SendByte(Array[i]);
		
	}
}

// 串口发送字符串
void Serial_SendString(char*String)
{
	uint8_t i;
	for(i=0;String[i]!='\0';i++)
	{
		Serial_SendByte(String[i]);

	}

}


uint32_t Serial_pow(uint32_t X,uint32_t  Y)
{
	uint32_t Result=1;
	while(Y--)
		{
		Result *=X;
		}
		return Result ;
}


int fputc(int ch, FILE *f)
{
	Serial_SendByte (ch);
	return ch;
}
	

void Serial_SendPacket(void)
{
	Serial_SendByte(0XFF);
	Serial_SendArray (Serial_TxPacket,3);
	Serial_SendByte (0X55);
}
	

uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag ==1)
	{
		Serial_RxFlag =0;
		return 1;
	}
	return 0;
}


void USART1_IRQHandler(void)
{
	static  uint8_t RxState=0;
	static  uint8_t pRxPacket=0;
	if(USART_GetITStatus (USART1 ,USART_IT_RXNE )==SET)
	{
		uint8_t RxData =USART_ReceiveData (USART1 );
		
		if(RxState ==0)
		{
			if(RxData  ==0XFF)
			{
				RxState  =1;
				pRxPacket = 0;
			}
		}
		else if(RxState == 1)
		{
			Serial_RxPacket [pRxPacket]=RxData ;
			pRxPacket ++;
			if(pRxPacket >=2)
			{
				RxState =2;
			}
		}
		else if(RxState == 2)
		{
			//当为空格或换行结束
			if (RxData  == 0X55)
			{
				RxState =0;
				Serial_RxFlag =1;
			}
		}
		USART_ClearITPendingBit (USART1 ,USART_IT_RXNE );
	}
}
