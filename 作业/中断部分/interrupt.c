#include<reg52.h>


//unsigned char k = 0;
//unsigned char n = 0;
unsigned char num = 0;
unsigned char counter = 0;

sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;

code unsigned char Duan_choose[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
code unsigned char Wei_choose[8]  =  {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

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


 void Timer0Init(void)		//50毫秒@11.0592MHz
{
//	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x4C;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA = 1;
	ET0 = 1;
}

void Delay(int i)
{
 	unsigned char j;
	for(;i >=0;i--)
	{
	 	for(j = 0;j <= 110;j++)
		{}
	}
}



void main()
{
	Timer0Init();
	while(1)
	{
		Send_data(Duan_choose[num/10],Wei_choose[6]);  //真正实现了动态扫描，可以用Delay函数来进行延时观察效果，减少视觉暂留效应
		Send_data(Duan_choose[num%10],Wei_choose[7]);  //因为这两句的间隔非常短，所以形成了动态扫描，视觉暂留效应
		if(20 == counter)
		{
			num++;
			counter = 0;
		}
		if(61 == num)
		{
			num = 0;
		}
	}
}


void interrupt_1() interrupt 1
{
	TF0 = 0;
   	TL0 = 0x00;		//设置定时初值
	TH0 = 0x4C;		//设置定时初值
	counter++;
}