#include<reg52.h>

unsigned char Tx_Char[] = "shangchao\n";

void Scon_Timer1_Init()		        //SCON和定时器1初始化
{
	TMOD = 0X20;	//设置定时器1工作方式为2
	SCON = 0X50;	//设置串行控制寄存器工作方式为1
	TH1 = 0XFD;	   //当定时器1选择工作方式1时，有两个8位的计数器，一个叫TH1,一个叫TL1
	TL1 = 0XFD;	   //赋给这两个计数器数，然后他们开始进行数数从而达到定时的目的
	TR1 = 1;			 //控制定时器1工作与否
}

void Delay1ms(int i)					 //延时函数
{
    int j;
    for(; i > 0; i--)
    {
         for(j = 0; j < 110; j++);
    }
}

void Byte_Send(unsigned char byte)
{
	//SBUF = byte;
	TI = 0;
	while(TI == 0)		  //这一句非常重要,它使得字符不是一个一个(混乱)的输出，而是一块输出
	{
		SBUF = byte;
	}
	//TI = 0;
}

void String_Send(unsigned char *Tx_Data)
{	  
	while(*Tx_Data != '\0')
	{
		Byte_Send(*Tx_Data++);
	}
}


void main()							 //主函数
{						 
	Scon_Timer1_Init();
	while(1)
	{
		String_Send(Tx_Char);
		Delay1ms(1000);
	}
}



