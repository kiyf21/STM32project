#include "stm32f10x.h"  
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "Key.h"
uint8_t KeyNum;
float Angle;



int main(void)
{
	OLED_Init();
	Servo_Init();
	
	Key_Init();
	
	OLED_ShowString(1,1,"Angle:");
	Angle = 0;
	Servo_SetAngle(Angle);
	OLED_ShowNum(1,7,Angle,3);
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			Angle += 30;
			if(Angle >= 210)
			{
				Angle = 0;
			}
			Servo_SetAngle(Angle);
			//Servo_SetAngle1(Angle);
			OLED_ShowNum(1,7,Angle,3);
		}			
	}

}
