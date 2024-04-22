#ifndef __CAL_SUPPORT_H
#define __CAL_SUPPORT_H

typedef struct
{
	float Kp;                       
	float Ki;                       
	float Kd;                       
	
	float pMax;                   
	float iMax;                     
	float dMax;                     
	float locMax;
	
	float PIDLoc;                 
 
	float Ek;                       
	float Ek1;                      
	float Ek2;                      
	float EkSum;                   
}PID_LocTypeDef;

void mecanumCalculate(float V_X,float V_Y,float V_Rotate,float MaxWheelSpeed,float* Wheel_Speed);
float PID_Calculate(float SetValue, float ActualValue, PID_LocTypeDef *PID);
int Incremental_PI (int Encoder2,int Target);
float myabs(float x);
#endif

