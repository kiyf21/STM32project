#include "stm32f10x.h"  
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"




int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1,1,"Freq:00000Hz");
	OLED_ShowString(2,1,"Duty:00%");
	PWM_SetPrescaler(720-1);//PSC=720-1        //72MHz / (PSC+1) = 72MHz / 720 = 100kHz。
	PWM_SetCompare1(50);//占空比为50%           //占空比Duty = CCR / 100
	
	while(1)
	{
		OLED_ShowNum(1,6,IC_GetFreq(),5);
		
		OLED_ShowNum(2,6,IC_GetDuty(),2);
	}

}
