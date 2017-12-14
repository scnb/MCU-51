#include<reg52.h>
#include<INTRINS.h>
sbit RS = P1^2; 
sbit RW = P1^3;
sbit E =  P1^4;
sbit if_busy = P0^7;
code unsigned char arry[9] = "shangchao";
code unsigned char num[9] = {'1','2','3','4','5','6','7','8','9'};
void IF_busy()
{
	P0 = 0xff;
	do				//¶Á×´Ì¬
	{
		RS = 0;
		RW = 1;
		E = 0;
		E = 1;	 	
	}while(if_busy == 1);
	E = 0;
} 

void Delay5us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 11;
	while (--i);
}

void write_command(unsigned char command)//Ð´Ö¸Áî£¬Éè¶¨¼Ä´æÆ÷×´Ì¬
{
	IF_busy();
 	RS = 0;
	RW = 0;
	P0 = command;
	//Delay5us();
	E = 1;
	//Delay5us();
	E = 0;
}

void write_data(unsigned char Data)
{
	IF_busy();
	RS = 1;
	RW = 0;
	P0 = Data;
//	Delay5us();
	E = 1;
//	Delay5us();
	E = 0;
}

void Init_1602()
{
	E = 0;
	write_command(0x38);
	write_command(0x06);
	write_command(0x0f);
	write_command(0x01);
}

void write_string(unsigned char addr_start, unsigned char table[])
{
	unsigned char i;
	write_command(addr_start);
	for(i = 0;i < 9;i++)
	{
		write_data(table[i]);
	}
}


void main()
{
 	Init_1602();
	write_string(0x80,arry);
	write_string(0x80+0x40,num);
	while(1);
}