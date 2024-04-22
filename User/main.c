/*********************************Header Files*********************************/
#include "stm32f10x.h"                  
#include "Delay.h"
#include "Serial.h"
#include "string.h"
#include "openmv.h"
#include <stdio.h>
#include "servo.h"
#include "step.h"
#include "Key.h"
#include "sys.h"
#include "control.h"
#include "timer.h"
#include "OLED.h"
#include "BlueTooth.h"

/*********************************Variable Declaration*********************************/
int angle_left=-133,angle_right=127;
extern int first[3];//指令顺序1
extern int second[3];//指令顺序2
int count=0;
//int rx,ry,gx,gy,bx,by;//物块或圆环位置
int get_x,get_y;
u16 up1=80,down1=80,up2=60,down2=60;
int time=0;
int ColorArray[3];
float X=1,p=0,Y=0,n=0;
float X_set=175,Y_set=130,move_x,move_y;
int back=-70;
extern int CTIMEO;
extern int CTIMET;
extern int CTIMES;
extern int CTIMEF;
extern int Depart;
extern int MA;//出发区到道路
extern int MB;//道路上到扫码区
extern int MC;//扫码区到原料区
extern int MD;//原料区到角落
extern int ME;//角落到红色圆环
extern int MF;//粗加工区到暂存区

