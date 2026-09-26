#include "stm32f10x.h"  
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "PWM.h"
uint16_t ADValue;
float Voltage;
uint8_t i;

int main(void)
{
	OLED_Init();
	AD_Init();
	PWM_Init();
	OLED_ShowString(1,1,"ADValue:");
	OLED_ShowString(2,1,"Voltage:0.00");
	OLED_ShowString(3,1,"Duty:");
	i=10;
	while(1)
	{
		ADValue = AD_GetValue();
		PWM_SetCompare4(i);
		
		Voltage = (float)ADValue / 4095 * 3.3;
		
		if(Voltage>=2.55&&i>=5)
		{
			i--;
		}
		if(Voltage<=2.45&&i<=90)
		{
			i++;
		}
			
		
		
		
		
		
		
		
		
		
		
		
		OLED_ShowNum(1,9,ADValue,4);
		OLED_ShowNum(2,9,Voltage,1);
		OLED_ShowNum(2,11,(uint16_t)(Voltage*100)%100,2);
		OLED_ShowNum(3,9,i,2);
		Delay_ms(10);
	}

}
