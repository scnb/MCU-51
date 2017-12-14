
/*ds1302��غ���*/
#include "inc/ds1302.h"
#include "reg52.h"
#include "inc/lcd1602.h"

/*��ds1302�����Ž�������*/
sbit RST = P3^7;
sbit SCL = P3^5;
sbit SDA = P3^6;

#define write_addr 0x80;
#define read_addr  0x81;

/*��λds1302    ÿ�η������ݴ���ǰ����Ҫ��λ1302*/
void reset_1302()
{
 	RST = 0;
	SCL = 0;
	RST = 1;
}

/*д����*/
void write_data_1302(unsigned char dat)
{
 	unsigned char i;
	SCL = 0;
	for(i = 0;i < 8;i++)
	{
	 	SDA = dat&0x01;
		SCL = 1;
		dat >>= 1;
		SCL = 0;//����һ��ŵ�dat�������棬��������ʱ������,�ӳ���һ��ָ���ʱ��
	}
}

/*������*/
unsigned char read_data_1302()
{
 	unsigned char dat = 0;
	unsigned char i;
	for(i = 0;i < 8;i++)
	{
		SCL = 0;
	 	dat >>= 1;
		if(SDA)
		{
		 	dat |= 0x80;
		}
		SCL = 1;
	}
	return dat;
}

/*������д��ds1302*/
void write_ds1302(unsigned char addr,unsigned char dat)
{
 	reset_1302();
	RST = 1;
	write_data_1302(addr);
	write_data_1302(dat);
	SDA = 0;   //�ر������ߣ���ֹ���ݲ�С�ı��޸�
	RST = 0;  //��ֹds1302��λ�������ݶ�ʧ��
}

/*��ds1302�ж�ȡ����*/
unsigned char read_ds1302(unsigned char addr)
{
	unsigned char temp = 0;
 	reset_1302();
	RST = 1;
	write_data_1302(addr);
	temp = read_data_1302();
	SDA = 0;
	RST = 0;
	return temp;
}


/*д��������*/
void write_protect()
{
	reset_1302();
	RST = 1;
	write_ds1302(0x8e,0x80);
	SDA = 0;
	RST = 0;
}
																	 
/*���д����*/
void clear_write_protect()
{
 	reset_1302();
	RST = 1;
	write_ds1302(0x8e,0);
	SDA = 0;
	RST = 0;
}



/*�趨ds1302ʱ��*/
void set_time_ds1302(unsigned char *timedata)
{
 	unsigned char i;
	unsigned char tmp;
	for(i = 0;i < 7;i++)
	{
		tmp = timedata[i]/10;			   //����������佫ʮ���Ƶ���ת������BCD����ʽ����Ӧ��ʮ������
		timedata[i] = timedata[i]%10;
		timedata[i] = timedata[i] + tmp*16;	 	
	}
	clear_write_protect();
	tmp = write_addr;
	for(i = 0;i < 7;i++)  //��ds1302�д��������е����ݣ���������ʱ����
	{
	 	write_ds1302(tmp,timedata[i]);
		tmp += 2;//�����ֽڴ�һ��  //��Ϊ0x80��Ӧ�������ڼĴ����еĴ洢��ַ��0x82�Ƿֶ�Ӧ�Ĵ洢��ַ����������������
	}
	write_protect();
}

/*��ȡds1302�е�ʱ��*/
void read_time_ds1302(unsigned char *timedata)
{
 	unsigned char i;
	unsigned char tmp;
	tmp = read_addr;
	for(i = 0;i < 7;i++)
	{
	 	timedata[i] = (read_ds1302(tmp)/16*10+read_ds1302(tmp)%16);
		tmp += 2;
	}
}