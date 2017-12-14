#include<reg52.h>
sbit HC595_SCK=P0^5;
sbit HC595_RCK=P0^6;
sbit HC595_DATA=P0^7;
void main()
{
	unsigned char i,Led8_One,Led8_Two;
	Led8_One=0x01;
	Led8_Two=0x01;
	HC595_RCK=0;
	while(1)
	{
		//HC595_RCK=0;
	    //HC595_SCK=0;
		for(i=0;i<8;i++)
		{
		    HC595_SCK=0;
			if(0!=(Led8_One&0x80))
			{
				HC595_DATA=1;
			}
			else
			{
				HC595_DATA=0;
			}
			Led8_One=Led8_One<<1;
			HC595_SCK=1;
		}
		for(i=0;i<8;i++)
		{
			HC595_SCK=0;
			if(0!=(Led8_Two&0x80))
			{
				HC595_DATA=1;
			}
			else
			{
				HC595_DATA=0;
			}
			Led8_Two=Led8_Two<<1;
			HC595_SCK=1;
		}
		HC595_RCK=1;
	}
}