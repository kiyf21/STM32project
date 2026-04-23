#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

// 定义LED引脚 (使用TIM2_CH1 - PA0)
#define LED_PIN GPIO_Pin_0
#define LED_PORT GPIOA
#define LED_TIM TIM2
#define LED_TIM_CHANNEL TIM_Channel_1

// 定义按键引脚
#define KEY_PORT GPIOC
#define KEY_PIN GPIO_Pin_13

// 全局变量
volatile uint8_t breathing_enabled = 1;  // 呼吸灯使能标志
volatile uint16_t pwm_duty = 0;          // PWM占空比
volatile int8_t breath_direction = 1;    // 呼吸方向：1增加，-1减少

// 函数声明
void GPIO_Configuration(void);
void TIM_PWM_Configuration(void);
void NVIC_Configuration(void);
void Breathing_Update(void);
uint8_t KEY_Scan(void);

int main(void)
{
    SystemInit();
    
    GPIO_Configuration();
    TIM_PWM_Configuration();
    NVIC_Configuration();
    
    breathing_enabled = 1;
    pwm_duty = 0;
    breath_direction = 1;
    
    while(1)
    {
        if(KEY_Scan())
        {
            breathing_enabled = !breathing_enabled;
            
            if(!breathing_enabled)
            {
                // 停止时关闭LED
                TIM_SetCompare1(LED_TIM, 0);
            }
            
            while(KEY_Scan());
            for(volatile uint32_t i = 0; i < 0xFFFF; i++);
        }
    }
}

// GPIO配置
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 开启GPIOA和GPIOC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    
    // 配置LED引脚为复用推挽输出 (PWM输出)
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    
    // 配置按键引脚为上拉输入
    GPIO_InitStructure.GPIO_Pin = KEY_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

// PWM定时器配置
void TIM_PWM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 开启TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // 定时器基础配置
    // 系统时钟72MHz，预分频72-1，得到1MHz的计数频率
    // 自动重装载值1000-1，PWM频率 = 1MHz / 1000 = 1kHz
    TIM_TimeBaseStructure.TIM_Period = 999;          // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 71;        // 预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(LED_TIM, &TIM_TimeBaseStructure);
    
    // PWM模式配置
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;  // 初始占空比为0
    
    TIM_OC1Init(LED_TIM, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(LED_TIM, TIM_OCPreload_Enable);
    
    // 使能定时器
    TIM_Cmd(LED_TIM, ENABLE);
    
    // 配置TIM3用于呼吸效果控制 (10ms中断)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Period = 999;          // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 719;       // 10kHz / 1000 = 10Hz (100ms)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

// NVIC配置
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    // 配置TIM3中断
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// TIM3中断服务函数 - 控制呼吸效果
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        
        if(breathing_enabled)
        {
            Breathing_Update();
        }
    }
}

// 呼吸效果更新
void Breathing_Update(void)
{
    // 更新PWM占空比
    pwm_duty += breath_direction * 5;  // 每次变化5个单位
    
    // 检查边界
    if(pwm_duty >= 1000)
    {
        pwm_duty = 1000;
        breath_direction = -1;  // 改为减少
    }
    else if(pwm_duty <= 0)
    {
        pwm_duty = 0;
        breath_direction = 1;   // 改为增加
    }
    
    // 更新PWM输出
    TIM_SetCompare1(LED_TIM, pwm_duty);
}

// 按键扫描函数
uint8_t KEY_Scan(void)
{
    if(GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == 0)
    {
        for(volatile uint32_t i = 0; i < 0x2FFF; i++);
        if(GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == 0)
        {
            return 1;
        }
    }
    return 0;
}
