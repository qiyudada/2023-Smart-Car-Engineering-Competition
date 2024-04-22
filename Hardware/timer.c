#include "timer.h"
#include "sys.h"

/**************************************************************************
函数功能：定时中断初始化
入口参数：arr：自动重装值  psc：时钟预分频数 
返回  值：无
**************************************************************************/

int CountTimes=0;

void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//TIM_OCInitTypeDef TIM_OCInitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInitStruct.TIM_Period = arr;     //重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;  //预分频系数
	TIM_TimeBaseInitStruct.TIM_ClockDivision =0; //时钟分割
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_Trigger,ENABLE);  
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn;   
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;   
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;    
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM1,ENABLE);	 
}

void TIM_Count_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(TIM5_IRQn);
	
	TIM_Cmd(TIM5, ENABLE);
}

void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
	{
		CountTimes++;
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	}
}




