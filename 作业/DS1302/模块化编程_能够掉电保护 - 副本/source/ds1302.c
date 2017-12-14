
/*ds1302相关函数*/
#include "inc/ds1302.h"
#include "reg52.h"
#include "inc/lcd1602.h"

/*对ds1302的引脚进行设置*/
sbit RST = P3^7;
sbit SCL = P3^5;
sbit SDA = P3^6;

#define write_addr 0x80;
#define read_addr  0x81;

/*复位ds1302    每次发起数据传输前，需要复位1302*/
void reset_1302()
{
 	RST = 0;
	SCL = 0;
	RST = 1;
}

/*写数据*/
void write_data_1302(unsigned char dat)
{
 	unsigned char i;
	SCL = 0;
	for(i = 0;i < 8;i++)
	{
	 	SDA = dat&0x01;
		SCL = 1;
		dat >>= 1;
		SCL = 0;//将这一句放到dat左移下面，可以起到延时的作用,延长了一个指令的时间
	}
}

/*读数据*/
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

/*将数据写入ds1302*/
void write_ds1302(unsigned char addr,unsigned char dat)
{
 	reset_1302();
	RST = 1;
	write_data_1302(addr);
	write_data_1302(dat);
	SDA = 0;   //关闭数据线，防止数据不小心被修改
	RST = 0;  //防止ds1302复位导致数据丢失？
}

/*从ds1302中读取数据*/
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


/*写保护函数*/
void write_protect()
{
	reset_1302();
	RST = 1;
	write_ds1302(0x8e,0x80);
	SDA = 0;
	RST = 0;
}
																	 
/*清除写保护*/
void clear_write_protect()
{
 	reset_1302();
	RST = 1;
	write_ds1302(0x8e,0);
	SDA = 0;
	RST = 0;
}



/*设定ds1302时间*/
void set_time_ds1302(unsigned char *timedata)
{
 	unsigned char i;
	unsigned char tmp;
	for(i = 0;i < 7;i++)
	{
		tmp = timedata[i]/10;			   //以下三句语句将十进制的数转换成了BCD码形式所对应的十进制数
		timedata[i] = timedata[i]%10;
		timedata[i] = timedata[i] + tmp*16;	 	
	}
	clear_write_protect();
	tmp = write_addr;
	for(i = 0;i < 7;i++)  //向ds1302中传入数组中的数据，即年月日时分秒
	{
	 	write_ds1302(tmp,timedata[i]);
		tmp += 2;//两个字节存一次  //因为0x80对应的是秒在寄存器中的存储地址，0x82是分对应的存储地址····依次类推
	}
	write_protect();
}

/*读取ds1302中的时间*/
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