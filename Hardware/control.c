#include "control.h"
#include "cal_support.h"
#include "servo.h"
#include "openmv.h"
#include "sys.h"

s8 flagStop,flagMove,flagBreak=0;     //运动标
int Encoder;        //编码器的脉冲计数
int motor[4];           //电机PWM变量

float speed_X,speed_Y,speed_R;  //设定速度控制的目标速度为50个脉冲每10ms
float speedFbk[4],speedRef[4],posError[3];   //轮子速度反馈，速度期望，位置误差
float Xc,Yc,Rc,actual_X,actual_Y,actual_R;   //位置反馈，整车速度反馈

PID_LocTypeDef speedPID[4],position_PID[3];

void PID_init(void){
	
	u8 i;
	for(i=0;i<4;i++)
	{
		speedPID[i].Kp = 354;
		speedPID[i].Ki = 0.39;
		speedPID[i].Kd = 1800;
		speedPID[i].iMax = 6900;
		speedPID[i].locMax=7200;	
	}

//	for(i=0;i<3;i++)
//	{
//		position_PID[i].Kp = 0.019;
//		position_PID[i].Ki = 0.25;
//		position_PID[i].Kd = 1;
//		position_PID[i].iMax=7000;
//		position_PID[i].locMax=7600;	11  0.007  1.9
//	}
	for(i=0;i<3;i++)
	{
		position_PID[i].Kp = 9;
		position_PID[i].Ki = 0.2;
		position_PID[i].Kd = 2;
		position_PID[i].iMax=2500;
		position_PID[i].locMax=3000;	
	}	
}

//void TIM4_IRQHandler(void)   //TIM6??
//{
//		if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //?????TIM??????:TIM ???
//		{			
//			
//			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);   //??TIMx???????:TIM ???
// 
//		}
//}
/**************************************************************************
函数功能：限制PWM赋值 
入口参数：pid输出值
返回  值：pwm设定值
**************************************************************************/
int PwmLimit(int moto) //限制幅度的函数
{
	int limit=7180,temp;  //===PWM满幅是7200 限制在7100
	if(moto < 0)
	{
	 if(moto < -limit)
			temp = -limit;
	 else
		 temp = -moto;
	}		 
	if(moto > limit)   temp =  limit;
	
	return temp;
}

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void SetPwm(int* moto)//赋值给PWM寄存器
{
	if(moto[0]>0) {RF1=1;RF2=0;}
	else   {  RF1=0;  RF2=1;} 
	PWMRF=PwmLimit(moto[0]);

	if(moto[1]>0) {LF1=0;LF2=1;}
	else   { LF1=1;  LF2=0; } 
	PWMLF=PwmLimit(moto[1]);
		
	if(moto[2]>0) {LB1=1;LB2=0;}
	else   { LB1=0;  LB2=1; } 
	PWMLB=PwmLimit(moto[2]);
		
	if(moto[3]>0){RB1=1;RB2=0;}		
	else 		{ RB1=0;  RB2=1; }   
	PWMRB=PwmLimit(moto[3]);
}

/*
***************************************************
函数名：reVelocity
功能：麦克纳姆轮角速度计算
入口参数：	Wheel_Speed：底盘电机速度
返回值：V_Rotate：底盘角速度
应用范围：外部调用
备注：
***************************************************
*/
u8 GYRO[12],YAW[12];
float Wy,Wz,Yaw,k=0.8;

