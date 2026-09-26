#include "stm32f10x.h"                  // Device header


void PWM_Init_SPWM(void)
{
	//打开时钟，定时器内部的所有时序逻辑都需要时钟驱动
	//芯片默认关闭外设时钟以降低功耗，需要时由软件按需开启。
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//配置输出PWM的GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	//设置PA3的模式为复用推挽输出。
	//因为PWM波形由定时器外设控制输出，GPIO需设置为复用功能模式。
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	//根据上述配置初始化GPIO。
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	TIM_InternalClockConfig(TIM2);//使用内部时钟，初始化时基单元
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//设置计数模式为向上计数，即从0计数到ARR值后溢出并重新从0开始。
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//设置自动重装载寄存器（ARR）的值为99。
	//即计数从0到99，共100个计数脉冲，决定PWM周期。
	TIM_TimeBaseInitStructure.TIM_Period = 100-1;      //ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler =36 -1;     //PSC的值 //20khz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	//根据上述配置初始化TIM2的时基单元。
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	
	
	
	
	//初始化输出比较oc单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCStructInit(&TIM_OCInitStructure);//赋一个默认初始值
	//设置输出比较模式为PWM1模式。
	//PWM1模式中，当CNT < CCR时输出有效电平，CNT ≥ CCR时输出无效电平（极性为正时）。
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//设置输出极性为高电平有效。即有效电平为高电平。
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//使能OC1输出，即让PWM波形从对应引脚输出。
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//设置比较值（CCR）初始为0，即占空比为0%。
	//后续通过PWM_SetCompare1函数动态修改。-------------------关联另一函数函数
	TIM_OCInitStructure.TIM_Pulse = 0;          //CCR的值
	
	
	//根据上述配置初始化TIM2的通道1（OC1）输出比较单元。
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);
	

	//使能TIM2计数器，定时器开始工作，立即产生PWM波形。
	TIM_Cmd(TIM2,ENABLE);
	
}
void PWM_Init_PWM(void)
{
    // 1. 开启TIM1和GPIOA时钟（TIM1在APB2，GPIOA也在APB2）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 如需重映射可开启AFIO，但此处使用默认引脚PA8，无需重映射

    // 2. 配置PA8为复用推挽输出（TIM1_CH1）
    GPIO_InitTypeDef GPIO_Initstructure;
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstructure);

    // 3. 时基单元配置（计数频率 = 72MHz / (71+1) = 1MHz → 1μs/步）
    TIM_InternalClockConfig(TIM1);                     // TIM1使用内部时钟
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;  
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;  // PSC = 71
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 高级定时器专用，这里不用
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    // 4. 输出比较单元配置（PWM1模式，高电平有效）
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);            // 先赋默认值
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 10000;             // CCR = 10000 → 占空比50%
    // 注意：高级定时器还有TIM_OCIdleState等参数，用默认即可
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);           // 初始化通道1

    // 5. 【关键】使能TIM1的主输出（MOE），高级定时器必须调用此函数
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    // 6. 启动TIM1计数器
    TIM_Cmd(TIM1, ENABLE);
}

//本函数用于调节占空比
//定义一个函数，用于设置TIM2通道1的比较值（即CCR寄存器值），参数Compare范围为0~ARR（此处为99），决定了PWM的占空比。
//占空比 = Compare / (ARR+1) = Compare / 100。
void PWM_SetCompare4(uint16_t Compare)
{
	//TIM_SetCompare1(TIM2, Compare); 是一个用于动态改变 PWM 占空比的标准库函数。
	TIM_SetCompare4(TIM2,Compare);
	//相当于改变TIM_OCInitStructure.TIM_Pulse = 0的值
}
