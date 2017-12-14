#include<reg52.h>

/*定义向电脑发送的数据*/
char *s = "shangchao";


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

/*发送数据的函数*/
/*void Send_uart(char string[])
{
	unsigned char i = 0;
 	for(i = 0;i < 9;i++)
	{
	 	SBUF = string[i];
		while(!TI);
		Delay(500);
		TI = 0;
	}
} */

/*指针形式*/
void Send_uart(char *string)
{
	unsigned char i = 0;
 	while(*(string+i) != '\0')
	{
	 	SBUF = string[i];
		while(!TI);
		Delay(1000);// 没有特别得意思，只是想让每个字符之间发送的慢一些，从而不像是整个数据同时发过来的
		i++;
		TI = 0;
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
		TI = 0;
	 	Send_uart(s);
		Delay(1000);
	}
}


void interrupt_4() interrupt 4
{
 	TI = 0;
}