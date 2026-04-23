#include "stm32f10x.h"  
#include "Delay.h"
#include "OLED.h"
#include "Remote.h"
#include "Timer.h"
extern uint32_t g_lastTime;
extern uint32_t g_delta;
extern uint32_t guide_count;


int main(void)
{
	 // 1. 设置中断优先级分组（必须在所有NVIC初始化之前）
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//Remote_Init();
	OLED_Init();
	Timer_Init();
	

	
	
	while(1)
	{
		// 显示引导码计数
		//OLED_ShowNum(4, 1, guide_count, 5);   // 假设 OLED 有第5行
		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
		OLED_ShowNum(3, 4, GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0), 1);

		
		OLED_ShowNum(1, 1, cnt, 5);   // 显示 cnt（当前测量值）
        OLED_ShowNum(2, 1, sign, 5);  // 显示 sign（-1 或计数值）

		
		
		//OLED_ShowNum(2,1,Timer_GetMicros(),5);
		 // 显示时间间隔 delta (单位 µs)
        //OLED_ShowNum(3, 1, g_delta, 5);      // 第3行，占5位
        // 显示当前计数值 now
      //  OLED_ShowNum(4, 1, g_lastTime, 5);   // 第4行

	}

}
