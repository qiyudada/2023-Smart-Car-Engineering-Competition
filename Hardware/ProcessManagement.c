#include "stm32f10x.h"                  // Device header
#include "control.h"
#include "servo.h"
#include "openmv.h"
#include "OLED.h"

int first[3];
int second[3];
int count=0;
int rx,ry,gx,gy,bx,by;
int get_x,get_y;
int b=0;
float i=1,X=120,p=0,Y=0;
int executetime;
int CircularRing[3]={1,2,3};
int ColorArray[3];
//extern char gImage_guetpicture[512];


uint8_t ViewColor()
{
	int ColorNum=0;
	if(rx!=0&&ry!=0&&gx==0&&gy==0&&bx==0&&by==0)
	{
		ColorNum=1;
	}
	else if(rx==0&&ry==0&&gx!=0&&gy!=0&&bx==0&&by==0)
	{
		ColorNum=2;
	}
	else
	{
		ColorNum=3;
	}
	return ColorNum;
}



void OperationRunning(int *array)//�����ͣ�¡�ʶ��ץȡ
{	
		int time=0;
		int ColorNum1,ColorNum2,ColorNum;
		while(1)
		{
			while(1)
			{ 
				colour(&rx,&ry,&gx,&gy,&bx,&by,0);
				if((rx<170&&rx>160&&ry>150&&ry<160)||(gx<170&&gx>160&&gy>150&&gy<160)||(bx<170&&bx>160&&by>150&&by<160))
				{
					ColorNum1=ViewColor();
				}
				Delay_ms(200);
				colour(&rx,&ry,&gx,&gy,&bx,&by,0);
				if((rx<170&&rx>160&&ry>150&&ry<160)||(gx<170&&gx>160&&gy>150&&gy<160)||(bx<170&&bx>160&&by>150&&by<160))
				{
					ColorNum2=ViewColor();

				}
				if(ColorNum1==ColorNum2)
				{
					ColorNum=ColorNum2;
					break;
				}
			}
			
			if(time==0)
			{
				while(1)
				{
					if(ColorNum==array[0])
					{
						//��Ӧ��ɫ�Ļ�е�۶�����
						time++;
						break;
					}
					else 
					{
						break;
					}
				}	
			}
			if(time==1)
			{
				while(1)
				{
					if(ColorNum==array[1])
					{
						//��Ӧ��ɫ�Ļ�е�۶�����
						time++;
						break;
					}
					else 
					{
						break;
					}
				}	
			}
			if(time==2)
			{
				while(1)
				{
					if(ColorNum==array[2])
					{
						//��Ӧ��ɫ�Ļ�е�۶�����
						time++;
						break;
					}
					else 
					{
						break;
					}
				}	
			}
			if(time==3)
			{
				break;
			}
	}
}			


//step_3
void identifyPlace()//��Բ����λ��΢��
{
	
}

void put(int color)//�ƶ�����ӦԲ��λ�á�΢��������
{
	
	switch (color)
	{
	case 1:
	moving(0,0,0);
	identifyPlace();
	//��Ӧ��ɫץȡ�����á���λ�ĺ���
	break;
	case 2:
	moving(0,0,0);
	identifyPlace();
	////��Ӧ��ɫץȡ�����á���λ�ĺ���
		break;
	default:
	moving(0,0,0);
	identifyPlace();
	////��Ӧ��ɫץȡ�����á���λ�ĺ���
		break;
	}
}


void ready_to_zancun(int color)
{
	switch (color)
	{
	case 1:
	moving(0,0,0);
	break;
	
	case 2:
	moving(0,0,0);
	break;
	
	default:
	moving(0,0,0);
	break;
	}
}


void ready_to_yuanliao(int color)
{
		switch (color)
	{
	case 1:
	moving(0,0,0);
	break;
	
	case 2:
	moving(0,0,0);
	break;
	
	default:
	moving(0,0,0);
	break;
	}
}


