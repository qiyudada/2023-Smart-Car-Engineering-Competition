#include "string.h"
#include "stdlib.h"
#include "stm32f10x.h" // Device header
#include "BlueTooth.h"
#include "Serial.h"
#include "OLED.h"
#include "Key.h"

int CTIMEO=1000;
int CTIMET=800;
int CTIMES=500;
int CTIMEF=300;
int Depart=0;
char substring[8];
char QRC1[4];
char QRC2[4];
int QRN1;
int QRN2;
int first[3];//指令顺序1
int second[3];//指令顺序2
int MA=-170;
int MB=520;
int MC=850;
int MD=450;
int ME=700;
int MF=700;

void BlueDebug(void)
{
	Key_Init();
	while(1)
	{
		if(Key_GetNum()==1)
		{
			Depart=1;
			break;
		}
		else if(Serial3_RxFlag == 1)
		{
			if (strncmp(Serial3_RxPacket, "CTIMEO", 6) == 0) 
			{
				strncpy(substring, &Serial3_RxPacket[6], 4);
				substring[4] = '\0'; // 添加字符串结束符
				CTIMEO = atoi(substring);
				Serial_SendString(USART3,"CTIMEOALTER\n");
				Serial_SendNumber(USART3,CTIMEO,4);
				//OLED_ShowNum(1,1,CTIMEO,4);
				Serial3_RxFlag=0;
			}
			else if (strncmp(Serial3_RxPacket, "CTIMET", 6) == 0) 
			{
				strncpy(substring, &Serial3_RxPacket[6], 3);
				substring[3] = '\0'; // 添加字符串结束符
				CTIMET = atoi(substring);
				Serial_SendString(USART3,"CTIMETALTER\n");
				Serial_SendNumber(USART3,CTIMET,3);
//				OLED_ShowNum(1,1,CTIMET,3);
				Serial3_RxFlag=0;
			}
			else if (strncmp(Serial3_RxPacket, "CTIMES", 6) == 0) 
			{
				strncpy(substring, &Serial3_RxPacket[6], 3);
				substring[3] = '\0'; // 添加字符串结束符
				CTIMES = atoi(substring);
				Serial_SendString(USART3,"CTIMESALTER\n");
				Serial_SendNumber(USART3,CTIMES,3);
//				OLED_ShowNum(1,1,CTIMES,3);
				Serial3_RxFlag=0;
			}
			else if (strncmp(Serial3_RxPacket, "MA", 2) == 0) 
			{
				strncpy(substring, &Serial3_RxPacket[2], 4);
				substring[4] = '\0'; // 添加字符串结束符
				MA = atoi(substring);
				Serial_SendString(USART3,"MAALTER\n");
				Serial_SendNumber(USART3,MA,3);
//				OLED_ShowSignedNum(1,1,MA,3);
				Serial3_RxFlag=0;
			}
			else if (strncmp(Serial3_RxPacket, "MB", 2) == 0) 
			{
				strncpy(substring, &Serial3_RxPacket[2], 3);
				substring[3] = '\0'; // 添加字符串结束符
				MB = atoi(substring);
				Serial_SendString(USART3,"MBALTER\n");
				Serial_SendNumber(USART3,MB,3);
//				OLED_ShowSignedNum(1,1,MB,3);
				Serial3_RxFlag=0;
			}
			else if (strncmp(Serial3_RxPacket, "MC", 2) == 0) 
			{
				strncpy(substring, &Serial3_RxPacket[2], 3);
				substring[3] = '\0'; // 添加字符串结束符
				MC = atoi(substring);
				Serial_SendString(USART3,"MCALTER\n");
				Serial_SendNumber(USART3,MC,3);
//				OLED_ShowSignedNum(1,1,MC,3);
				Serial3_RxFlag=0;
			}
			else if (strncmp(Serial3_RxPacket, "MD", 2) == 0) 
			{
				strncpy(substring, &Serial3_RxPacket[2], 3);
				substring[3] = '\0'; // 添加字符串结束符
				MD = atoi(substring);
				Serial_SendString(USART3,"MDALTER\n");
				Serial_SendNumber(USART3,MD,3);
//				OLED_ShowSignedNum(1,1,MD,3);
				Serial3_RxFlag=0;
			}
			else if (strncmp(Serial3_RxPacket, "ME", 2) == 0) 
			{
				strncpy(substring, &Serial3_RxPacket[2], 3);
				substring[3] = '\0'; // 添加字符串结束符
				ME = atoi(substring);
				Serial_SendString(USART3,"MEALTER\n");
				Serial_SendNumber(USART3,ME,3);
//				OLED_ShowSignedNum(1,1,ME,3);
				Serial3_RxFlag=0;
			}
			else if (strncmp(Serial3_RxPacket, "MF", 2) == 0) 
			{
				strncpy(substring, &Serial3_RxPacket[2], 3);
				substring[3] = '\0'; // 添加字符串结束符
				MF = atoi(substring);
				Serial_SendString(USART3,"MFALTER\n");
				Serial_SendNumber(USART3,MF,3);
//				OLED_ShowSignedNum(1,1,MF,3);
				Serial3_RxFlag=0;
			}
			else if (strncmp(Serial3_RxPacket, "QR", 2) == 0) 
			{
				strncpy(substring, &Serial3_RxPacket[2], 7);
				substring[7] = '\0'; // 添加字符串结束符
				sscanf(substring, "%[^+]", QRC1);
				sscanf(substring, "%*4s %s",QRC2); 
				Serial_SendString(USART3,"QRALTER\n");
				Serial_SendString(USART3,substring);
//				OLED_ShowString(1,1,QRC1);
//				OLED_ShowString(1,5,QRC2);
				//printf("n1.val=%s\xff\xff\xff",QR1);//第一个
				//printf("n0.val=%s\xff\xff\xff",QR2);//第二个
				sscanf(QRC1, "%d", &QRN1);
				sscanf(QRC2, "%d", &QRN2);
				first[0]=QRN1/100%10;
				first[1]=QRN1/10%10;
				first[2]=QRN1/1%10;
				second[0]=QRN2/100%10;
				second[1]=QRN2/10%10;
				second[2]=QRN2/1%10;
//				OLED_ShowNum(2,1,first[0],1);
//				OLED_ShowNum(3,1,first[1],1);
//				OLED_ShowNum(4,1,first[2],1);
//				OLED_ShowNum(2,2,second[0],1);
//				OLED_ShowNum(3,2,second[1],1);
//				OLED_ShowNum(4,2,second[2],1);
				Serial3_RxFlag=0;
			}
			else if (strcmp(Serial3_RxPacket, "exit") == 0)
			{
				Serial_SendString(USART3,"BTED");
				Depart=1;
				OLED_ShowNum(3,3,Depart,1);
				break;
			}
		}
	}
}

