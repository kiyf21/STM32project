#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

/**
 * @brief 初始化 SysTick 定时器，产生 1ms 中断
 */
void SysTick_Init(void);

/**
 * @brief 获取系统运行的毫秒数
 * @return 当前毫秒计数值（从0开始，每1ms增加1）
 */
uint32_t GetTick(void);

/**
 * @brief 启动一个非阻塞计时器
 * @param startVar 指向存储开始时间的变量
 * @param ms       需要延时的毫秒数
 */
void Delay_NonBlocking_Start(uint32_t *startVar, uint32_t ms);

/**
 * @brief 检查非阻塞计时器是否到期
 * @param startVar 指向存储开始时间的变量
 * @return 1: 到期, 0: 未到期
 */
uint8_t Delay_NonBlocking_Check(uint32_t *startVar);

#endif
