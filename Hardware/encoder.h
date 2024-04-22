#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"

#define ENCODER_TIM_PERIOD (u16)(65536) //不可大于65535 因为F103的定时器是16位的
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(u8 TIMX);
void Encoder_Init(void);

#endif

