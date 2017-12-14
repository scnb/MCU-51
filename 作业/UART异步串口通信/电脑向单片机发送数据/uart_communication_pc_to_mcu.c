 #include<reg52.h>

/*定义从电脑接收的数据存储变量*/
unsigned char receive_data;//只接收数字，然后在数码管上显示
unsigned char receive_flag = 0;//接收成功标志位

void Delay(int i)
{
 	unsigned char j;
	for(;i>=0;i--)
	{
	 	for(j = 0;j < 110;j++)
		{
		}
	}
}

sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;

code unsigned char Duan_choose[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Wei_choose[8] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};


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



/*从电脑接收数据的函数*/
void Receive_fun()
{
	if(1 == receive_flag) //在while中不停的检测receive_flag是否为1,当为1的时候说明已经进入过中断，并且进入中断的条件是SBUF已经接收到数据
	{
	 	receive_flag = 0;
		Send_data(Duan_choose[receive_data],Wei_choose[0]);
	} 	
}




/*定时器初始化  中断初始化    串口控制寄存器SCON初始化*/
void UartInit(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
//	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
//	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA = 1;
	ES = 1;
}




void main()
{
 	UartInit();
	while(1)
	{
		Receive_fun();
	}
}


void interrupt_4() interrupt 4
{
	Send_data(Duan_choose[0],Wei_choose[0]);
 	RI = 0;
	receive_data = SBUF;
	//Send_data(Duan_choose[receive_data],Wei_choose[0]);
	receive_flag = 1;
}