#include "stm32f10x.h"  
#include "Delay.h"// Device header
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"



//led和按键的驱动代码单独封装，单独放在.c.h文件里
int main(void)
{
	Buzzer_Init();
	LightSensor_Init();
	
	while(1)
	{
		if(LightSensor_Get () == 1)
		{
			Buzzer_ON();
		}
		else 
		{
			Buzzer_OFF(); 
		}
	}
}


