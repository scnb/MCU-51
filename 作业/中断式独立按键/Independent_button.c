#include<reg52.h>

/*���ĸ�������������IO������*/
sbit sw_17 = P2^3;//KEY4
sbit sw_18 = P2^2;//KEY3
sbit sw_19 = P2^1;//KEY2
sbit sw_20 = P2^0;//KEY1
sbit down_flag = P3^2;//�������±�־λ	 ����P3^2���ⲿ�ж�0�Ŀ�

/*����ܶ�Ӧ�Ĵ�������*/
sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;

/*�����λѡ�Ͷ�ѡ��*/
code unsigned char Duan_choose[5] = {0x06,0x5b,0x4f,0x66,0x3f};
code unsigned char Wei_choose[4] =  {0xfe,0xfd,0xfb,0xf7};

unsigned char i;//��Ӧ������������
/*����ܷ������ݺ���*/
void Send_data(unsigned char duan,unsigned char wei)
{
	unsigned char i;	   //������hc595��λ�Ĵ��������в���������������ܵ�ԭ����ʱ����д����
 	HC595_RCK = 0;
	for(i = 0;i < 8;i++)
	{
	 	HC595_SCK = 0;
		if(0 != (duan&0x80))
		{
		 	HC595_DATA = 1;
		}
		else
		{
		 	HC595_DATA = 0;
		}
		HC595_SCK = 1;
		duan = duan << 1;
	}
	for(i = 0;i < 8;i++)
	{
	 	HC595_SCK = 0;
		if(0 != (wei&0x80))
		{
		 	HC595_DATA = 1;
		}
		else 
		{
		 	HC595_DATA = 0;
		}
		HC595_SCK = 1;
		wei = wei << 1;
	}
	HC595_RCK = 1;
}


/*�ж��Ƿ��ж����������£������ж����ĸ�����������*/
void Which_down()
{
	if(0 == down_flag)
	{
	 	down_flag = 1;
		if(sw_17 == 0)
		{
			if(sw_17 == 0)
			{
		 		i = 0;
			}
		}
		if(sw_18 == 0)
		{
			if(sw_18 == 0)
			{
		 		i = 1;
			}
		}
		if(sw_19 == 0)
		{
			if(sw_19 == 0)
			{
		 		i = 2;
			}
		}
		if(sw_20 == 0)
		{
			if(sw_20 == 0)
			{
		 		i = 3;
			}
		}
	}
}


/*�жϳ�ʼ������*/
void Init_interrupt()
{
 	EA = 1;
	EX0 = 1;
	IT0 = 0;//����ʽ�������ж�������Ʒ�ʽ�ǵ͵�ƽ��Ч����ƽ������ʽ
}


/*������*/
void main()
{
	Init_interrupt();
	Send_data(Duan_choose[4],Wei_choose[0]);
	while(1)
	{
		Which_down();
	}
}


/*�ⲿ�ж�0  ��Ϊ����������һ���ⲿ�ж�*/
void interrupt_0() interrupt 0
{
	Send_data(Duan_choose[i],Wei_choose[i]);
}