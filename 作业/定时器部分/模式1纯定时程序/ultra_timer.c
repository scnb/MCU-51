//该程序目前存在的问题是，上电后需要等待大约3秒钟才开始流水灯
#include<reg52.h>

unsigned char num = 0;
unsigned char k = 0;
sbit HC595_SCK = P0^5;
sbit HC595_RCK = P0^6;
sbit HC595_DATA = P0^7;
//对应的led序号是：
code unsigned char Led_One[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
code unsigned char Led_Two[16] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//依次对应的led序号是：

void Send_data(unsigned char Led8_one,unsigned char Led8_two)
{
	unsigned char i;
 	HC595_RCK = 0;
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
	 	if(0 != (Led8_one&0x80))
		{
		 	HC595_DATA = 1;
		}
		else
		{
		 	HC595_DATA = 0;
		}
		Led8_one = Led8_one << 1;
		HC595_SCK = 1;
	}
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
	 	if(0 != (Led8_two&0x80))
		{
		 	HC595_DATA = 1;
		}
		else
		{
		 	HC595_DATA = 0;
		}
		Led8_two = Led8_two << 1;
		HC595_SCK = 1;
	}
	HC595_RCK = 1;
}

		
void Timer1_Init()
{
 	TMOD = 0X10;
	TH1 = 0x4c;
	TL1 = 0x00;
	EA = 1;
	ET1 = 1;
	TR1 = 1;
}

void  main()
{
 	Timer1_Init();
	while(1)
	{
		if(1 == TF1)
		{
		  TF1 = 0;
		  TH1 = 0x4c;
		  TL1 = 0x00;
		  num++;
		}
		if(20 == num)
		{
		 	Send_data(Led_One[k],Led_Two[k]);
			if(k == 15)
			{
			 	k = 0;
			}
			else
			{
			 	k++;
			}
			num = 0;
		}
	}
}

/*void interrupt_T1()  interrupt 3
{
	TH1 = 0x4c;
	TL1 = 0x00;
	if(num == 20)
	{
	 	Send_data(Led_One[k],Led_Two[k]);
		if(k == 15)
		{
	 		k = 0;
			Send_data(Led_One[0],Led_Two[15]);//加入这一句是为了消除从右到左16个灯亮完之后有几个等瞬时闪烁这种现象
		}
		else
		{
	 		k++;
		}
		num = 0;
	}
	else
	{
	 	num++;
	}
}

*/