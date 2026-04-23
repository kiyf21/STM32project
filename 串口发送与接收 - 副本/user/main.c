#include "stm32f10x.h"  
#include "Delay.h"// Device header
#include "OLED.h"
#include "Key.h"
#include "Serial.h"

//uint8_t RxData;
uint8_t KeyNum;

int main(void)
{
	OLED_Init();
	
	//OLED_ShowChar(1,1,'A');
	//OLED_ShowString(1,3,"Hello world!");
	//OLED_ShowNum(2,1,12345,6);
	//OLED_ShowString(1,4,"RxData:");
	Key_Init ();
	OLED_ShowString (1,1,"TxPacket");
	OLED_ShowString (3,1,"RxPacket");
	
	Serial_Init();
	//Serial_SendByte (0x41);
	//Serial_SendString("Hello world!");
	//Serial_SendNumber (12345,5);
	//printf("Num=%d\r\n",666);
	
	//char String[100];
	//sprintf (String,"");
	//Serial_SendString (String );
	Serial_TxPacket[0] = 0X01;
	Serial_TxPacket[1] = 0X02;
	Serial_TxPacket[2] = 0X03;
	Serial_TxPacket[3] = 0X04;
	
	
	Serial_SendPacket();
	
	
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
		if(Serial_GetRxFlag()==1)
		{
			OLED_ShowHexNum(1,1,Serial_RxPacket[0],2);
			OLED_ShowHexNum(1,4,Serial_RxPacket[1],2);
			OLED_ShowHexNum(1,7,Serial_RxPacket[2],2);
			OLED_ShowHexNum(1,10,Serial_RxPacket[3],2);
		}
		
		
	}	
}
