#include "include/delay.h"

void delay1ms(int i)						   //ԭ������ʱ����д����
{											   //��i��j��ѭ���㷴��
	int j;
	for(;i > 0;i--)
	{
		for(j = 0;j < 110;j++);
	}
}