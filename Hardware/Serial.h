#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;
extern char Serial2_RxPacket[];
extern uint8_t Serial2_RxFlag;
extern char Serial3_RxPacket[];
extern uint8_t Serial3_RxFlag;

void Serial_Init(void);
void Serial_SendByte(USART_TypeDef* USARTx,uint8_t Byte);
void Serial_SendArray(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length);
void Serial_SendString(USART_TypeDef* USARTx,char *String);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendNumber(USART_TypeDef* USARTx,uint32_t Number, uint8_t Length);
void Serial_Printf(USART_TypeDef* USARTx,char *format, ...);

#endif
