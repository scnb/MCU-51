#include<reg52.h>



/*键盘对应IO口设置*/
unsigned char temp;//用来存储P2口的状态
static unsigned char value = 5;//用来记录是哪一个键盘被按下,同时也是要向电脑发送的数字
static unsigned char pre_value;


/*纯延时程序*/
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




/*矩阵键盘扫描函数*/
void matrix_button_scan_uart_send()
{
	/*第一行*/
	P2 = 0xfe;
	temp = P2;
	temp = temp & 0x0f;
	if(0x0f != temp)
	{
		Delay(10);
		temp = P2;
		temp = temp & 0x0f;
		if(0x0f != temp)
		{
			temp = P2;
			switch(temp)
			{
				case 0xee:value = 0;break;
			 	case 0xde:value = 1;break;
				case 0xbe:value = 2;break;
				case 0x7e:value = 3;break;
			}
		}
	}
	/*第  二  行*/
	P2 = 0xfd;
	temp = P2;
	temp = temp & 0x0f;
	if(0x0f != temp)
	{
		Delay(10);
		temp = P2;
		temp = temp & 0x0f;
		if(0x0f != temp)
		{
			temp = P2;
			switch(temp)
			{
				case 0xed:value = 4;break;
			 	case 0xdd:value = 5;break;
				case 0xbd:value = 6;break;
				case 0x7d:value = 7;break;
			}
		}
	}
	/*第  三  行*/
	P2 = 0xfb;
	temp = P2;
	temp = temp & 0x0f;
	if(0x0f != temp)
	{
		Delay(10);
		temp = P2;
		temp = temp & 0x0f;
		if(0x0f != temp)
		{
			temp = P2;
			switch(temp)
			{
				case 0xeb:value = 8;break;
			 	case 0xdb:value = 9;break;
				case 0xbb:value = 10;break;
				case 0x7b:value = 11;break;
			}
		}
	}
	/*第  四  行*/
	P2 = 0xf7;
	temp = P2;
	temp = temp & 0x0f;
	if(0x0f != temp)
	{
		Delay(10);
		temp = P2;
		temp = temp & 0x0f;
		if(0x0f != temp)
		{
			temp = P2;
			switch(temp)
			{
				case 0xe7:value = 12;break;
			 	case 0xd7:value = 13;break;
				case 0xb7:value = 14;break;
				case 0x77:value = 15;break;
			}
		}
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
		pre_value = value;
		while(1)
		{
	 		matrix_button_scan_uart_send();
			if(pre_value != value)
			{
				break;
			}
		}
		if(pre_value != value)//2016/11/21凌晨一点26分，终于把这个程序搞定了，要想按一次按键，发送一个数据，就不能用中断来发送数据，而只能用if判断，value的值是否改变了，目前不能使用中断的原因未知
		{
		 	SBUF = value;	  //注意发送数据的时候，用中断是不合适的，因为TI只有在数据已经存入SBUF后才会被硬件置1，而自己使用中断的话只能用软件设置TI=1才能进入中断，这样是不合适的
			while(!TI);   //在接收数据的时候，使用中断是顺利合理的，因为，外部数据发送过来，被存到SBUF中后，RI被置1，进入中断，通过设立数据接收标志位，然后在其他的函数中通过判断数据接收标志位的值来决定是否执行相应的程序
			TI = 0;
		}
	}
}

/*void interrupt_4() interrupt 4
{
 	SBUF = value;
	while(0 == TI);
}
*/