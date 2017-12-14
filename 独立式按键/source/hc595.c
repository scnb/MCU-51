#include "reg51.h"
#include "include/hc595.h"
//Led部分的寄存器与I/O口接口方式
sbit HC595_SCK = P0^5;
sbit HC595_RCK = P0^6;
sbit HC595_DATA = P0^7;

void Send_Data(unsigned int Data_One,unsigned int Data_Two)
{
	unsigned int i;
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
		Data_One = Data_One << 1;		//  <<是左移运算符,所有的为向左移动，溢出的删去，右边补0
	}
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
		if(0 != (Data_Two&0x80))    //&按位与，同真为真，有一个是假则为假
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
