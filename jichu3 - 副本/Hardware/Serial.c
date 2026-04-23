#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <string.h>

//uint8_t Serial_TxPacket[2];
//uint8_t Serial_RxPacket[2];
//uint8_t Serial_RxFlag;


#define RX_BUFFER_SIZE 64
uint8_t rx_buffer[RX_BUFFER_SIZE];
uint16_t rx_index = 0;
uint8_t reception_complete = 0;

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

// 串口发送字符串
void USART_SendString(USART_TypeDef* USARTx, char *str)
{
    while(*str)
    {
        USART_SendData(USARTx, *str++);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    }
}

// 串口发送字节数组
void USART_SendBytes(USART_TypeDef* USARTx, uint8_t *data, uint16_t len)
{
    for(uint16_t i = 0; i < len; i++)
    {
        USART_SendData(USARTx, data[i]);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    }
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t received_data = USART_ReceiveData(USART1);
        
        // 将数据存入缓冲区
        if(rx_index < RX_BUFFER_SIZE - 1)
        {
            rx_buffer[rx_index++] = received_data;
            
            // 检测回车或换行作为命令结束符
            if(received_data == '\r' || received_data == '\n' || rx_index >= RX_BUFFER_SIZE - 1)
            {
                reception_complete = 1;
            }
        }
        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

void ProcessCommand(uint8_t *data, uint16_t len)
{
    // 十六进制命令1: 0XFF 0XFF 0X1 0X55
    if(len >= 4 && data[0] == 0xFF && data[1] == 0xFF && data[2] == 0x01 && data[3] == 0x55)
    {
        USART_SendString(USART1, "涵熙师兄举世无双\r\n");
    }
    // 十六进制命令2: 0XFF 0XFF 0X2 0X55
    else if(len >= 4 && data[0] == 0xFF && data[1] == 0xFF && data[2] == 0x02 && data[3] == 0x55)
    {
        USART_SendString(USART1, "Cheri Wen 不是 Kris Wu\r\n");
    }
    // 字符串命令: "anshen666"
    else if(len >= 9 && strncmp((char*)data, "anshen666", 9) == 0)
    {
        USART_SendString(USART1, "安神666\r\n");
    }
    // 字符串命令: "东旭666"
    else if(len >= 7) // 中文字符需要更多字节
    {
        // 简单的中文字符串匹配（实际项目中建议使用更完善的中文处理）
        uint8_t dongxu_cmd[] = {0xE4, 0xB8, 0x9C, 0xE6, 0x97, 0xAD, 0x36, 0x36, 0x36}; // "东旭666"的UTF-8编码
        int match = 1;
        for(int i = 0; i < 9 && i < len; i++)
        {
            if(data[i] != dongxu_cmd[i])
            {
                match = 0;
                break;
            }
        }
        if(match)
        {
            USART_SendString(USART1, "Dongxu666\r\n");
        }
        else
        {
            USART_SendString(USART1, "未知命令\r\n");
        }
    }
    else
    {
        USART_SendString(USART1, "未知命令\r\n");
    }
}