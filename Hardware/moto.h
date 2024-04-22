#ifndef __MOTO_H
#define __MOTO_H
#include "sys.h"

//#define PWMA   TIM1->CCR1
#define PWMLF   TIM4->CCR4
#define PWMLB   TIM4->CCR3
#define PWMRB   TIM4->CCR2
#define PWMRF   TIM4->CCR1

//#define AIN2   PBout(15)
//#define AIN1   PBout(14)
//#define BIN1   PBout(13)
//#define BIN2   PBout(12)

#define LF2   PBout(12)
#define LF1   PBout(13)
#define LB1   PCout(14)
#define LB2   PCout(15)

#define RF2   PCout(4)
#define RF1   PCout(5)
#define RB1   PBout(4)
#define RB2   PBout(5)

void MOTO_Init(void);    //初始化控制电机所需的引脚
void pwm_Init(u16 arr,u16 psc); //PWM输出初始化0OL

#endif

