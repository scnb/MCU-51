#include<reg52.h>

sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;

code unsigned char Duan_choose[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Wei_choose[16] =  {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

void Send_data(unsigned char duan,unsigned char wei)
{
	unsigned char i;	   //还是用hc595移位寄存器来进行操作，参照着数码管的原理与时序来写程序
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

void Delay(unsigned int i)
{
 	unsigned char j;
	for(;i > 0;i--)
	{
	 	for(j = 0;j < 110;j++)
		{}
	}
}

void main()
{
 	unsigned char k;
	for(k = 0;k <= 15;k++)	  //原来错误的写成了k<15,导致最后的一个f显示不出来
	{
	 	Send_data(Duan_choose[k],Wei_choose[k]);
		Delay(2000);
	}
}