#include "moto.h"

void MOTO_Init(void)//初始化控制电机所需的IO
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//PORTB12 13 14 15推挽输出
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_Init(GPIOC,&GPIO_InitStruct);
}

void pwm_Init(u16 arr,u16 psc) //初始化pwm输出引脚
{	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  //使能定时器1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE); //使能GPIOA的时钟

//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;         //复用输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_6|GPIO_Pin_7;   //PA8 PA11
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Period = arr;                     //设定计数器自动重装值 
	TIM_TimeBaseInitStruct.TIM_Prescaler  = psc;                 //设定预分频器
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;                //设置时钟分割
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);              //初始化定时器
	
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;              //选择PWM2模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;  //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                             //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;       //设置输出极性
	
	TIM_OC1Init(TIM4,&TIM_OCInitStruct);                         //初始化输出比较参数
	TIM_OC2Init(TIM4,&TIM_OCInitStruct);                         //初始化输出比较参数
	TIM_OC3Init(TIM4,&TIM_OCInitStruct);                         //初始化输出比较参数
	TIM_OC4Init(TIM4,&TIM_OCInitStruct);                         //初始化输出比较参数

	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable); //CH1使能预装载寄存器
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable); //CH1使能预装载寄存器
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable); //CH1使能预装载寄存器
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); //CH1使能预装载寄存器
 
//	TIM_CtrlPWMOutputs(TIM4,ENABLE);                 //高级定时器输出必须设置这句
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);              //使能TIM1在ARR上的预装载寄存器
	
	TIM_Cmd(TIM4,ENABLE);                            //使能定时器1
}

