//用定时器0的工作方式来进行led流水灯的实验

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
	TH1 = (8192-4608)/32;			   //2^13=8192，单次最高定时时间是8.89ms,本次定时考虑一次定5ms，所以是分别对2^5=32求余和取模
	TL1 = (8192-4608)%32;
	EA = 1;		  //中断允许总控制位
	ET1 = 1;	   //定时器1中断允许位
	TR1 = 1;	  //定时器1运行控制位
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
	TH1 = (8192-4608)/32;		 //每一次进入中断之后要将计数位重新设置
	TL1 = (8192-4608)%32;
		if(num == 200)			 //这一部分放在while里面或者放在中断里面都是可以的，当中断200次时，说明已经有1s了
		{
			Send_data(Led_One[k],Led_Two[k]);	//在程序的起始定义了k=0
			if(k == 15)
			{
				k = 0;	//当led循环一次之后，重新开始循环
				//Send_data(Led_One[0],Led_Two[15]);很奇怪，在这个程序中不用加这个消除突闪的语句
			}
			else
			{				  
	 			k++;	 
			}
			num = 0;	 //当进入中断200次之后，将num计数清零，重新开始计时1s
		}
		else
		{
 			num++;
		}
}