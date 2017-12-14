#include<reg52.h>

sbit HC595_SCK = P0^5;
sbit HC595_RCK = P0^6;
sbit HC595_DATA = P0^7;


code unsigned char Led_One[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
code unsigned char Led_Two[16] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//����������������Ϊ�˽��main�������Ǹ�������뷨�����ⶨ���
code unsigned char Led_One[16] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
code unsigned char Led_Two[16] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

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

void Delay_1ms(int i)
{
 	int j;
	for(;i > 0;i--)
	{
	 	for(j = 0;j < 110;j++)
		{}
	}
}		  	

void main()
{
 	unsigned char k;
	for(k = 0;k < 8;k++)
	{	
		Send_data(Led_One[15-k],Led_Two[k]);
		Delay_1ms(1000);
	}
	//�ڶ����汾�������ٴ�ͷ�������������ǽ����ȥ������������,Ȼ���뷨�Ǵ���ģ���Ϊk��8֮��ͽ���������led����ԶΪ�������ֵ
	/*for(k = 0;k < 16;k++)
	{	
		Send_data(Led_One[15-k],Led_Two[k]);
		Delay_1ms(1000);
	}*/
}