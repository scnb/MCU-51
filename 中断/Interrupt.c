#include<reg52.h>

sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;

typedef unsigned char uchar;
typedef unsigned int  uint;

//uchar counter,k;错误的部分
uchar k = 0;
uint counter=0;//同时还给counter覆了初值


code unsigned char Shu_Duan_Xuan[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
code unsigned char Shu_Wei_Xuan[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
unsigned char Led8_Eight[8]={0x00};

void Shu_Ma_Data(unsigned char Shu_Ma_Duan,unsigned char Shu_Ma_Wei)
{
	uchar i;
	HC595_RCK = 0;
	for(i=0;i<8;i++)
	{
		HC595_SCK = 0;
		if(0 != (Shu_Ma_Duan&0x80))
		{
			HC595_DATA = 1;
		}
		else
		{
			HC595_DATA = 0;
		}
		HC595_SCK=1;
	    Shu_Ma_Duan = Shu_Ma_Duan << 1;	   //对16进制的每一位进行操作
	}
	for(i=0;i<8;i++)
	{
		HC595_SCK = 0;
		if( 0 != (Shu_Ma_Wei&0x80))
		{
			HC595_DATA = 1;
		}
		else
		{
			HC595_DATA = 0;
		}
		HC595_SCK = 1;
	    Shu_Ma_Wei = Shu_Ma_Wei << 1;
	}
	 HC595_RCK = 1;
}

void Time1_Init()  //定时器1初始化
{
	TMOD = 0X10;
	TH1 = 0XFC;   //TH1即为定时器1在方式1工作下的16位计数器的高8位 ，0xfc值是通过公式与延时1s算出来的
	TL1 = 0X66;	  //TL1即为定时器1在方式1工作下的16位计数器的低8位
	TR1 = 1;
}

void int_Init()	  //中断初始化
{
	EA = 1;		  //中断总控制
	ET1 = 1;	  //定时器1中断下的IE控制
}

void main()		  //主函数
{
	//Shu_Ma_Data(uchar Shu_Ma_Duan,uchar Shu_Ma_Wei);
	Time1_Init();
	int_Init();
	//interrupt();  //以上为函数初始化
	while(1);//(1)能省略
}


void interrupt_time_1() interrupt 3  //中断程序	，后面的interrupt 3必须写上
{
	//Shu_Ma_Data(uchar,uchar);
	static uint sec = 0;	 //是uint不是uchar，最高是65536
	counter++;	                //该函数应该是嵌套在一个while循环里，counter不断的加加
	TH1 = 0XFC;				   //每次循环开始之前要对16位计数器重新赋值
	TL1 = 0X66;
	if(1000 == counter)
	{
		counter = 0;
		sec++;
		Led8_Eight[7] = sec%10;
		Led8_Eight[6] = sec/10%10;
		Led8_Eight[5] = sec/100%10;
		Led8_Eight[4] = sec/1000%10;
		Led8_Eight[3] = sec/10000%10;
	}
	if(k < 7)
	{
		k++;
	}		
	else
	{
		k = 0;
	}
	Shu_Ma_Data(Shu_Duan_Xuan[Led8_Eight[k]],Shu_Wei_Xuan[k]);
}