float reVelocity(float* Wheel_Speed){

	u8 i;
//	s16 data,sum1=0,sum2=0;
	float V_Rotate=0;   
	for( i= 0; i < 4; i++)
		V_Rotate += Wheel_Speed[i];
  
//	for(i=0;i<10;i++)
//	{
//		sum1+=GYRO[i];
//		sum2+=YAW[i];
//	}
//	if((sum1&0x00FF)==GYRO[10])
//	{
//		data=(short)((short)(GYRO[5]<<8)|GYRO[4]);
//		Wy=data*2000/32765;
//		data=(short)((short)(GYRO[7]<<8)|GYRO[6]);
//		Wz=data*2000/32765;
//	}
//	if((sum2&0x00FF)==YAW[10])
//	{
//		data=(short)((short)(YAW[7]<<8)|YAW[6]);
//		Yaw=data*180/32765;
//	}

	Yaw=actual_R;
//	Wz=actual_X;
//	Wy=actual_Y;
	actual_R = k*(-V_Rotate/4)+(1-k)*Yaw;
	actual_X=(Wheel_Speed[2]+Wheel_Speed[3]-Wheel_Speed[0]-Wheel_Speed[1])/4;
	actual_Y=(Wheel_Speed[3]+Wheel_Speed[0]-Wheel_Speed[1]-Wheel_Speed[2])/4;

//	actual_X=(Wheel_Speed[2]+Wheel_Speed[3]-Wheel_Speed[0]-Wheel_Speed[1])/4;
//	actual_X=k*actual_X+(1-k)*Wz;
//	actual_Y=(Wheel_Speed[3]+Wheel_Speed[0]-Wheel_Speed[1]-Wheel_Speed[2])/4;
//	actual_Y=k*actual_Y+(1-k)*Wy;
//	if(Wheel_Speed[2]<0 && Wheel_Speed[3]>0 && Wheel_Speed[0]<0 && Wheel_Speed[1]>0)
//		actual_X=(Wheel_Speed[3]);
//	else if(Wheel_Speed[2]>0 && Wheel_Speed[3]<0 && Wheel_Speed[1]<0 && Wheel_Speed[0]>0)
//		actual_X=(Wheel_Speed[3])/2;

//	if(Wheel_Speed[2]<0 && Wheel_Speed[3]>0 && Wheel_Speed[1]<0 && Wheel_Speed[0]>0)
//		actual_Y=Wheel_Speed[0];
//	else if(Wheel_Speed[2]>0 && Wheel_Speed[3]<0 && Wheel_Speed[0]<0 && Wheel_Speed[1]>0)
//		actual_Y=Wheel_Speed[0];

	return V_Rotate;
}

float a=0.75,value,arf=1.1,t=0;
float correcte(float ek)
{
//	float ek1;
//	ek=myabs(ek);	
//		 + correcte(speedFbk[i]),&speedPID[i]a*ek+(1-a)*ek1
//	ek1=ek;	
	value=arf*(50-ek)*(50-ek)+1500;
	return value;
}

u8 test(float x,int i)
{
	float temp;
	flagStop=0;
	Xc=0;
	temp=x*1768/(PI*48)*1.04;
	while(1){
		actual_X=speedFbk[i];
		speed_X=PID_Calculate(temp,Xc,&position_PID[0])/76;		
		motor[i]=Incremental_PI(speed_X,speedFbk[i]);
//		motor[i]=PID_Calculate(speed_X,speedFbk[i],&speedPID[i]);
		if(x==0)
			posError[0]=0;
		else
			posError[0]=(temp-Xc)/temp;
		if(myabs(posError[0])>LINE_ERROR)
		{
			flagMove=1;
		}
		else
		{
			flagMove=0;
			return flagMove;
		}
	}
}

uint8_t moving(int x,int y,int r)
{
	u8 i=0;
	float Xt,Yt,Rt;
	
	Xc=0;	Yc=0; Rc=0;	
	flagStop=0;
	
	Xt=x*1768/(PI*48)*0.74;
	Yt=y*1768/(PI*48)*0.74;
	Rt=r*1768*11/48/18;
	posError[0]=0;
	posError[1]=0;
	posError[2]=0;		
	while(1)
	{
		reVelocity(speedFbk);
		flagMove=0;
		if(x)	
			posError[0]=(Xt-Xc)/Xt;		
		if(y)
			posError[1]=(Yt-Yc)/Yt;	
		if(r)
			posError[2]=(Rt-Rc)/Rt;

		if(myabs(posError[0])>LINE_ERROR)
		{
			speed_X=-PID_Calculate(Xt,Xc,&position_PID[0])/76;
			t=correcte(actual_X);
			flagMove=1;
		}
		else
		{
			speed_X=0;
		}
		if(myabs(posError[1])>LINE_ERROR)
		{
			speed_Y=-PID_Calculate(Yt,Yc,&position_PID[1])/76;
			t=correcte(actual_Y);
			flagMove=1;
		}
		else
		{
			speed_Y=0;
		}		
		if(myabs(posError[2])>LINE_ERROR)
		{
			speed_R=-PID_Calculate(Rt,Rc,&position_PID[2])/76;
			t=correcte(actual_R);
			flagMove=1;
		}	
		else
		{
			speed_R=0;
		}		
		mecanumCalculate(speed_X,speed_Y,speed_R,200,speedRef);
		
		for(i=0;i<4;i++)
		{
//			motor[i]=PID_Calculate(speedRef[i],speedFbk[i],&speedPID[i]);
			motor[i]=Incremental_PI(speedRef[i],speedFbk[i]);
//			if(motor[i]<0)
//				motor[i]-=t;
//			else if(motor[i]>0)
//				motor[i]+=t;
		}

		if(flagMove==0)
		{
			return flagMove;
		}
	}
//	return 0;
}

