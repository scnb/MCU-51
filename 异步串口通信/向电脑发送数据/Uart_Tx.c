#include<reg52.h>
/*
sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;  //目前这个程序还用不着这些定义

code unsigned char Shu_Duan_Chose[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Shu_Wei_Chose[16]  = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
code unsigned char Led8_One[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char Led8_Two[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  */
unsigned char Given_Data = 0x55;

void Serial_Timer1_Init()
{
	TMOD = 0x20;		  //定时器1采用工作方式2（从0开始），溢出能够自动重装
	SCON = 0X50;          //选择串行控制寄存器的工作方式，选择为工作模式1,十六进制为0B0101 0000，即SM0 = 0，SM1 = 1,SM2 = 0,REN = 1，工作模式1位8位寄存器，波特率由定时器的溢出时间决定
	TH1 = 0xfd;          // 256 - 11059200/(12*32*9600)	，因为工作方式2采用8位计数器，所以2^8=256，32是单片机的采样率，12是一个指令周期，是时钟周期的12倍
	TL1 = 0XFD;
	TR1 = 1;			 //TR1 = 1使得定时器1可以工作
	ES = 1;				  //串行总中断开关	 指的是串口的中断吗
	//ET1 = 1;			 //定时器1的中断开关
	EA = 1;            //中断允许总控制位
}

/*void Uart_Tx_Init()							//uart
{
	SCON = 0x50;
	PCON = 0X80;
	REN = 1;
}  */                  //这个函数里的内容可以直接写在上面那个函数里，但是我想分开写也是可以的？要试一试


void Delay1ms(int i)
{
	int j;
	for(;i > 0;i--)
	{
		for(j = 0;j < 110;j++);
	}
}

void Delay()
{
	unsigned char Num = 20000;
	while(Num--)
	{
	}
} 



void Send_Data(unsigned char Data)
{
	SBUF = Data;		
}



void main()
{
	//Send_Data(unsigned char Data);
	/*TMOD = 0X20;
	SCON = 0X50;
	TH1 = TL1 = 0xfd;                                 //256 - 11059200/(12*32*9600)
	EA = 1;
	ES = 1;
	TR1 = 1;
	ET1 = 1;
	Uart_Tx_Init();	 */     //这些都写在初始化函数里了，应该试一试单独写在main函数里可以吗？
	//Delay1ms(int i);
	//Delay();
	
	
	Serial_Timer1_Init();
	
	while(1)
	{
		//SBUF = 0X55;	   //要单独写个数据发送函数，在while里一直执行
		Send_Data(Given_Data++);	  //每循环一次之后加1
		Delay1ms(1000);
		//Delay();
	}
}


void interrupt_Uart_Tx() interrupt 4	   //判断是否发送数据的中断，若发送数据则清零才能重新发送
{
	//EA = 1;
	//ES = 1;
	if(1 == TI)		   //TI 检测是否已发送数据，若已发送，则TI会被硬件置1，
						//所以要用软件置0才能重新接着发送
	{
		TI = 0;

	}
	if(1 == RI)
	{
		RI = 0;
	}
}

/*void interrupt_SendData() interrupt 3		//根本就不需要的函数，应该写一个发送数据
											的函数而不是在中断程序里发送函数
											写中断程序就是为了检测是否已发送数据，若发送，则把TI（发送标志位清零）
											这样才能继续发送数据
											
{
	EA = 1;
	ES = 1;
	SBUF = 0x55;
} */