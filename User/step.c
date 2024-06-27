#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Serial.h"
#include "string.h"
#include "openmv.h"
#include <stdio.h>
#include "servo.h"
#include "LED.h"
#include "Key.h"
#include "sys.h"
#include "control.h"
#include "timer.h"
#include "step.h"
#include "OLED.h"
#include <stdlib.h>

int key_start=0;
int KeyNum=0;

void aim_blobs()//based on blobs to adjust location
{
	colorchange=0;
	static float ak=2,lastRx,lastRy;
	float move_x,move_y;
	float X_set=89,Y_set=61;
	int mx,my;
//	int speed_1=15,time_1=3,Delay_1=200;
	int speed_2=10,time_2=4,Delay_2=200;

	while(1)
	{

		if(mx-lastRx>ak || lastRx-mx>ak)
		{
			mx=lastRx;
			lastRx=mx;
		}
		if(my-lastRy>ak || lastRy-my>ak)
		{
			my=lastRy;
			lastRy=my;
		}
		if(rx!=0){mx=rx;my=ry;}
		if(gx!=0){mx=gx;my=gy;}
		if(bx!=0){mx=bx;my=by;}
			
		if(mx!=0)
		{
			move_x=X_set-mx;
			move_y=Y_set-my;
			
//			if(move_y<-30)
//			{
//				if(move_x>10) {adjust(0,speed_2,0,  time_2); Delay_ms(Delay_2);}//x取负
//				else if(move_x<-10) {adjust(0,-speed_2,0,time_2);  Delay_ms(Delay_2);}
//				else if(move_y>10) {adjust(-speed_1,0,0, time_1); Delay_ms(Delay_1);}
//				else if(move_y<-10) {adjust(speed_1,0,0, time_1);  Delay_ms(Delay_1);}
//			}
//			
//			else
//			{
				if(move_x>12) 
				{
					adjust(0,speed_2,0,time_2); 
					Delay_ms(Delay_2);
				}//x取负
				else if(move_x<-12) 
				{
					adjust(0,-speed_2,0,time_2); 
					Delay_ms(Delay_2);
				}
				else if(move_y>12) 
				{
					adjust(-speed_2,0,0,time_2);
					Delay_ms(Delay_2);
				}
				else if(move_y<-12)
				{
					adjust(speed_2,0,0, time_2);
					Delay_ms(Delay_2);
				}
				else	
					break;
//			}
		}
		else stop();
		
	}
	
}

void aim_circle(int color)
{
	colorchange=color;
	static float ak=2,lastRx,lastRy;
	float move_x,move_y;
	float X_set=180,Y_set=113;
	int mx,my;
//	int speed_1=17,time_1=5,Delay_1=100;
	int speed_2=10,time_2=3,Delay_2=100;

	while(1)
	{
		if((mx-lastRx)>ak || (lastRx-mx)>ak)
			
			mx=lastRx;
			lastRx=mx;

		if((my-lastRy)>ak || (lastRy-my)>ak)
			my=lastRy;
			lastRy=my;
		
			if(color==1){mx=rx;my=ry;}
			if(color==2){mx=gx;my=gy;}
			if(color==3){mx=bx;my=by;}
			
		if(mx!=0)
		{
			move_x=X_set-mx;
			move_y=Y_set-my;
			
//			if(move_y<-30)
//			{
//				if(move_x>10) {adjust(0,speed_2,0,  time_2); Delay_ms(Delay_2);}//x取负
//				else if(move_x<-10) {adjust(0,-speed_2,0,time_2);  Delay_ms(Delay_2);}
//				else if(move_y>10) {adjust(-speed_1,0,0, time_1); Delay_ms(Delay_1);}
//				else if(move_y<-10) {adjust(speed_1,0,0, time_1);  Delay_ms(Delay_1);}
//			}
//			
//			else
//			{
				if(move_x>12) {adjust(0,speed_2,0,  time_2); Delay_ms(Delay_2);}//x取负
				else if(move_x<-12) {adjust(0,-speed_2,0,time_2);  Delay_ms(Delay_2);}
				else if(move_y>12) {adjust(-speed_2,0,0, time_2); Delay_ms(Delay_2);}
				else if(move_y<-12) {adjust(speed_2,0,0, time_2);  Delay_ms(Delay_2);}
				else	break;
//			}
		}
		else stop();
		
	}
}

