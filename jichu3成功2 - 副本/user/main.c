#include "stm32f10x.h"  
#include "Delay.h"// Device header
#include "OLED.h"
#include "Key.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	Serial_Init();
	

	while(1)
	{
		if(Serial_GetRxFlag()==1)
		{
			if(Serial_RxPacket[1] == 0X1)
			{
				printf("涵熙师兄举世无双\r\n");
			}
			else if(Serial_RxPacket[1] == 0X2)
			{
				printf("Cheri Wen 不是 Kris Wu\r\n");
			}
		}
		
	}	
}

