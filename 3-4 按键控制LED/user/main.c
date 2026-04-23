#include "stm32f10x.h"  
#include "Delay.h"// Device header
#include "LED.h"
#include "Key.h"


uint8_t KeyNum;


//led和按键的驱动代码单独封装，单独放在.c.h文件里
int main(void)
{
	LED_Init();
	Key_Init();
	
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum ==1)
		{
			LED1_Turn();
			
		}
		if(KeyNum == 2)
		{
			LED2_Turn();
		}
		
	}


}
