//用定时器0的工作方式来进行led流水灯的实验

#include<reg52.h>




sbit HC595_SCK = P0^5;
sbit HC595_RCK = P0^6;
sbit HC595_DATA = P0^7;

code unsigned char Led_One[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
code unsigned char Led_Two[16] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void Timer0_Init()
{
 	TMOD = 0x00;
	TH0 = (8192-4608)/32;			   //2^13=8192，单次最高定时时间是8.89ms,本次定时考虑一次定5ms
	TL0 = (8192-4608)%32;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
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
	
	unsigned char counter;
	unsigned char k = 0;Timer0_Init();

	while(1)
	{ 
		if(1 == TF0)
		{
		 	TF0 = 0;
			TH0 = (8192-4608)/32;
			TL0 = (8192-4608)%32;
			counter++;
		}
		if(200 == counter)
		{
			counter = 0;
		 	Send_data(Led_One[k],Led_Two[k]);
			if(15 == k)
			{
			 	k = 0;
				//Send_data(Led_One[0],Led_Two[15]);   //在这种不使用中断的程序中就不需要加这一句来消除突闪了
			}
			else
			{
			 	k++;
			}
		}
	}
	
}