/*********************************OPERATION*********************************/
int main(void)
{	
	Serial_Init();
	openmv_Init();  
	Key_Init();
	servo_Init();	
//	Stm32_Clock_Init(9);      //系统时钟设置            
	LED_Init();               //=====初始化与 LED 连接的硬件接口
	pwm_Init(7199,0);         //初始化pwm输出
	MOTO_Init();              //初始化控制电机所需的IO
	Encoder_Init();
	Encoder_Init_TIM3();
//	Encoder_Init_TIM4();
	TIM5_Int_Init(99,7199);   //10ms一次中断
	Encoder_Init_TIM2();      //初始化计数器（定时器）
	PID_init();
	OLED_Init();
	TIM_Count_Init();

/*********************************Start*********************************/
/*********************************BlueTooth Debug*********************************/	
//	BlueDebug();
/*********************************Depart*********************************/
	while (1)
	{
		if(1)
		{
			colour(&rx,&ry,&gx,&gy,&bx,&by,0);//颜色识别准备
			Delay_ms(CTIMEO);
			Servo3_SetAngle(150); //爪子合拢
			Delay_ms(CTIMEO);
			Servo1_SetAngle(120);  //转盘
			keystart();//一键启动
			Delay_ms(CTIMEO);
		
/*********************************Scan QR Code*********************************/
			moving(MA,0,0);//左侧移
			moving(0,MB,0); //前进
			stop();
			Mission();//扫码
			
/*********************************Arrival Masterial Area*********************************/
            Servo1_SetAngle(260); //云台回正
			Delay_ms(CTIMEO);
			Servo3_SetAngle(100); //爪子张开
			moving(0,MC,0);// 继续前进到原料区
			stop();
            //adjust(12,0,0,6);  //右侧移
    		stop();
 			Delay_ms(CTIMES);
			move_or_not();
			aim_blobs();//微调瞄准
			OperationRunning(first);//识别+抓取  三个物块
/*********************************Arrival Rough Red Area*********************************/
	        adjust(-14,0,0,6);  //左侧移 缓慢
			moving(0,MD,0);    //前进到角落
			stop();
			Delay_ms(CTIMES);
			moving(0,0,angle_left);  //逆时针转弯90°
			moving(0,ME,0);  //停到红色圆环位置
			stop();
			colorchange=first[0]; //靶环识别
/*********************************Place Thrice Object*********************************/			
			moving(0,(first[0]-1)*150,0);  //停到第1个圆环位置
			stop();
			for(int i=0;i<3;i++)
			{
			//			ToRightSite(first,first[i]);
			   if(i>=1) 
				   moving(0,(first[i]-first[i-1])*150,0);//基准后找靶环位置，中调整精准到环
				stop();
				aim_circle(first[i]);
				Delay_ms(CTIMET);
				arm_moving(2,i);
				moving(back,0,0);
				stop();
			}		

/*********************************Catch Object To Vehicle*********************************/	
			moving(0,(first[0]-first[2])*150,0);  //停到第1个要抓取的圆环位置，位置上有物件
			stop();

			for(int i=0;i<3;i++)
			{
				if(i>=1) moving(0,(first[i]-first[i-1])*150,0);//调整精准到环
				stop();
				aim_circle(first[i]);
				Delay_ms(CTIMEO);
				arm_moving(3,i);
				moving(back,0,0);
				stop();
			}		
/*********************************Arrival TS Red Area*********************************/
			moving(0,480+150*(3-first[2]),0);
			moving(0,0,angle_left);
			moving(0,MF,0);//停到红色圆环位置
			//colorchange=first[0]; //靶环识别
/*********************************Place Object TS Area*********************************/		
			moving(0,(first[0]-1)*150,0);  //停到第1个圆环位置
			stop();
			for(int i=0;i<3;i++)
			{
				//ToRightSite(first,first[i]);
			   if(i>=1) 
			   {
				   moving(0,(first[i]-first[i-1])*150,0);
			   }//基准后找靶环位置，中调整精准到环
					stop();
					aim_circle(first[i]);
					Delay_ms(CTIMEO);
					arm_moving(2,i);
					moving(back,0,0);
					stop();
			}	
/*********************************Back to Masterial Area*********************************/
			moving(0,-700-150*(1-first[2]),0); //退回拐角处
			moving(0,0,angle_right);  //顺时针转弯90°
			moving(0,-1580,0);//退回另一个拐角处
			stop();
			Delay_ms(CTIMET);
			moving(0,0,angle_right);  //顺时针转弯90°
			moving(0,-380,0); //退回原料区
			stop();
/*********************************Ready to Second Circulation*********************************/
/*********************************Use Second QR Code*********************************/
			for(int j=0;j<3;j++)
			{
				first[j]=second[j];
			}
            moving(50,0,0);  //右侧移
    		stop();
 			Delay_ms(CTIMES);
			//Servo3_SetAngle(150);
			move_or_not();
			aim_blobs();//微调瞄准
			OperationRunning(first);//第二轮  识别+抓取
/*********************************Arrival Rough Red Area*********************************/
	        adjust(-12,0,0,6);  //左侧移 缓慢
			moving(0,380,0);    //前进到角落
			moving(0,0,angle_left);  //逆时针转弯90°
			moving(0,700,0);  //停到红色圆环位置
			colorchange=first[0]; //靶环识别
/*********************************Place Thrice Object*********************************/			
			moving(0,(first[0]-1)*150,0);  //停到第1个圆环位置
			stop();
			for(int i=0;i<3;i++)
			{
			//ToRightSite(first,first[i]);
			   if(i>=1)
			   {
				   moving(0,(first[i]-first[i-1])*150,0);
			   }//基准后找靶环位置，中调整精准到环
				stop();
				aim_circle(first[i]);
				Delay_ms(CTIMEO);
				arm_moving(2,i);
				moving(back,0,0);
				stop();
			}		
/*********************************Catch Object To Vehicle*********************************/		
			moving(0,(first[0]-first[2])*150,0);  //停到第1个要抓取的圆环位置，位置上有物件
			stop();
			for(int i=0;i<3;i++)
			{
			//ToRightSite(first,first[i]);
			   if(i>=1) 
			   {
				   moving(0,(first[i]-first[i-1])*150,0);
			   }//调整精准到环
				stop();
				aim_circle(first[i]);
				Delay_ms(CTIMEO);
				arm_moving(3,i);
				moving(back,0,0);
				stop();
			}		

/*********************************Arrival TS Red Area*********************************/			
			moving(0,480+150*(3-first[2]),0);
			moving(0,0,angle_left);
			moving(0,700,0);//停到红色圆环位置

			//colorchange=first[0]; //靶环识别 （有物件在上面）
/*********************************Place Object TS Area 2*********************************/			
			moving(0,(first[0]-1)*150,0);  //停到第1个圆环位置
			stop();
			for(int i=0;i<3;i++)
			{
			//ToRightSite(first,first[i]);
			   if(i>=1) 
			   {
				   moving(0,(first[i]-first[i-1])*150,0);
			   }//基准后找靶环位置，中调整精准到环
				stop();
				aim_circle(first[i]);
				Delay_ms(CTIMEO);
				arm_moving(4,i);
				moving(back,0,0);
				stop();
			}	
/*********************************Return Home*********************************/
			moving(0,700+150*(3-first[2]),0); //前进到拐角处
			moving(0,0,angle_left);  //逆时针转弯90°
			moving(0,1600,0);//前进回家
			Depart--;
			stop();
		} //if
	} //while
} //main
/*********************************End*********************************/





