//�ö�ʱ��0�Ĺ�����ʽ������led��ˮ�Ƶ�ʵ��

#include<reg52.h>

unsigned char k = 0;
unsigned char num = 0;


sbit HC595_SCK = P0^5;
sbit HC595_RCK = P0^6;
sbit HC595_DATA = P0^7;

code unsigned char Led_One[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
code unsigned char Led_Two[16] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void Timer1_Init()
{
 	TMOD = 0x00;
	TH1 = (8192-4608)/32;			   //2^13=8192��������߶�ʱʱ����8.89ms,���ζ�ʱ����һ�ζ�5ms�������Ƿֱ��2^5=32�����ȡģ
	TL1 = (8192-4608)%32;
	EA = 1;		  //�ж������ܿ���λ
	ET1 = 1;	   //��ʱ��1�ж�����λ
	TR1 = 1;	  //��ʱ��1���п���λ
}

void Send_data(unsigned char Led8_one,unsigned char Led8_two)
{
	unsigned char i;
 	HC595_RCK = 0;
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
	 	if(0 != (Led8_one&0x80))
		{
		 	HC595_DATA = 1;
		}
		else
		{
		 	HC595_DATA = 0;
		}
		Led8_one = Led8_one << 1;
		HC595_SCK = 1;
	}
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
	 	if(0 != (Led8_two&0x80))
		{
		 	HC595_DATA = 1;
		}
		else
		{
		 	HC595_DATA = 0;
		}
		Led8_two = Led8_two << 1;
		HC595_SCK = 1;
	}
	HC595_RCK = 1;
}


void main()
{
	Timer1_Init();
	while(1)
	{
	
	}	
}


void interrupt_3() interrupt 3
{
	TH1 = (8192-4608)/32;		 //ÿһ�ν����ж�֮��Ҫ������λ��������
	TL1 = (8192-4608)%32;
		if(num == 200)			 //��һ���ַ���while������߷����ж����涼�ǿ��Եģ����ж�200��ʱ��˵���Ѿ���1s��
		{
			Send_data(Led_One[k],Led_Two[k]);	//�ڳ������ʼ������k=0
			if(k == 15)
			{
				k = 0;	//��ledѭ��һ��֮�����¿�ʼѭ��
				//Send_data(Led_One[0],Led_Two[15]);����֣�����������в��ü��������ͻ�������
			}
			else
			{				  
	 			k++;	 
			}
			num = 0;	 //�������ж�200��֮�󣬽�num�������㣬���¿�ʼ��ʱ1s
		}
		else
		{
 			num++;
		}
}