float calibrateTime,at=20;
u8 adjust(int x,int y,int r,u8 t)//速度用+-15，时间1对100ms
{
		u8 i=0;
//	float Xt,Yt,Rt;
	
	Xc=0;	Yc=0; Rc=0;	
	flagStop=0;
	flagMove=2;	

	calibrateTime=t*10;		

	while(1)
	{
		reVelocity(speedFbk);	
		mecanumCalculate(-x,-y,-r,100,speedRef);
		
		for(i=0;i<4;i++)
		{
//			motor[i]=PID_Calculate(speedRef[i],speedFbk[i],&speedPID[i]);
			motor[i]=Incremental_PI(speedRef[i],speedFbk[i]);
		}
		if(calibrateTime<=2)
		{
			stop();
			return 		flagMove=0;	
		}
	}
}

//u8 adjust(int x,int y,int r,u8 t)
//{
//		u8 i=0;
////	float Xt,Yt,Rt;
//	
//	Xc=0;	Yc=0; Rc=0;	
//	flagStop=0;
//	flagMove=2;	

//	x*=-8.6761;
//	y*=-8.6761;
//	r*=-22.51;
//	calibrateTime=t*10;		

//	while(1)
//	{
//		reVelocity(speedFbk);	
//		mecanumCalculate(x,y,r,100,speedRef);
//		
//		for(i=0;i<4;i++)
//		{
////			motor[i]=PID_Calculate(speedRef[i],speedFbk[i],&speedPID[i]);
//			motor[i]=Incremental_PI(speedRef[i],speedFbk[i]);
//			if(motor[i]<0)
//				motor[i]-=2100;
//			else if(motor[i]>0)
//				motor[i]+=2100;
//		}
//		if(calibrateTime<=2)
//		{
//			stop();
//			return 		flagMove=0;	
//		}
//	}
//}

//u16 up1=289,down1=240,up2=170,down2=150;
u8 operate(s8 p,s8 order)
{
	switch(p)
	{
		case 1:
			switch(order)
			{
				case 0:Servo1_SetAngle(260);break;	//轮盘//
				case 1:Servo1_SetAngle(119);break;	//轮盘//
				case 2:Servo1_SetAngle(76);break;	//轮盘//
				case 3:Servo1_SetAngle(31);break;	//轮盘//
			}
		break;
		case 2:
			if(order)
				Servo3_SetAngle(150);//爪子	
			else
				Servo3_SetAngle(135);//爪子	
		break;
		case 3:
			if(order>0)
				Servo2_Set(-1000,order);//传动带	
			else if(order<0)
				Servo2_Set(800,order);//传动带	
			else
				Servo2_Set(0,order);//传动带	
		break;			
	}
	return 0;
}

void stop(void)
{
	speed_X=0;
	speed_Y=0;
	speed_R=0;
	flagStop=1;
	mecanumCalculate(speed_X,speed_Y,speed_R,1,speedRef);		
}

u8 schedule(int x)
{
	if(x)
		return 1;
	else
		return 0;
}

int colorchange;

int TIM1_UP_IRQHandler(void)
{
	u8 i;
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);   //===清除定时器1中断标志位
		for(i=0;i<4;i++)
		{
			Encoder=(int)Read_Encoder(i);	 //取定时器2计数器的值

			speedFbk[i]=Encoder;		
		}
					
		if(flagMove)
		{
			Xc+=actual_X;
			Yc+=actual_Y;
			Rc+=actual_R;	 
		}
		else if(flagStop)
		{
			flagStop++;
			for(i=0;i<4;i++)
			{
				motor[i]=PID_Calculate(0,speedFbk[i],&speedPID[i]);
			}
			if(flagStop>3)
			{
				flagStop=0;
				for(i=0;i<4;i++)
				{
					motor[i]=0;
				}
			}	 
		}
		if(flagMove>1)
			calibrateTime-=1;
		
		colour(&rx,&ry,&gx,&gy,&bx,&by,colorchange);//???:0,???:1?2?3?

//		if(PCin(2)==0)
//			flagBreak++;
//		else
//			flagBreak=0;

//		if(( bx<=159 && by>=132) || ( bx<=159 && by>=132) || ( rx<=159 && ry>=132) || flagBreak>2)
//		{		
//			flagBreak=0;
//			operate(3,0);
//		}

//		if(schedule(i)==1)
//		{

//		}
		

//		motor[0]=PID_Calculate(speed_X,speedFbk[0],&speedPID[0]);    //===位置PID控制器
		//		moto=Incremental_PI(Encoder2,Target_velocity);
		SetPwm(motor);
	}
	return 0;
}



