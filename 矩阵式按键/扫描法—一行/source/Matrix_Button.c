//�����������ڵ�һ�����һ��֮��ת������Ķ���P2_0 = 0����P2_4 = 0 ,Ȼ���ٸı��жϺ������ʮ��������ֵ
//�������Ҳ���Ըĳ������������һ�У������������һ��


#include <reg51.h>
#include "include/delay.h"
#include "include/main.h"
#include "intrins.h"
#include "include/hc595.h"


void Key_Check_Dis() interrupt 0 using 0       //�жϳ���
{
	
	//P2 = 0xf0;							   //���д����仰����ô���еİ��������Ա�����ȥ
	if(0x0e == (P2&0x0f))				//��0xe0	   //�����0x0f,�����У������0xf0��������
	{
		Display[0] = Shu_Duan_Chose[1];	 
		Send_Data(Display[0],0xfe);
	}
	else if(0x0d == (P2&0x0f))				//��0xd0
	{
		Display[0] = Shu_Duan_Chose[5];
		Send_Data(Display[0],0xfe);
	}
	else if(0x0b == (P2&0x0f))				//��0xb0
	{
		Display[0] = Shu_Duan_Chose[9];
		Send_Data(Display[0],0xfe);
	}
	else if(0x07 == (P2&0x0f))			   //��0x70
	{
	    Display[0] = Shu_Duan_Chose[13];
		Send_Data(Display[0],0xfe);
	}
	//Send_Data(Display[0],0xfe);
}

/*void Init_Show()
{
	Send_Data(0x3f,0xfe);
} */




void main()
{
	Send_Data(0x3f,0xfe);
	EA = 1;
	EX0 = 1;
	IT0 = 0;
	TR0 = 0;
	//Init_Show();
	
	//P2_0 = 0;			//�˶���˵���ǵ���λ��P2^0���ó��˵͵�λ ,�����ǵ�һ��
	P2_4 = 0;			//�˶���˵���Ǹ���λ��P2^4���ó��˵͵�λ�������ǵ�һ��
	P0 = 0xff;
	P1 = 0xff;
	while(1)
	{
		Delay1ms(2000);
		Delay1ms(2000);
	}
}