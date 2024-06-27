#include "cal_support.h"

/**************************************************************************
	函数功能：绝对值函数
	入口参数：int
	返回  值：unsigned int
**************************************************************************/
float myabs(float x) //取绝对值
{ 		   
	 float temp;
	 temp=(x>=0)?x:-x;
	 return temp;
}

/**************************************************************************
	函数功能：增量PI控制器
	入口参数：编码器测量值，目标速度
	返回  值：电机PWM
	根据增量式离散PID公式 
	pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
	e(k)代表本次偏差 
	e(k-1)代表上一次的偏差  以此类推 
	pwm代表增量输出
	在我们的速度控制闭环系统里面，只使用PI控制
	pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
float Kp=280,Ki=0.3;//PI parameter	
int Incremental_PI (int Target,int Encoder2)
{ 	

	static int Bias,Pwm,Last_bias,corret=0;
	Bias=Target-Encoder2;                //计算偏差	
	Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	if(Pwm>7200)
		Pwm=7200;
	else if(Pwm<-7200)
		Pwm=-7200;
	Last_bias=Bias;	                   //保存上一次偏差 
	if(Pwm>0)
		corret=1900;
	else
		corret=-1900;
	return Pwm + corret;                         //增量输出
}

float PID_Calculate(float SetValue, float ActualValue, PID_LocTypeDef *PID)
{
	PID->Ek = SetValue - ActualValue;          
	 
	PID->EkSum += PID->Ek;
	if(PID->EkSum>PID->iMax)
		PID->EkSum=PID->iMax;
	else if(PID->EkSum<-PID->iMax)
		PID->EkSum=-PID->iMax;
		 
  PID->PIDLoc = PID->Kp * PID->Ek + (PID->Ki * PID->EkSum) + PID->Kd * (PID->Ek - PID->Ek1);
 
	if(PID->PIDLoc>PID->locMax)
		PID->PIDLoc=PID->locMax;
	else if(PID->PIDLoc<-(PID->locMax))
		PID->PIDLoc=-PID->locMax;

	PID->Ek2 = (PID->Ek) - (PID->Ek1);

  PID->Ek1 = PID->Ek;  
	
	return PID->PIDLoc; 
}

/*
***************************************************
	函数名：mecanumCalculate
	功能：麦克纳姆轮速度计算
	入口参数：	V_X：左右速度（右为正）
				V_Y：前后速度（前为正）
				V_Rotate：旋转速度（顺时针为正）
				MaxWheelSpeed：最大轮子速度限制（绝对值）
				Wheel_Speed：速度指针变量（逆时针为正）

	返回值：无
	应用范围：外部调用
	备注：
***************************************************
*/
void mecanumCalculate(float V_X,float V_Y,float V_Rotate,float MaxWheelSpeed,float* Wheel_Speed)
{
	float Buffer[4],MaxSpeed=0;
	unsigned char index;
	Buffer[0] = V_Rotate + V_X - V_Y ;
	Buffer[1] = V_Rotate + V_X + V_Y ;
	Buffer[2] = V_Rotate - V_X + V_Y ;
	Buffer[3] = V_Rotate - V_X - V_Y ;

	//限幅
	for(index = 0; index < 4; index++)
	{
		Wheel_Speed[index] = Buffer[index];
		
//		if((Buffer[index] > 0 ? Buffer[index] : -Buffer[index]) > MaxSpeed){
//			MaxSpeed = (Buffer[index] > 0 ? Buffer[index] : -Buffer[index]);
//		}
	}
//	if(MaxWheelSpeed < MaxSpeed){
//		Wheel_Speed[0] = Buffer[0];
//		Wheel_Speed[1] = Buffer[1];
//		Wheel_Speed[2] = Buffer[2];s
//		Wheel_Speed[3] = Buffer[3];
//	}
//	else{
//		Wheel_Speed[0] = Buffer[0];
//		Wheel_Speed[1] = Buffer[1];
//		Wheel_Speed[2] = Buffer[2];
//		Wheel_Speed[3] = Buffer[3];
//	}
}


