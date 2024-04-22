#ifndef __PWM_H
#define __PWM_H

void servo_Init(void);
void PWM8_SetCompare1(uint16_t Compare);
void PWM8_SetCompare2(uint16_t Compare);
void PWM8_SetCompare3(uint16_t Compare);
void Servo1_SetAngle(float Angle);
void Servo2_Set(int speed,int time);
void Servo3_SetAngle(float Angle);
void stepper(float Compare);
#endif
