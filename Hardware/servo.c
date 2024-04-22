#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "sys.h"

s8 h;
void servo_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM8);
//	TIM_InternalClockConfig(TIM1);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 30000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);
//	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
//	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);
//	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
//	TIM_Cmd(TIM1, ENABLE);
//	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

//void PWM8_SetCompare1(uint16_t Compare)
//{
//	TIM_SetCompare1(TIM8, Compare);
//}

void PWM8_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM8, Compare);
}

void PWM8_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM8, Compare);
}

void PWM8_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM8, Compare);
}
///////////////////////////////////////////////////////
void Servo1_SetAngle(float Angle)
{
	PWM8_SetCompare2(Angle / 270 * 2000 + 500);
//	PAout(11)=1;
//	Delay_ms(19);
//	PAout(11)=0;
//	Delay_ms(Angle);
	
}

extern s8 flagBreak;
void Servo2_Set(int speed,int time)
{
	int i;

		PWM8_SetCompare3(speed+1500);
	for(i=0;i<time;i++)
	{
		Delay_ms(10);
	}
	time=(speed>0)?time:-time;
	h+=time;
	PWM8_SetCompare3(0+1500);
}

void Servo3_SetAngle(float Angle)
{
	PWM8_SetCompare4(Angle / 270 * 2000 + 500);
}

//void stepper(float Compare)
//{
//	TIM_SetCompare1(TIM8, Compare);
//}
