#include<reg52.h>

/*数码管对应寄存器设置*/
sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;

/*键盘对应IO口设置*/

unsigned char temp;//用来存储P2口的状态
unsigned char value = 5;//用来记录是哪一个键盘被按下

/*数码管的段选表和位选表*/
code unsigned char Duan_choose[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Wei_choose[8] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

/*数码管数据发送函数*/
void Send_data(unsigned char duan,unsigned char wei)
{
	unsigned char i;	   //还是用hc595移位寄存器来进行操作，参照着数码管的原理与时序来写程序
 	HC595_RCK = 0;
	for(i = 0;i < 8;i++)
	{
	 	HC595_SCK = 0;
		if(0 != (duan&0x80))
		{
		 	HC595_DATA = 1;
		}
		else
		{
		 	HC595_DATA = 0;
		}
		HC595_SCK = 1;
		duan = duan << 1;
	}
	for(i = 0;i < 8;i++)
	{
	 	HC595_SCK = 0;
		if(0 != (wei&0x80))
		{
		 	HC595_DATA = 1;
		}
		else 
		{
		 	HC595_DATA = 0;
		}
		HC595_SCK = 1;
		wei = wei << 1;
	}
	HC595_RCK = 1;
}

/*纯延时程序*/
void Delay(unsigned int i)
{
 	unsigned char j;
	for(;i > 0;i--)
	{
	 	for(j = 0;j < 110;j++)
		{}
	}
}

/*矩阵键盘扫描函数*/
void matrix_button_scan()
{
	/*第一行*/
	P2 = 0xfe;
	temp = P2;
	temp = temp & 0x0f;
	if(0x0f != temp)
	{
		Delay(10);
		temp = P2;
		temp = temp & 0x0f;
		if(0x0f != temp)
		{
			temp = P2;
			switch(temp)
			{
				case 0xee:value = 0;break;
			 	case 0xde:value = 1;break;
				case 0xbe:value = 2;break;
				case 0x7e:value = 3;break;
			}
		}
	}
	/*第  二  行*/
	P2 = 0xfd;
	temp = P2;
	temp = temp & 0x0f;
	if(0x0f != temp)
	{
		Delay(10);
		temp = P2;
		temp = temp & 0x0f;
		if(0x0f != temp)
		{
			temp = P2;
			switch(temp)
			{
				case 0xed:value = 4;break;
			 	case 0xdd:value = 5;break;
				case 0xbd:value = 6;break;
				case 0x7d:value = 7;break;
			}
		}
	}
	/*第  三  行*/
	P2 = 0xfb;
	temp = P2;
	temp = temp & 0x0f;
	if(0x0f != temp)
	{
		Delay(10);
		temp = P2;
		temp = temp & 0x0f;
		if(0x0f != temp)
		{
			temp = P2;
			switch(temp)
			{
				case 0xeb:value = 8;break;
			 	case 0xdb:value = 9;break;
				case 0xbb:value = 10;break;
				case 0x7b:value = 11;break;
			}
		}
	}
	/*第  四  行*/
	P2 = 0xf7;
	temp = P2;
	temp = temp & 0x0f;
	if(0x0f != temp)
	{
		Delay(10);
		temp = P2;
		temp = temp & 0x0f;
		if(0x0f != temp)
		{
			temp = P2;
			switch(temp)
			{
				case 0xe7:value = 12;break;
			 	case 0xd7:value = 13;break;
				case 0xb7:value = 14;break;
				case 0x77:value = 15;break;
			}
		}
	}
	Send_data(Duan_choose[value],Wei_choose[0]);
}



/*主函数*/
void main()
{
 	while(1)
	{
		matrix_button_scan();
	}
}