#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void TIM5_Int_Init(u16 arr,u16 psc);
void TIM_Count_Init(void);
void TIM5_IRQHandler(void);

#endif 

