#include "stm32f10x.h"  
#include "Delay.h"// Device header
#include "OLED.h"
#include "Key.h"
#include "Serial.h"

uint8_t RxData;


int main(void)
{
	OLED_Init();
	
	//OLED_ShowChar(1,1,'A');
	//OLED_ShowString(1,3,"Hello world!");
	//OLED_ShowNum(2,1,12345,6);
	OLED_ShowString(1,4,"RxData:");
	
	Serial_Init();
	//Serial_SendByte (0x41);
	//Serial_SendString("Hello world!");
	//Serial_SendNumber (12345,5);
	//printf("Num=%d\r\n",666);
	
	//char String[100];
	//sprintf (String,"");
	//Serial_SendString (String );
	
	while(1)
	{
		/*if(Serial_GetRxFlag() ==1)
		{
			RxData =Serial_GetRxData ();
			OLED_ShowHexNum (1,1,RxData,2);
			if(RxData ==0X1)
				{
				printf("涵熙师兄举世无双\r\n");
			}
			if(RxData ==0X2)
				{
				printf("Cheri Wen 不是 Kris Wu\r\n");
			}
			
		}*/
		
		
		
		
		
		
		
		
		
		
		
		
		
	}	
}
