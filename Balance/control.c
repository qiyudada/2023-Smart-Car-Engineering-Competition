#include "control.h"
#include "cal_support.h"

u8 flagStop,flagMove;     //�˶���־λ
int Encoder;        //���������������
int motor[4];           //���PWM����

float speed_X,speed_Y,speed_R;  //�趨�ٶȿ��Ƶ�Ŀ���ٶ�Ϊ50������ÿ10ms
float speedFbk[4],speedRef[4],posError[3];   //�����ٶȷ������ٶ�������λ�����
float Xc,Yc,Rc,actual_X,actual_Y,actual_R;   //λ�÷����������ٶȷ���

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

	for(i=0;i<3;i++)
	{
		position_PID[i].Kp = 0.025;
		position_PID[i].Ki = 1.1;
		position_PID[i].Kd = 1;
		position_PID[i].iMax=70;
		position_PID[i].locMax=100;	
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
�������ܣ�����PWM��ֵ 
��ڲ�����pid���ֵ
����  ֵ��pwm�趨ֵ
**************************************************************************/
int PwmLimit(int moto) //���Ʒ��ȵĺ���
{
	int limit=7100,temp;  //===PWM������7200 ������7100
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
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void SetPwm(int* moto)//��ֵ��PWM�Ĵ���
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
��������reVelocity
���ܣ������ķ�ֽ��ٶȼ���
��ڲ�����	Wheel_Speed�����̵���ٶ�
����ֵ��V_Rotate�����̽��ٶ�
Ӧ�÷�Χ���ⲿ����
��ע��
***************************************************
*/
float reVelocity(float* Wheel_Speed){

	u8 i;
	float V_Rotate=0;   
	for( i= 0; i < 4; i++)
  	V_Rotate += Wheel_Speed[i];
  	
//	actual_R = -V_Rotate/4;
	actual_X=(Wheel_Speed[2]+Wheel_Speed[3]-Wheel_Speed[0]-Wheel_Speed[1])/4;
	actual_Y=(Wheel_Speed[3]+Wheel_Speed[0]-Wheel_Speed[1]-Wheel_Speed[2])/4;

	return V_Rotate;
}

float a=0.75,value,k=0.6,t=80;
float correcte(float ek)
{
	float ek1;
	ek=a*ek+(1-a)*ek1;	
//	if(ek>t)
//		ek=80;
//	else if(ek<-t)
//		ek=-80;
	ek1=ek;	
	value=k*ek*ek;
	return value;
}

u8 test(float x,int i)
{
	float temp;
	flagStop=0;
	Xc=0;
	temp=x*1768/(PI*45)*0.9;
	while(1){
//		actual_X=speedFbk[i];
//		speed_X=PID_Calculate(temp,Xc,&position_PID[0]);		
		motor[i]=PID_Calculate(speed_X,speedFbk[i],&speedPID[i]) + correcte(speedFbk[i]);
	
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

uint8_t flag;
uint8_t moving(int x,int y,int r)
{
	u8 i=0;
	float Xt,Yt,Rt;
	
	Xc=0;	Yc=0; Rc=0;	
	flagStop=0;
	
	Xt=x*1768/(PI*48);
	Yt=y*1768/(PI*48);
	Rt=r*1768/(PI*48);
	posError[0]=0;
	posError[1]=0;
	posError[2]=0;		
	while(1)
	{
		reVelocity(speedFbk);
		flag=0;
		if(x)	
			posError[0]=(Xt-Xc)/Xt;		
		if(y)
			posError[1]=(Yt-Yc)/Yt;	
		if(r)
			posError[2]=(Rt-Rc)/Rt;

		if(myabs(posError[0])>LINE_ERROR)
		{
			speed_X=PID_Calculate(Xt,Xc,&position_PID[0]);
			flag=1;
		}
		else
		{
			speed_X=0;
		}
		if(myabs(posError[1])>LINE_ERROR)
		{
			speed_Y=PID_Calculate(Yt,Yc,&position_PID[1]);
			flag=1;
		}
		else
		{
			speed_Y=0;
		}		
		if(myabs(posError[2])>LINE_ERROR)
		{
			speed_R=PID_Calculate(Rt,Rc,&position_PID[2]);
			flag=1;
		}	
		else
		{
			speed_R=0;
		}		
		mecanumCalculate(speed_X,speed_Y,speed_R,100,speedRef);
		
		for(i=0;i<4;i++)
		{
			motor[i]=-PID_Calculate(speedRef[i],speedFbk[i],&speedPID[i]);
		}

		if(flag)
		{
			flagMove=1;
		}
		else
		{
			flagMove=0;
			return flagMove;
		}
	}
//	return 0;
}

u8 adjust()
{
	return 0;
}

u8 operate()
{
	
	
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

int TIM1_UP_IRQHandler(void)
{
	u8 i;
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);   //===�����ʱ��1�жϱ�־λ
		
		for(i=0;i<4;i++)
		{
			Encoder=(int)Read_Encoder(i);	 //ȡ��ʱ��2��������ֵ

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

//		motor[0]=PID_Calculate(speed_X,speedFbk[0],&speedPID[0]);    //===λ��PID������
		//		moto=Incremental_PI(Encoder2,Target_velocity);
		SetPwm(motor);
	}
	return 0;
}

