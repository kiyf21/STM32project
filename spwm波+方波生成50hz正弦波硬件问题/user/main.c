#include "stm32f10x.h"  
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "TIMER.h"

//// 128点正弦表，用于 SPWM，范围 0~99
//const uint16_t sin_table[128] = {
//    49, 51, 54, 56, 59, 61, 63, 66, 68, 70, 72, 74, 76, 78, 80, 82,
//    83, 85, 86, 88, 89, 90, 91, 92, 93, 94, 95, 95, 96, 96, 96, 96,
//    96, 96, 96, 96, 95, 95, 94, 93, 92, 91, 90, 89, 88, 86, 85, 83,
//    82, 80, 78, 76, 74, 72, 70, 68, 66, 63, 61, 59, 56, 54, 51, 49,
//    47, 44, 42, 39, 37, 35, 33, 30, 28, 26, 24, 22, 20, 18, 16, 14,
//    13, 11, 10, 8, 7, 6, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0,
//    0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 13,
//    14, 16, 18, 20, 22, 24, 26, 28, 30, 33, 35, 37, 39, 42, 44, 47
//};
const uint16_t sin_table1[64] = {
	1,  3,  5,  6,  7,  8,  10, 12, 15, 18, 21, 25, 28, 32, 36, 41, 45, 50,
	55, 60, 64, 69, 73, 77, 81, 85, 88, 91, 94, 96, 98, 99,
	99, 98, 96, 94, 91, 88, 85, 81, 77, 73, 69, 64, 60, 55,
	50, 45, 41, 36, 32, 28, 25, 21, 18, 15, 12, 10, 8,  7,  6,  5,  3,  1
};
// 这组数据是用公式 round(49 + 49*sin(2π*i/128)) 生成的，已确保最大99最小0
uint8_t head1 = 0;
uint8_t head2 = 0;



int main(void)
{
	OLED_Init();
	Timer_Init();
	PWM_Init();
	PWM_Init_TIM1_50Hz();	
	while(1)
	{
		PWM_SetCompare4(sin_table1[head1]);   // 更新CCR
		//PWM_SetCompare1(head2);
	}

}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        
        head1++;
        if (head1 >= 64) head1 = 0;          // 循环
		head2=100-head1;
    }
}

