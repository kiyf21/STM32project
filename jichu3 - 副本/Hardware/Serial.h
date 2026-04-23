#ifndef __Serial_H
#define __Serial_H


#include <stdio.h>

void Serial_Init(void);
void USART_SendBytes(USART_TypeDef* USARTx, uint8_t *data, uint16_t len);
//void Serial_SendArray(uint8_t *Array,uint16_t Length);
//void Serial_SendString(char*String);
//void Serial_SendNumber(uint32_t Number,uint8_t Length);
uint8_t Serial_GetRxFlag(void);
void USART_SendString(USART_TypeDef* USARTx, char *str);
void USART_SendBytes(USART_TypeDef* USARTx, uint8_t *data, uint16_t len);
void USART1_IRQHandler(void);
void Serial_SendPacket(void);

void ProcessCommand(uint8_t *data, uint16_t len)；


#endif
