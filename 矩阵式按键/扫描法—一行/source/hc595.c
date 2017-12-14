#include "reg51.h"
#include "include/hc595.h"

void Send_Data(unsigned char Data_One,unsigned char Data_Two)
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
		Data_One = Data_One << 1;
	}
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
		if(0 != (Data_Two&0x80))
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
