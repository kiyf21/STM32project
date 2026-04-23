#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

// 定义LED和按键引脚
#define LED_PORT GPIOA
#define LED_PINS GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                 GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7

#define KEY_PORT GPIOC
#define KEY_PIN GPIO_Pin_13

// 全局变量
volatile uint8_t flow_enabled = 1;    // 流水灯使能标志
volatile uint8_t current_led = 0;     // 当前LED位置

// 函数声明
void GPIO_Configuration(void);
void TIM_Configuration(void);
void NVIC_Configuration(void);
void LED_Flow(void);
void LED_All_Off(void);
uint8_t KEY_Scan(void);

int main(void)
{
    // 系统时钟初始化
    SystemInit();
    
    // 外设初始化
    GPIO_Configuration();
    TIM_Configuration();
    NVIC_Configuration();
    
    // 初始状态：开启流水灯
    flow_enabled = 1;
    current_led = 0;
    
    while(1)
    {
        // 检测按键
        if(KEY_Scan())
        {
            // 切换流水灯状态
            flow_enabled = !flow_enabled;
            
            if(!flow_enabled)
            {
                // 停止时关闭所有LED
                LED_All_Off();
            }
            
            // 等待按键释放
            while(KEY_Scan());
            
            // 消抖延时
            for(volatile uint32_t i = 0; i < 0xFFFF; i++);
        }
    }
}

// GPIO配置
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 开启GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    
    // 配置LED引脚 (推挽输出)
    GPIO_InitStructure.GPIO_Pin = LED_PINS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    
    // 配置按键引脚 (上拉输入)
    GPIO_InitStructure.GPIO_Pin = KEY_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
    
    // 初始关闭所有LED
    LED_All_Off();
}

// 定时器配置 (用于流水灯时间控制)
void TIM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    // 开启TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // 定时器基础配置
    // 定时器频率 = 72MHz / (7199 + 1) = 10kHz
    // 中断时间 = (9999 + 1) / 10kHz = 1s
    TIM_TimeBaseStructure.TIM_Period = 9999;
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // 使能定时器更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

// NVIC配置
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 设置中断优先级分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    // 配置TIM2中断
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 定时器2中断服务函数
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        // 如果流水灯使能，则执行流水效果
        if(flow_enabled)
        {
            LED_Flow();
        }
    }
}

// LED流水效果
void LED_Flow(void)
{
    // 关闭所有LED
    LED_All_Off();
    
    // 点亮当前LED
    GPIO_SetBits(LED_PORT, (uint16_t)(1 << current_led));
    
    // 移动到下一个LED位置
    current_led = (current_led + 1) % 8;
}

// 关闭所有LED
void LED_All_Off(void)
{
    GPIO_ResetBits(LED_PORT, LED_PINS);
}

// 按键扫描函数
uint8_t KEY_Scan(void)
{
    // 按键按下为低电平
    if(GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == 0)
    {
        // 简单消抖
        for(volatile uint32_t i = 0; i < 0x2FFF; i++);
        
        if(GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == 0)
        {
            return 1;
        }
    }
    return 0;
}
