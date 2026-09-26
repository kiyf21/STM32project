#include "stm32f10x.h"  
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "PWM.h"
#include "PID.h"
#include "Timer.h"

PID_Controller pid;   // 定义结构体变量（全局或局部均可）

uint16_t ADValue;
float Voltage;//电压
uint16_t duty;
int dpid;
//int point =2689;
int point =1689;
//void PID_Init(PID_Controller* pid, float Kp, float Ki, float Kd, float max, float min);
//float PID_Calculate(PID_Controller* pid, float setpoint, float feedback);

int main(void)
{
	OLED_Init();
	AD_Init();
	PWM_Init();
	TIM1_Init();
	PID_Init(&pid,0.1,0.01,0.01,150.0,-250.0);
	OLED_ShowString(1,1,"ADValue:");
	OLED_ShowString(2,1,"Voltage:00.00");
	OLED_ShowString(3,1,"Duty:");
	duty = 2800;
	while(1)
	{
		Voltage = (float)ADValue / 4096 * 99;
		OLED_ShowNum(2,9,Voltage,2);
		OLED_ShowNum(2,12,(uint16_t)(Voltage*100)%100,2);
		
		OLED_ShowNum(1,9,ADValue,4);

		OLED_ShowSignedNum(3,9,duty,4);
		OLED_ShowSignedNum(4,9,dpid,4);
		
	}

}


void TIM1_UP_IRQHandler(void)
{
    // 检查是否为 TIM1 更新中断
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);   // 清除中断标志位
		//AD采集（1ms）
		ADValue = AD_GetValue();//0-4095
//		//PID计算
//		//目标值为1817
		dpid = (int)PID_Calculate(&pid,point,ADValue);
		

//		//设置pwm(降压为负号）
		if(duty-dpid>=2500&&duty-dpid<=3000)
		{
			
			duty=duty-dpid;
			PWM_SetCompare4(duty);
		}
        // 在这里添加你的 1ms 处理代码

    }
}