void move_or_not()
{
	colorchange=0;
	int mx_1=0,my_1=0,mx_2=0,my_2;


	while(1)
	{
		if(rx!=0){mx_1=rx;my_1=ry;}
		else if(gx!=0){mx_1=gx;my_1=gy;}
//		else {mx_1=bx;my_1=by;}
		
		
		if(mx_1!=0 && mx_2!=0 &&  mx_1-mx_2<5&&mx_1-mx_2>-5  &&  my_1-my_2<5&&my_1-my_2>-5)
		{
			break;
		}
		else stop();
		Delay_ms(500);
		mx_2=mx_1,my_2=my_1;
	}
}


void move_or_not_2()
{
	colorchange=0;
	int mx_1,my_1,mx_2,my_2;
	int flag_1,flag_2;

	while(1)
	{
		int limit=10;
		mx_1=0,my_1=0;
		if(rx!=0){mx_1=rx;my_1=ry;}
		else if(gx!=0){mx_1=gx;my_1=gy;}
		else {mx_1=bx;my_1=by;}
		
		if(mx_1!=0 &&  (mx_1-mx_2)<limit && (mx_1-mx_2)>-limit  &&  (my_1-my_2)<limit && (my_1-my_2)>-limit)
		{
			flag_1=1;//静止置1
		}
		else if(mx_1!=0 &&  (mx_1-mx_2)>=limit && (mx_1-mx_2)<=-limit  &&  (my_1-my_2)>=limit && (my_1-my_2)<=-limit)
		{flag_1=0;}//动置0
		
		if (flag_2==0&&flag_1==1)
		{
			break;
		}
		
		Delay_ms(400);
		mx_2=mx_1,my_2=my_1;
		flag_2=flag_1;
		
	}
}

void keystart()//start control key
{
		while(1)
		{
			KeyNum = Key_GetNum();
			if (KeyNum == 1)
			{
				LED1_Turn();
				break;
			}
		}
}


void Mission()
{	
	while(1)
	{
		scan(first,second);
		if (first[0]!=0)
		{
			scan_stop();
			break;
		}
	}
		
}


void arm_moving(int order,int ci)
{
	switch(order)
	{
		case 1:  //原料台→车上	
			Servo2_Set(800,120);//传动带 空载下降
			Delay_ms(300);
			Servo3_SetAngle(155);//爪子抓取	
			Delay_ms(400);
			Servo2_Set(-800,155);//传动带 爪子拿物块上升  放到车上
			//Servo2_Set(-800,160);//传动带 爪子上升  放到车上
			Delay_ms(400);
			Servo1_SetAngle(ci*45+30);//轮盘	
			Delay_ms(600);
			Servo2_Set(800,174);//传动带，爪子拿物块放车上
			Delay_ms(300);
			Servo3_SetAngle(138);//爪子张开-中	
			Delay_ms(300);
			Servo2_Set(-800,190);//传动带，爪子空载上升
			Delay_ms(300);
			Servo1_SetAngle(260);//轮盘回正
			Servo3_SetAngle(70);//爪子
			break;
		
		case 2:  //车上→地上
//			Servo1_SetAngle(260);//轮盘
//			Servo3_SetAngle(225);//爪子	
			if(ci==0||ci==2)
			{
				Servo1_SetAngle(ci*45+27);
			}//轮盘
			else if(ci==1)
			{
				Servo1_SetAngle(72);
			}//轮盘
			Delay_ms(400);
			Servo3_SetAngle(140);//爪子张开-中	
			Delay_ms(400);
			Servo2_Set(800,170);//传动带
			Delay_ms(400);
			Servo3_SetAngle(152);//爪子	
			Delay_ms(400);
    		if(ci==0)
			{
				Servo2_Set(-800,210);
			}
			if(ci==1||ci==2)
			{				
				Servo2_Set(-800,220);
			}//传动带爪子带物块上升
			Delay_ms(400);
			Servo1_SetAngle(260);//轮盘
			Delay_ms(400);
			Servo2_Set(800,255);//传动带带着物块下降
			Delay_ms(800);
			Servo3_SetAngle(140);//爪子	把放下物块到地板
			Delay_ms(500);
			if(ci==0||ci==1)
			{
				Servo2_Set(-800,280);//传动带
			}
			if(ci==2)
			{
				Servo2_Set(-800,340);//传动带
			}
			Delay_ms(500);
			Servo3_SetAngle(70);//爪子
			break;
			
		case 3:  //地上→车上 第三次
			if(ci==0)
			{
				Servo2_Set(800,315);//传动带 空载爪子下降
			}
			if(ci==1||ci==2)
			{
				Servo2_Set(800,240);
			}
			Delay_ms(500);
			Servo3_SetAngle(150);//爪子闭合	
			Delay_ms(500);
    		Servo2_Set(-800,246);//传动带 爪子带着物块上升
			Delay_ms(500);
			if(ci==0)
			{
				Servo1_SetAngle(ci*45+25);
			}//轮盘	
			else if(ci==1)
			{
				Servo1_SetAngle(72);//轮盘
			}
			else if(ci==1)
			{
				Servo1_SetAngle(110);//轮盘
			}
			Delay_ms(800);
			Servo2_Set(800,95);//传动带，爪子带着物块下降到车上
			Delay_ms(400);
			Servo3_SetAngle(140);//爪子张开-中
			Delay_ms(300);

			if(ci==0||ci==1)
			{
				Servo2_Set(-800,160);//传动带爪子上升
			}
			if(ci==2)
			{
				Servo2_Set(-800,225);//传动带
			}
			Delay_ms(500);
			Servo1_SetAngle(260);//轮盘
			Servo3_SetAngle(70);//爪子
			break;
			
		case 4: //车上→
			//Servo1_SetAngle(260);//轮盘
			//Servo3_SetAngle(225);//爪子	
			if(ci==0||ci==2)
			{
				Servo1_SetAngle(ci*45+27);//轮盘
			}
			else if(ci==1)
			{
				Servo1_SetAngle(72);//轮盘
			}
			Delay_ms(300);
			Servo3_SetAngle(140);//爪子张开-中	
			Delay_ms(300);
			Servo2_Set(800,170);//传动带
			Delay_ms(500);
			Servo3_SetAngle(152);//爪子	
			Delay_ms(500);
    		if(ci==0)
			{
				Servo2_Set(-800,210);
			}
			if(ci==1||ci==2)	
			{
				Servo2_Set(-800,220);//传动带爪子带物块上升
			}
			Delay_ms(400);
			Servo1_SetAngle(260);//轮盘
			Delay_ms(400);
			Servo2_Set(800,128);//传动带带着物块下降
			Delay_ms(800);
			Servo3_SetAngle(140);//爪子	放下物块到地板
			Delay_ms(400);
			if(ci==0||ci==1)
			{
				Servo2_Set(-800,140);//传动带
			}
			if(ci==2)
			{
				Servo2_Set(-800,340);//传动带
			}
			Delay_ms(400);
			Servo3_SetAngle(70);//爪子
			break;
	}
}

