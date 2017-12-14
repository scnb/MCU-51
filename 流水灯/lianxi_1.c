#include<reg52.h>
sbit HC595_SCK = P0^5;
sbit HC595_RCK = P0^6;
sbit HC595_DATA = P0^7;
code unsigned char Led8_One_Array[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
code unsigned char Led8_Two_Array[16]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
void Send_Data(unsigned char Led8_One,unsigned char Led8_Two)
{
	unsigned char i;
	HC595_RCK=0;
//	HC595_SCK=0;   这个地方没有
	for(i=0;i<8;i++)
	{
		HC595_SCK=0;//上面放的这一句应该写在这里，在每个for循环开始之前相当于初始化
		if(0!=(Led8_One&0x80))
		{
			HC595_DATA=1;
		}
		else
		{
			HC595_DATA=0;
		}
		Led8_One=Led8_One<<1;//这一句也缺了
		HC595_SCK=1;
	}	
	for(i=0;i<8;i++)
	{
		HC595_SCK=0;//少了这个初始化
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
void delay_ms(int i)
{
	//unsigned char j;	 用unsigned char比用int变化得快
	int j;//正常写法是这样的
	for(i=i;i>0;i--)  //i=i可以省略
	{
		for(j=0;j<110;j++)
		{
			
		}
	}
}
void main()
{
	//unsigned char i,Led8_One=0x01,Led8_Two=0x00;
	unsigned char k;
	while(1)
	{
		for(k=0;k<16;k++)
		{
			Send_Data(Led8_One_Array[k],Led8_Two_Array[k]);
			delay_ms(1000);
		}
	}	
		
}