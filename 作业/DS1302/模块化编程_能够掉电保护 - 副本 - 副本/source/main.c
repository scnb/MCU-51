#include<reg52.h>
#include<INTRINS.h>
#include "inc/lcd1602.h"
#include "inc/ds1302.h"
#include "inc/i2ceeprom.h"
#include "inc/hc595.h"


/*定义ds1302  RAM中读、写地址起始位*/
#define write_addr 0x80;
#define read_addr  0x81;

unsigned char time[] = {55,16,0,30,11,4,16};//定义ds1302初始化的时间

unsigned char second = 59,minute = 59,hour = 11,day = 25,mouth = 11,week = 3,year = 16;

extern unsigned char addr = 0x01;//器件起始地址

unsigned int num = 1;

unsigned char first_start_flag;

code unsigned char Duan_choose[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Wei_choose[16] =  {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

void Delay(unsigned int i)
{
 	unsigned char j;
	for(;i > 0;i--)
	{
		for(j = 0;j<110;j++)
		{}
	}
}



void main()
{
	unsigned char i;
 	Init_1602();
	InitI2C();
	
	addr = 0x01;
	first_start_flag = read_ds1302(0xc1);//读取ds1302是否是初次初始化,0位初次启动，1为非初次启动

	if(!first_start_flag)
	{
	 	first_start_flag = 1;
		read_time_ds1302(&time);
		set_time_ds1302(&time);
		
		
	}
	while(1)
	{
		read_time_ds1302(&time);
		//第一行显示  年-月-日
		write_string(0x80,'D');
		write_string(0x81,'A');
		write_string(0x82,'T');
		write_string(0x83,'E');											
		write_string(0x85,time[6]/10+'0');
		write_string(0x86,time[6]%10+'0');
		write_string(0x87,'-');
		write_string(0x88,time[4]/10+'0');
		write_string(0x89,time[4]%10+'0');
		write_string(0x8a,'-');
		write_string(0x8b,time[3]/10+'0');
		write_string(0x8c,time[3]%10+'0');
		//第二行显示   时-分-秒
		write_string(0xc0,'T');
		write_string(0xc1,'I');
		write_string(0xc2,'M');
		write_string(0xc3,'E');
		write_string(0xc5,time[2]/10+'0');
		write_string(0xc6,time[2]%10+'0');
		write_string(0xc7,':');
		write_string(0xc8,time[1]/10+'0');
		write_string(0xc9,time[1]%10+'0');
		write_string(0xca,':');
		write_string(0xcb,time[0]/10+'0');
		write_string(0xcc,time[0]%10+'0');
	}
}