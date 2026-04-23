#include "stm32f10x.h"  
#include "Delay.h"// Device header
#include "OLED.h"
#include "Key.h"
#include "Serial.h"


int main(void)
{
	OLED_Init();
	
	//OLED_ShowChar(1,1,'A');
	//OLED_ShowString(1,3,"Hello world!");
	//OLED_ShowNum(2,1,12345,6);
	
	
	Serial_Init();
	//Serial_SendByte (0x41);
	//Serial_SendString("Hello world!");
	//Serial_SendNumber (12345,5);
	//printf("Num=%d\r\n",666);
	
	char String[100];
	sprintf (String,"");
	Serial_SendString (String );
	
	while(1)
	{
		
	}


}
