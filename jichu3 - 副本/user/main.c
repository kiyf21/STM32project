#include "stm32f10x.h"  
#include "Delay.h"// Device header
#include "OLED.h"
#include "Key.h"
#include "Serial.h"


int main(void)
{
	// 初始化系统
    RCC_Configuration();
    GPIO_Configuration();
    USART_Configuration();
    
    // 使能USART1接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    // 优先级设置
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	USART_SendString(USART1, "STM32串口通信就绪\r\n");
	while(1)
	{
		if(reception_complete)
        {
            // 处理接收到的数据
            ProcessCommand(rx_buffer, rx_index);
            
            // 重置接收状态
            rx_index = 0;
            reception_complete = 0;
            memset(rx_buffer, 0, RX_BUFFER_SIZE);
        }
	}
    
}

