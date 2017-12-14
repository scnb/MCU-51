#include<reg51.h>    //������reg52.h����reg52.h���Ӽ���
//reg51.h���������κ�һ��51��Ƭ�����е�sfr����reg52.h��reg51.h������
//#include<reg52.h>
#include "include/main.h"
#include "include/hc595.h"	 //�������õ���.c�ļ��ǲ���Ҳ�����أ�
#include "include/delay.h"
#include <intrins.h>


//�жϳ���  ����⵽�а������µ�ʱ���ڼ����жϳ���

void Interrupt_Led()  interrupt 0 using 0
{
	
	//P2 = 0xff;
	if(P2 != 0xff)
	{
		if(0xf7 == P2)
		{
			Display[0] = 0x01;
		}
		else if(0xfb == P2)
		{
			Display[0] = 0x02;
		}
		else if(0xfd == P2)
		{
			Display[0] = 0x04;
		}
		else if(0xfe == P2)
		{
			Display[0] = 0x08;
		}
	}
	Send_Data(Display[0],0x00);		
}

void Init_Fun()
{
	Send_Data(0x00,0x00);
}


void main(void)
{
	P2 = 0xff;
	EA = 1;
	IT0 = 0;
	EX0 = 1;	
	//Send_Data(0x00,0x00);

	Init_Fun();
	while(1)
	{
		delay1ms(3000);

		delay1ms(3000);
	}
}


