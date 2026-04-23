#include "stm32f10x.h"
#include "OLED.h"
#include "Exit_Key.h"
#include "Delay.h"
#include "Remote.h"
#include "Timer.h"

int16_t Num;


int main(void)
{
    OLED_Init();
	
	//OLED_ShowString(1,1,"a");
	Exti_Key_Init();
	OLED_ShowString(1,1,"Num:");
	OLED_ShowString(3,1,"RemKey:");
	Timer_Init();



    while(1)
    {
		//Num = Get_KeyCount();
		OLED_ShowNum(3,8,Remote_GetKey(),8);
		OLED_ShowHexNum(4,8,EXTI_GetITStatus(EXTI_Line0),2);
		OLED_ShowNum(2,1,Timer_GetCounter(),10);
		OLED_ShowSignedNum(1,6,Num,5);
		Delay_ms(100);
    }
}
