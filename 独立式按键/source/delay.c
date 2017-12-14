#include "include/delay.h"

void delay1ms(int i)						   //原来把延时函数写错了
{											   //把i和j的循环搞反了
	int j;
	for(;i > 0;i--)
	{
		for(j = 0;j < 110;j++);
	}
}