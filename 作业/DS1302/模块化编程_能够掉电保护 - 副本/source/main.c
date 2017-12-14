#include<reg52.h>
#include<INTRINS.h>
#include "inc/lcd1602.h"
#include "inc/ds1302.h"
#include "inc/18b20.h"

/*����ds1302  RAM�ж���д��ַ��ʼλ*/
#define write_addr 0x80;
#define read_addr  0x81;

unsigned char time[] = {30,5,22,25,1,4,17};//����ds1302��ʼ����ʱ��

unsigned char second = 59,minute = 59,hour = 11,day = 25,mouth = 11,week = 3,year = 16;

extern unsigned char addr = 0x01;//������ʼ��ַ

unsigned int num = 1;

unsigned char first_start_flag;

unsigned int temp;

unsigned char shiwei;
unsigned char gewei;
unsigned char xiaoshuwei;



void main()
{
//	unsigned char i;
 	Init_1602();
	
	addr = 0x01;
	first_start_flag = read_ds1302(0xc1);//��ȡds1302�Ƿ��ǳ��γ�ʼ��,0λ����������1Ϊ�ǳ�������

	if(!first_start_flag)
	{
	 	first_start_flag = 1;
		read_time_ds1302(&time);
		set_time_ds1302(&time);
		
		
	}
	while(1)
	{
		read_time_ds1302(&time);

		ds18b20();
		temp = get_temperature();
		shiwei = int_to_shiwei(temp);
		gewei = int_to_gewei(temp);
		xiaoshuwei = int_to_xiaoshuwei(temp);	  
		//��һ����ʾ  ��-��-��
		//write_string(0x80,'D');
		//write_string(0x81,'A');
		//write_string(0x82,'T');
		//write_string(0x83,'E');											
		write_string(0x80,time[6]/10+'0');
		write_string(0x81,time[6]%10+'0');
		write_string(0x82,'-');
		write_string(0x83,time[4]/10+'0');
		write_string(0x84,time[4]%10+'0');
		write_string(0x85,'-');
		write_string(0x86,time[3]/10+'0');
		write_string(0x87,time[3]%10+'0');
		//�ڶ�����ʾ   ʱ-��-��
		//write_string(0xc0,'T');
		//write_string(0xc1,'I');
		//write_string(0xc2,'M');
		//write_string(0xc3,'E');
		write_string(0xc0,time[2]/10+'0');
		write_string(0xc1,time[2]%10+'0');
		write_string(0xc2,':');
		write_string(0xc3,time[1]/10+'0');
		write_string(0xc4,time[1]%10+'0');
		write_string(0xc5,':');
		write_string(0xc6,time[0]/10+'0');
		write_string(0xc7,time[0]%10+'0');
		
		write_string(0x8a,shiwei+'0');
		write_string(0x8b,gewei+'0');
		write_string(0x8c,'.');
		write_string(0x8d,xiaoshuwei+'0');
		write_string(0x8e,0xdf);
		write_string(0x8f,'C');
	}
}