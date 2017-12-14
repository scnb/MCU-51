
/*lcd1602相关函数*/
#include "inc/lcd1602.h"
#include "inc/ds1302.h"
#include "reg52.h"


/*lcd1602引脚初始化*/
sbit RS = P1^2; 
sbit RW = P1^3;
sbit E =  P1^4;
sbit if_busy = P0^7;

/*在每次对1602操作之前都要进行P0^7位的判忙*/ 
void IF_busy()
{
	P0 = 0xff;
	do				//读状态
	{
		RS = 0;
		RW = 1;
		E = 0;
		E = 1;	 	
	}while(if_busy == 1);
	E = 0;
} 

/*1602写指令*/
void write_command_1602(unsigned char command)
{
	IF_busy();
 	RS = 0;
	RW = 0;
	P0 = command;
	E = 1;
	E = 0;
}

/*1602写数据*/
void write_data_1602(unsigned char Data)
{
	IF_busy();
	RS = 1;
	RW = 0;
	P0 = Data;
	E = 1;
	E = 0;
}

/*lcd1602初始化函数*/
void Init_1602()
{
	E = 0;
	write_command_1602(0x38);
	write_command_1602(0x06);
	write_command_1602(0x0c);
	write_command_1602(0x01);
}

/*向1602中写入字符串*/
void write_string(unsigned char addr_start,unsigned char display)
{

	write_command_1602(addr_start);

	write_data_1602(display);
}

