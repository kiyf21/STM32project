#ifndef __REMOTE_H
#define __REMOTE_H

void EXTI0_Init(void);
void EXTI0_IRQHandler(void);

extern volatile int16_t sign;
extern volatile uint16_t cnt;

#endif
