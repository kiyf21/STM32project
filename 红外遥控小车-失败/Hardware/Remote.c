#include "stm32f10x.h"                  // Device header
#include "Exit_Key.h"
#include "Timer.h"


// 私有变量
uint8_t  s_RemoteKey = 0;      // 最新有效按键值（读后自动清零）
uint8_t  s_RepeatFlag = 0;     // 重复码标志（可选）

// NEC 解码状态机//结构体
typedef enum {
    STATE_IDLE = 0,     // 空闲，等待引导码
    STATE_GUIDE,        // 检测到引导码开始，等待引导码结束
    STATE_DATA,         // 接收数据位
    STATE_REPEAT        // 重复码
} NecState;

NecState s_State = STATE_IDLE;
uint32_t s_Data = 0;             // 暂存接收到的32位数据
uint8_t  s_BitCount = 0;         // 已接收位数
uint32_t s_LastTime = 0;         // 上一次中断时的计数器值


// 外部中断0处理函数（直接在此实现，避免修改 stm32f10x_it.c）
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) == RESET) return;

    uint32_t now = Timer_GetCounter();          // 当前时间 (µs)
    uint32_t delta = now - s_LastTime;   // 两次下降沿的时间间隔
    s_LastTime = now;

    // 状态机处理
    switch (s_State)
    {
        case STATE_IDLE:
            // 检测引导码：第一个下降沿到来，进入引导码等待状态
            // 引导码由9ms低电平 + 4.5ms高电平组成，我们需要等待第二个下降沿来判断
            // 此处先记录时间，下一个下降沿到来时判断是否在13.5ms左右
            s_State = STATE_GUIDE;
            break;

        case STATE_GUIDE:
            // 第二个下降沿，判断是否引导码结束
            if (delta > 13000 && delta < 14000)  // 13.5ms 左右 (9ms+4.5ms)
            {
                // 引导码正确，准备接收数据
                s_State = STATE_DATA;
                s_BitCount = 0;
                s_Data = 0;
            }
            else if (delta > 11000 && delta < 12000) // 11.25ms 左右 (9ms+2.25ms) 重复码
            {
                // 重复码，可以标记长按，这里简单处理：忽略重复码
                s_State = STATE_IDLE;
            }
            else
            {
                // 不是有效引导码，回到空闲
                s_State = STATE_IDLE;
            }
            break;

        case STATE_DATA:
            // 数据位：每个下降沿之间的时间决定 bit 值
            if (s_BitCount < 32)
            {
                if (delta > 1000 && delta < 1300)      // 1.12ms -> 逻辑0
                {
                    s_Data <<= 1;          // 左移，低位自动补0
                    s_BitCount++;
                }
                else if (delta > 2000 && delta < 2500) // 2.24ms -> 逻辑1
                {
                    s_Data = (s_Data << 1) | 1;
                    s_BitCount++;
                }
                else
                {
                    // 时间异常，重新开始
                    s_State = STATE_IDLE;
                }
            }

            if (s_BitCount == 32)
            {
                // 32位接收完成，校验地址码和命令码
				//分别提取出地址码，地址反码，命令码，命令反码
                uint8_t addr    = (s_Data >> 24) & 0xFF;
                uint8_t addr_inv= (s_Data >> 16) & 0xFF;
                uint8_t cmd     = (s_Data >> 8)  & 0xFF;
                uint8_t cmd_inv = s_Data & 0xFF;
				//“~addr_inv”是对“addr_inv”进行按位取反。然后对地址码与地址反码的按位取反进行比较
				//只有addr与cmd都符合才输出cmd即命令码来实现按键含义
                if (addr == (uint8_t)~addr_inv && cmd == (uint8_t)~cmd_inv)
                {
                    s_RemoteKey = cmd;   // 保存有效按键值
                }
                s_State = STATE_IDLE;    // 回到空闲，等待下一帧
            }
            break;

        default:
            s_State = STATE_IDLE;
            break;
    }
	//清除中断
    EXTI_ClearITPendingBit(EXTI_Line0);
}

// 初始化红外接收
void Remote_Init(void)
{
    Timer_Init();      // 必须先初始化定时器，因为中断中会调用 GetMicros()
    Exti_Key_Init();
}

// 获取最新按键值（读后自动清零）
uint8_t Remote_GetKey(void)
{
    uint8_t key = s_RemoteKey;
    s_RemoteKey = 0;
    return key;
}

