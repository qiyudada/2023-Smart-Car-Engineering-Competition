#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "Serial.h"
#include "string.h"

#define pin_1 GPIO_Pin_0
#define pin_2 GPIO_Pin_12
#define pin_3 GPIO_Pin_5

int rx,ry,gx,gy,bx,by;//物块或圆环位置

void openmv_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = pin_3|pin_2 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//	GPIO_SetBits(GPIOA, pin_1 | pin_2 | pin_3);
}



//////////////////////////////////////////指令
void scan_stop(void)
{
	GPIO_ResetBits(GPIOC, pin_1);
}

void scan_begin(void)
{
	GPIO_SetBits(GPIOC, pin_1);
}

void o_000(void)
{
	GPIO_ResetBits(GPIOA, pin_1);
	GPIO_ResetBits(GPIOA, pin_2);
	GPIO_ResetBits(GPIOA, pin_3);
}

void o_001(void)
{
	GPIO_ResetBits(GPIOA, pin_1);
	GPIO_ResetBits(GPIOA, pin_2);
	GPIO_SetBits(GPIOA, pin_3);
}
void o_010(void)
{
	GPIO_ResetBits(GPIOA, pin_1);
	GPIO_SetBits(GPIOA, pin_2);
	GPIO_ResetBits(GPIOA, pin_3);
}
void o_011(void)
{
	GPIO_ResetBits(GPIOA, pin_1);
	GPIO_SetBits(GPIOA, pin_2);
	GPIO_SetBits(GPIOA, pin_3);
}
void o_100(void)
{
	GPIO_SetBits(GPIOA, pin_1);
	GPIO_ResetBits(GPIOA, pin_2);
	GPIO_ResetBits(GPIOA, pin_3);
}
void o_101(void)
{
	GPIO_SetBits(GPIOA, pin_1);
	GPIO_ResetBits(GPIOA, pin_2);
	GPIO_SetBits(GPIOA, pin_3);
}
void o_110(void)
{
	GPIO_SetBits(GPIOA, pin_1);
	GPIO_SetBits(GPIOA, pin_2);
	GPIO_ResetBits(GPIOA, pin_3);
}
void o_111(void)
{
	GPIO_SetBits(GPIOA, pin_1);
	GPIO_SetBits(GPIOA, pin_2);
	GPIO_SetBits(GPIOA, pin_3);
}
//100扫码
//000物块
//001红圈
//010绿圈
//011蓝圈

////////////////////////////////////////////////////////////////
//二维码扫描函数+发送字节给串口屏
void scan(int *first,int *second)
{
	
	char sum_char1[7];
	char sum_char2[7];
	int sum_int1;
	int sum_int2;
	int lenth=0;
	
	//发送指令
	scan_begin();
	
	//接收、转换和发送
	if (Serial_RxFlag == 1)
	{
		lenth=strlen(Serial_RxPacket);
		if(lenth==7)
		{
//			OLED_ShowString(1, 1, "                ");
//			OLED_ShowString(3, 1, Serial_RxPacket);
			
			//提取字符
			sscanf(Serial_RxPacket, "%[^+]", sum_char1);
			sscanf(Serial_RxPacket, "%*4s %s",sum_char2 ); 
			
//			OLED_ShowString(4,1,sum_char1);
//			OLED_ShowString(4,5,sum_char2);
			
			//串口发送给显示屏
			printf("n1.val=%s\xff\xff\xff",sum_char1);//第一个
			printf("n0.val=%s\xff\xff\xff",sum_char2);//第二个
			//数据转换
			sscanf(sum_char1, "%d", &sum_int1);
			sscanf(sum_char2, "%d", &sum_int2);
			first[0]=sum_int1/100%10;
			first[1]=sum_int1/10%10;
			first[2]=sum_int1/1%10;
			second[0]=sum_int2/100%10;
			second[1]=sum_int2/10%10;
			second[2]=sum_int2/1%10;
		}
	Serial_RxFlag = 0;
	}
}

//////////////////////////////////////////////////////////////////
//物块、圆环的函数
void colour(int *rx,int *ry,int *gx,int *gy,int *bx,int *by,int b)
{
	int lenth;
	//给openmv发送指令，a为0找物块，a为1红2绿3蓝找圆环
	if (b==0)
		o_000();
	if (b==1)
		o_001();
	if (b==2)
		o_010();
	if (b==3)
		o_011();
	//串口接收和转换
	if (Serial2_RxFlag == 1)
	{
		lenth=strlen(Serial2_RxPacket);
		if(lenth>8)
		{
//			OLED_ShowString(3, 1, "                ");
//			OLED_ShowString(3, 1, Serial_RxPacket);
			sscanf(Serial2_RxPacket, "%d %d %d %d %d %d", &*rx,&*ry,&*gx,&*gy,&*bx,&*by);
		}
	Serial2_RxFlag = 0;
	}
}

void chose(int rx,int ry,int gx,int gy,int bx,int by,int *get_x,int *get_y)
{
	*get_x=0;
	*get_y=0;
	
	int middle_x=0;
	int middle_y=0;
	//红绿对比
	if(ry==0 && gy==0)
	{
		middle_x=0;
		middle_y=0;
	}
	else if(ry!=0 && gy==0)
	{
		middle_x=rx;
		middle_y=ry;
	}
	else if(ry==0 && gy!=0)
	{
		middle_x=gx;
		middle_y=gy;
	}
	else if(ry!=0 && gy!=0)
	{
		if(ry>gy)
		{
			middle_x=gx;
			middle_y=gy;
		}
		else if(ry<gy)
		{
			middle_x=rx;
			middle_y=ry;
		}
		
	}
	
	//和蓝色对比
	if(middle_y==0 && by==0)
	{
		*get_x=0;
		*get_y=0;
	}
	else if(middle_y!=0 && by==0)
	{
		*get_x=middle_x;
		*get_y=middle_y;
	}
	else if(middle_y==0 && by!=0)
	{
		*get_x=bx;
		*get_y=by;
	}
	else if(middle_y!=0 && by!=0)
	{
		if(middle_y>by)
		{
			*get_x=bx;
			*get_y=by;
		}
		else if(middle_y<by)
		{
			*get_x=middle_x;
			*get_y=middle_y;
		}
	}
}



