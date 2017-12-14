#include "include/delay.h"

void Delay1ms(int i)
{
	int j;
	for(;i > 0;i--)
	{
		for(j = 0;j < 110;j++);
	}
}