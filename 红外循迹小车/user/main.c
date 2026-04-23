#include "stm32f10x.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "Motor.h"
#include "CountSensor.h"
#include "Sensor.h"
#include "systick.h"   // 包含 GetTick() 和 SysTick_Init()

uint8_t KeyNum;


int main(void)
{
    SysTick_Init();          // 初始化1ms系统时钟
    OLED_Init();
    Motor_Init();
    CountSensor_Init();
    Sensor_Init();
	Key_Init();
	LED_Init();

    OLED_ShowString(1,1,"L&RSpeed:");
	OLED_ShowString(2,1,"state:");
    OLED_ShowString(3,1,"GetTick:");
    OLED_ShowString(4,1,"L&RNum:");

    uint8_t turning = 0;          // 0:直行, 1:右转, 2:左转
    uint32_t turnStartTime = 0;
    int8_t LSpeed, RSpeed;

    while(1)
    {
		KeyNum = Key_GetNum();

		if(KeyNum ==1)
		{
			LED1_Turn();	
		}

		// 实时读取传感器（假设检测到黑线为0，白线为1）
		uint8_t LNum = LSensor_Get();
		uint8_t RNum = RSensor_Get();

		if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_11) == 1)
		{
			OLED_ShowString(2,8,"ON ");
			
			if (turning == 0)  // 直行状态
			{
				if (LNum == 1 && RNum == 0)    //left  
				{
					turning = 1;
					turnStartTime = GetTick();
					LSpeed = 50; 
					RSpeed = 100;
				}
				else if (LNum == 0 && RNum == 1) //right
				{
					turning = 2;
					turnStartTime = GetTick();
					LSpeed = 100; 
					RSpeed = 50;
				}
				else if (LNum == 1 && RNum == 1) 
				{
					LSpeed = 0; 
					RSpeed = 0;
				}
				else                              
				{
					LSpeed = 100; 
					RSpeed = 100;
				}
			 }
			 else  // 转弯状态
			 {
					// 保持转弯速度（1秒内不变）
					if (turning == 1)      
					{
						LSpeed = 50; 
						RSpeed = 100;
					}
					else if (turning == 2) 
					{
						LSpeed = 100; 
						RSpeed = 50;
					}

					// 检查是否达到1秒，若到期则退出转弯
					if (GetTick() - turnStartTime >= 1000)
					{
						turning = 0;   // 下一循环将根据传感器重新判断
					}
			  }
				
		}
		else
		{
			LSpeed = 0;
			RSpeed = 0;
			OLED_ShowString(2,8,"OFF");
		}
   
        // 应用速度
        LMotor_SetSpeed(LSpeed);
        RMotor_SetSpeed(RSpeed);

        // 刷新OLED显示
        OLED_ShowSignedNum(1,7, LSpeed, 3);
        OLED_ShowSignedNum(1,12, RSpeed, 3);

        OLED_ShowNum(3,10, GetTick(), 4);
        OLED_ShowNum(4,8, LNum, 1);
        OLED_ShowNum(4,10, RNum, 1);
    }
}
