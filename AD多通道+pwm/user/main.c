#include "stm32f10x.h"  
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "PWM.h"
uint16_t AD0,AD1;
uint8_t i;
uint8_t j;


int main(void)
{
	OLED_Init();
	AD_Init();
	PWM_Init();
	OLED_ShowString(1,1,"AD0:");
	OLED_ShowString(2,1,"AD1:");
	OLED_ShowString(3,1,"DutyUp:");
	OLED_ShowString(4,1,"DutyDw:");
	i=100;
	j=10;
	//2171∂‘”¶1.75v
	//3722∂‘”¶3.00v
	while(1)
	{
		AD0 = AD_GetValue(ADC_Channel_0);
		AD1 = AD_GetValue(ADC_Channel_1);

		
		OLED_ShowNum(1,5,AD0,4);
		OLED_ShowNum(2,5,AD1,4);
		PWM_SetCompare4(j);
		PWM_SetCompare3(i);
		//…˝—π≈–∂œ
		if(AD0<=3720&&i>=5)
		{
			i--;
		}
		if(AD0>=3730&&i<=90)
		{
			i++;
		}
		//Ωµ—π≈–∂œ
		if(AD1>=2175&&j>=5)
		{
			j--;
		}
		if(AD1<=2165&&j<=90)
		{
			j++;
		}
		
		
		OLED_ShowNum(3,9,i,2);
		OLED_ShowNum(4,9,j,2);
		Delay_ms(10);
	}

}
