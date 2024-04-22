#ifndef __SSERIAL_H
#define __SSERIAL_H

#include <stdio.h>



void Serial_Init(void);

//串口1
void Serial2_SendByte(uint8_t Byte);
void Serial2_SendArray(uint8_t *Array, uint16_t Length);
void Serial2_SendString(char *String);
void Serial2_SendNumber(uint32_t Number, uint8_t Length);


//串口2
//void Serial_SendByte2(uint8_t Byte);
//void Serial_SendString2(char *String);
//void Serial_Printf(char *format, ...);

#endif

