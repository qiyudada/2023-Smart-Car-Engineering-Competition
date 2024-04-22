#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

#define PI 3.141593f
#define L
#define LINE_ERROR  0.006f

int Read_Encoder(u8 TIMX);
void PID_init(void);
u8 moving(int x,int y,int r);
void stop(void);
u8 test(float x,int i);
#endif

