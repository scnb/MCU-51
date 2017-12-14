#include<reg51.h>    //区别于reg52.h，是reg52.h的子集，
//reg51.h包含的是任何一种51单片机都有的sfr，而reg52.h是reg51.h的扩充
//#include<reg52.h>
#include "include/main.h"
#include "include/hc595.h"	 //若果调用的是.c文件是不是也可以呢？
#include "include/delay.h"
#include <intrins.h>


//中断程序  当检测到有按键按下的时候，在激发中断程序

void Interrupt_Led()  interrupt 0 using 0
{
	
	//P2 = 0xff;
	if(P2 != 0xff)
	{
		if(0xf7 == P2)
		{
			Display[0] = 0x01;
		}
		else if(0xfb == P2)
		{
			Display[0] = 0x02;
		}
		else if(0xfd == P2)
		{
			Display[0] = 0x04;
		}
		else if(0xfe == P2)
		{
			Display[0] = 0x08;
		}
	}
	Send_Data(Display[0],0x00);		
}

void Init_Fun()
{
	Send_Data(0x00,0x00);
}


void main(void)
{
	P2 = 0xff;
	EA = 1;
	IT0 = 0;
	EX0 = 1;	
	//Send_Data(0x00,0x00);

	Init_Fun();
	while(1)
	{
		delay1ms(3000);

		delay1ms(3000);
	}
}


