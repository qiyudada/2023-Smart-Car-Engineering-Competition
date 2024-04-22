#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

#define PI 3.141593f
#define L
#define LINE_ERROR  0.0095f

extern float countTime;
extern float speedFbk[4];
extern int colorchange;

int Read_Encoder(u8 TIMX);
void PID_init(void);
u8 moving(int x,int y,int r);
void stop(void);
u8 test(float x,int i);
float reVelocity(float* Wheel_Speed);
u8 adjust(int x,int y,int r,u8 t);
u8 operate(s8 p,s8 order);
#endif

