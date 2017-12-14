
/*lcd1602��غ���*/
#include "inc/lcd1602.h"
#include "inc/ds1302.h"
#include "reg52.h"


/*lcd1602���ų�ʼ��*/
sbit RS = P1^2; 
sbit RW = P1^3;
sbit E =  P1^4;
sbit if_busy = P0^7;

/*��ÿ�ζ�1602����֮ǰ��Ҫ����P0^7λ����æ*/ 
void IF_busy()
{
	P0 = 0xff;
	do				//��״̬
	{
		RS = 0;
		RW = 1;
		E = 0;
		E = 1;	 	
	}while(if_busy == 1);
	E = 0;
} 

/*1602дָ��*/
void write_command_1602(unsigned char command)
{
	IF_busy();
 	RS = 0;
	RW = 0;
	P0 = command;
	E = 1;
	E = 0;
}

/*1602д����*/
void write_data_1602(unsigned char Data)
{
	IF_busy();
	RS = 1;
	RW = 0;
	P0 = Data;
	E = 1;
	E = 0;
}

/*lcd1602��ʼ������*/
void Init_1602()
{
	E = 0;
	write_command_1602(0x38);
	write_command_1602(0x06);
	write_command_1602(0x0c);
	write_command_1602(0x01);
}

/*��1602��д���ַ���*/
void write_string(unsigned char addr_start,unsigned char display)
{

	write_command_1602(addr_start);

	write_data_1602(display);
}

