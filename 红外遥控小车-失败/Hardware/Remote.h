#ifndef __REMOTE_H
#define __REMOTE_H

void Remote_Init(void);          // 初始化红外接收（PB0 + TIM4）
uint8_t Remote_GetKey(void);     // 获取最新按键码，返回0表示无按键

#endif
