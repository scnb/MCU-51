//��ʱ��1������ʽ3

#include<reg52.h>

unsigned char k = 0;
unsigned int num = 0;//һ��Ҫע�⣬num�ļ�����Χ��ԭ����unsigned char num��num���ֻ�ܼ�����255

sbit HC595_SCK = P0^5;
sbit HC595_RCK = P0^6;
sbit HC595_DATA = P0^7;

code unsigned char Led_One[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
code unsigned char Led_Two[16] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*void Init_Timer0()
{
 	TMOD = 0X03;
	TL0 = 0xa4;                //��ʽ3������8λ��������ע�������������Ƿֿ������ģ�TL0��T0���жϣ�TH0��T1���ж�
	EA = 1;					   //2^8 = 256   ��0��255����һ�����ļ�ʱʱ���ǣ���1/11059200	��*12*256 = 277.78us �� 0.28ms
	ET0 = 1;
	TR0 = 1; 
	TF0 = 0;
}  */

void Timer0Init(void)		//100΢��@11.0592MHz
{
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x03;		//���ö�ʱ��ģʽ
	TL0 = 71;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	EA = 1;
	ET0 = 1;
}


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



void main()
{
 	//Init_Timer0();
	Timer0Init();
	Send_data(Led_One[0],Led_Two[8]);
	while(1)
	{
	   if(1== TF0)
	   {
			TF0 = 0;
			TL0 = 71;
			num++;
	   }
	   if(5000 == num)
	   {
			Send_data(Led_One[k], Led_Two[k]);
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

/*void interrupt_1() interrupt 1
{
 	TL0 = 71;
	TF0 = 0;
	if(num == 5000)
	{
	 	Send_data(Led_One[k], Led_Two[k]);
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
	else
	{
 		num++;
	}
}*/