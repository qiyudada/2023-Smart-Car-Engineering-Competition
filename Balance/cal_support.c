#include "cal_support.h"

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
float myabs(float x) //ȡ����ֵ
{ 		   
	 float temp;
	temp=(x>=0)?x:-x;
	 return temp;
}

/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI (int Encoder2,int Target)
{ 	
   float Kp=20,Ki=30;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder2-Target;                //����ƫ��
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
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
��������mecanumCalculate
���ܣ������ķ���ٶȼ���
��ڲ�����	V_X�������ٶȣ���Ϊ����
					V_Y��ǰ���ٶȣ�ǰΪ����
					V_Rotate����ת�ٶȣ�˳ʱ��Ϊ����
					MaxWheelSpeed����������ٶ����ƣ�����ֵ��
					Wheel_Speed���ٶ�ָ���������ʱ��Ϊ����

����ֵ����
Ӧ�÷�Χ���ⲿ����
��ע��
***************************************************
*/
void mecanumCalculate(float V_X,float V_Y,float V_Rotate,float MaxWheelSpeed,float* Wheel_Speed){
	float Buffer[4],MaxSpeed;
	unsigned char index;
	Buffer[0] = V_Rotate + V_X - V_Y ;
	Buffer[1] = V_Rotate + V_X + V_Y ;
	Buffer[2] = V_Rotate - V_X + V_Y ;
	Buffer[3] = V_Rotate - V_X - V_Y ;

	//�޷�
	for(index = 0, MaxSpeed = 0; index < 4; index++){
		if((Buffer[index] > 0 ? Buffer[index] : -Buffer[index]) > MaxSpeed){
			MaxSpeed = (Buffer[index] > 0 ? Buffer[index] : -Buffer[index]);
		}
	}
	if(MaxWheelSpeed < MaxSpeed){
		Wheel_Speed[0] = Buffer[0];
		Wheel_Speed[1] = Buffer[1];
		Wheel_Speed[2] = Buffer[2];
		Wheel_Speed[3] = Buffer[3];
	}
	else{
		Wheel_Speed[0] = Buffer[0];
		Wheel_Speed[1] = Buffer[1];
		Wheel_Speed[2] = Buffer[2];
		Wheel_Speed[3] = Buffer[3];
	}
}