uint8_t ViewColor()//确定颜色
{
	int ColorNum=0;
	if((rx!=0&&ry!=0)&&(gx==0&&gy==0)&&(bx==0&&by==0))
	{
		ColorNum=1;//红
	}
	else if((rx==0&&ry==0)&&(gx!=0&&gy!=0)&&(bx==0&&by==0))
	{
		ColorNum=2;//绿
	}
	else if((rx==0&&ry==0)&&(gx==0&&gy==0)&&(bx!=0&&by!=0))
	{
		ColorNum=3;//蓝
	}
	return ColorNum;
}

float T1;
float T0;
void OperationRunning(int *array)//识别、抓取
{	
    int x_min=59,x_max=119,y_min=31,y_max=91;
    int time=0;
    int ColorNum1,ColorNum2,ColorNum;
    int threshold = 20; // 初始阈值
	int avg=0;
    int last_avg = 0;// 上一次颜色的平均值
	extern int CountTimes;
	T0=CountTimes;
    while(1)
    {	
        while(1)
        { 
            colour(&rx,&ry,&gx,&gy,&bx,&by,0);
            if((rx<x_max && rx>x_min && ry>y_min && ry<y_max)||(gx<x_max && gx>x_min && gy>y_min && gy<y_max)||(bx<x_max && bx>x_min && by>y_min && by<y_max))
            {
                ColorNum1=ViewColor();
				 last_avg = (rx + ry + bx) / 3;// 计算当前颜色的平均值
            }
            Delay_ms(250);
            colour(&rx,&ry,&gx,&gy,&bx,&by,0);
            if((rx<x_max && rx>x_min && ry>y_min && ry<y_max)||(gx<x_max && gx>x_min && gy>y_min && gy<y_max)||(bx<x_max && bx>x_min && by>y_min && by<y_max))
            {
                ColorNum2=ViewColor();
				avg = (rx + ry + bx) / 3;
            }
			T1=CountTimes-T0;
            if(ColorNum1==ColorNum2)// 判断当前颜色是否符合 
            {
                ColorNum=ColorNum2;
			 
				if ((abs(avg - last_avg) < threshold)&&(ColorNum==array[time]))
				{
					arm_moving(1,time);
					time++;
				}
				break;
            }
			else if(T1 >= 16)
			{
				break;
			}
        }
        if(time==3)
        {
            break;
        }
        
        
    }
}



