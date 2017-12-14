#include<reg52.h>

/*对四个独立按键进行IO口设置*/
sbit sw_17 = P2^3;//KEY4
sbit sw_18 = P2^2;//KEY3
sbit sw_19 = P2^1;//KEY2
sbit sw_20 = P2^0;//KEY1
sbit down_flag = P3^2;//按键按下标志位	 并且P3^2是外部中断0的口

/*数码管对应寄存器设置*/
sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;

/*数码管位选和段选表*/
code unsigned char Duan_choose[5] = {0x06,0x5b,0x4f,0x66,0x3f};
code unsigned char Wei_choose[4] =  {0xfe,0xfd,0xfb,0xf7};

unsigned char i;//对应独立按键按下
/*数码管发送数据函数*/
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


/*判断是否有独立按键按下，并且判断是哪个按键被按下*/
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


/*中断初始化函数*/
void Init_interrupt()
{
 	EA = 1;
	EX0 = 1;
	IT0 = 0;//独立式按键的中断请求控制方式是低电平有效，电平触发方式
}


/*主函数*/
void main()
{
	Init_interrupt();
	Send_data(Duan_choose[4],Wei_choose[0]);
	while(1)
	{
		Which_down();
	}
}


/*外部中断0  因为按键按下是一个外部中断*/
void interrupt_0() interrupt 0
{
	Send_data(Duan_choose[i],Wei_choose[i]);
}