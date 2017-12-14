#include<reg52.h>
//定义引脚
sbit HC595_RCK=P1^0;
sbit HC595_SCK=P1^1;
sbit HC595_DATA=P1^4;  
code unsigned char Duan_chose[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Wei_chose[16]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

//code unsigned char Duan_chose2=0x06;
//code unsigned char Wei_chose2=0xfe;


//定义延时函数,注意这个函数中的j的值不再是110，经过调试之后发现j取200-255能使数字稍微缓慢变化
void Delay_ms(unsigned char i)
{
	unsigned int j;					//unsigned char 范围是0-255...坑爹啊
	for(;i>0;i--)
	{
		for(j=0;j<255;j++)
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
	HC595_RCK=0;
	
//	HC595_SCK=0;
//	HC595_DATA=0;
}
//主函数
void main()
{
	//Shu_Ma_Guan_Data(Duan_chose1,Wei_chose1);
   //	Delay_ms(1);
	while(1)
	{
		unsigned char k;
		for(k=0;k<15;k++)
		{
			Shu_Ma_Guan_Data(Duan_chose[k],Wei_chose[k]);
			Delay_ms(500);
		}
	
	} 
}