#include<reg52.h>

sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA =P1^4;

code unsigned char Shu_Duan[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x77,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Shu_Wei[8] =  {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

unsigned char Disbuf[8] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07};

unsigned char Rx_Data = 0;

unsigned char TORH = 0;		   //定义重载值的高四位
unsigned char TORL = 0;		   //定义重载值的低四位

void Timer0_Init(unsigned int ms)				//定时器0初始化
{
	unsigned long tmp;
	tmp = 11059200 / 12;	//定时器计数频率
	tmp =(tmp * ms) / 1000;
	tmp = 65536 - tmp;
	tmp = tmp + 18;         //加18是为了补偿误差
	TORH = (unsigned char)(tmp >> 8);		//将tmp转换成16进制再取出其高四位，作为重载值
	TORL = (unsigned char)tmp;		//将tmp转换成16进制再与上0x0f使其高四位变0，剩下低四位，作为重载值
	TMOD &= 0XF0;          //不给TMOD 赋初值，先与上0xf0，保持高四位不变，让低四位变成0
	TMOD |= 0X01;          //或上0x01，使低四位变成0001，则把T0设置成了工作方式1；
	TH0 = TORH;				//因为在这个函数中同时用到了定时器0和定时器1
	TL0 = TORL;
	TR0 = 1;
	ET0 = 1;			  //定时器0需在中断子程序，所以要使ET0 = 1
}

void Scon_Timer1_Init()		        //SCON和定时器1初始化
{
	TMOD &= 0X0F;
	TMOD |= 0X20;	//设置定时器1工作方式为2
	SCON = 0X50;	//设置串行控制寄存器工作方式为1
	TH1 = 0XFD;	   //当定时器1选择工作方式1时，有两个8位的计数器，一个叫TH1,一个叫TL1
	TL1 = 0XFD;	   //赋给这两个计数器数，然后他们开始进行数数从而达到定时的目的
	//ET1 = 1;
	TR1 = 1;			 //控制定时器1工作与否
	//ES = 1;				//串行中断控制
}


void Send_Data(unsigned char Data_One,unsigned char Data_Two)  //数码管发送数据
{
	unsigned char i;
	HC595_RCK = 0;
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
		if(0 != (Data_One&0x80))
		{
			HC595_DATA = 1;
		}
		else
		{
			HC595_DATA = 0;
		}
		HC595_SCK = 1;
		Data_One = Data_One << 1;
	}
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
		if(0 != (Data_Two&0x80))
		{
			HC595_DATA = 1;
		}
		else
		{
			HC595_DATA = 0;
		}
		HC595_SCK = 1;
		Data_Two = Data_Two << 1;
	}
	HC595_RCK = 1;
}


void Delay1ms(int i)					 //延时函数
{
    int j;
    for(; i > 0; i--)
    {
         for(j = 0; j < 110; j++);
    }
}

void main()							 //主函数
{						 
	unsigned char k = 0;
	EA = 1;							  //开总中断
	ES = 1;							  //开串行中断
	Timer0_Init(1);
	Scon_Timer1_Init();
	
	while(1)
	{
		Disbuf[0] = Shu_Duan[Rx_Data>>4];		 //有疑问	 
		Disbuf[1] = Shu_Duan[Rx_Data&0x0f];
	
		for(k = 0;k < 8;k++)
		{
			Send_Data(Disbuf[k],Shu_Wei[k]);
			Delay1ms(1);
		}
	}
}

void interrupt_Timer0() interrupt 1	  //自动重载计数器的高位和地位
{
	TH0 = TORH;
	TL0 = TORL;
}

void interrupt_Serial() interrupt 4	  //串行口中断函数,判断接收发送与否
{
	if(1 == TI)				//TI是串口发送标志，当其等于1时标志发送数据完毕(校验停止位)
	{
		TI = 0;			   //将TI置0才能继续进行发送数据
		REN = 1;		   //REN是使能串口接收，当REN等于1时，允许接收数据
	}	
	if(1 == RI)			  //RI是串口接收标志，当其等于1时即接收数据完毕(接收停止位)
	{
		RI = 0;			  //当RI置0时才能继续接收数据
		Rx_Data = SBUF;
	 
		SBUF = Rx_Data;	

		REN = 0;
	}
}
