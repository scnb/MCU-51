#include<reg52.h>
//定义引脚
sbit HC595_RCK=P1^0;
sbit HC595_SCK=P1^1;
sbit HC595_DATA=P1^4;  
code unsigned char Duan_chose[8]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07};
code unsigned char Wei_chose[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
//code unsigned char Duan_chose=0x5e;//
//code unsigned char Wei_chose=0xfe;
//定义延时函数
void Delay_ms(unsigned char i)
{
	unsigned char j;
	for(;i>0;i--)
	{
		for(j=0;j<110;j++)
		{
		}	
	}
}
//定义数码管数据传送函数
void Shu_Ma_Guan_Data(unsigned char Shu_Ma_Duan,unsigned char Shu_Ma_Wei)
{
	unsigned char i;
	HC595_RCK=0;
	for(i=0;i<8;i++)
	{
		HC595_SCK=0;
		if(0!=(Shu_Ma_Duan&0x80))
		{
			HC595_DATA=1;
		}
		else
		{
			HC595_DATA=0;
		}
		Shu_Ma_Duan=Shu_Ma_Duan<<1;
		HC595_SCK=1;
	}
	for(i=0;i<8;i++)
	{
		HC595_SCK=0;
		if(0!=(Shu_Ma_Wei&0x80))
		{
			HC595_DATA=1;
		}
		else
		{
			HC595_DATA=0;
		}
		Shu_Ma_Wei=Shu_Ma_Wei<<1;
		HC595_SCK=1;
	}
	HC595_RCK=1;
}
//主函数
void main()
{
	//Shu_Ma_Guan_Data(Duan_chose,Wei_chose);
	//Delay_ms(1);
	while(1)
	{
		unsigned char k;
		for(k=0;k<8;k++)
		{
			Shu_Ma_Guan_Data(Duan_chose[k],Wei_chose[k]);
			Delay_ms(1000);
		}
	
	} 
}