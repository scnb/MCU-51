#include<reg52.h>

sbit HC595_SCK = P0^5;
sbit HC595_RCK = P0^6;
sbit HC595_DATA = P0^7;

code unsigned char  Led8_One_Array[2] = {0x01,0x00};  //开发板上有16个LED，其中前8个算作Led_
//_One,后8个算作Led_Two
code unsigned char  Led8_Two_Array[2] = {0x00,0x00};

void Send_595_Data(unsigned char Led8_One,unsigned char Led8_Two)
{
	unsigned char i;
	HC595_RCK = 0;
	for(i=0;i<8;i++)
	{
		HC595_SCK = 0;
		if(0 != (Led8_One&0x80))
		{
			HC595_DATA = 1;
		}
		else
		{
			HC595_DATA = 0;
		}
		HC595_SCK=1;
		Led8_One = Led8_One << 1;
	}
	for(i=0;i<8;i++)
	{
		HC595_SCK = 0;
		if(0 != (Led8_Two&0x80))
		{
			HC595_DATA = 1;
		}
		else
		{
			HC595_DATA = 0;
		}
		HC595_SCK=1;
		Led8_Two = Led8_Two << 1;
	}
	HC595_RCK = 1;
}

void Time0_Init()
{
	TMOD = 0x01;
	TH0 = 0xB8;
	TL0 = 0x00;
	TR0 = 1;
}

void main()
{
	unsigned char k,counter=0;
	Time0_Init();
	while(1)
	{
		if(1 == TF0)
		{
			TF0 = 0;
			TH0 = 0XB8;
			TL0 = 0X00;
			counter++;
		}
		if(100 == counter)
		{
			counter = 0;
			k = !k;
		}
		Send_595_Data(Led8_One_Array[k],Led8_Two_Array[k]);
	}
}