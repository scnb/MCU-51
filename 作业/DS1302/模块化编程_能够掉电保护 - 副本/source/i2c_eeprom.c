
#include<reg52.h>
#include "inc/i2ceeprom.h"
#include<INTRINS.h>

/*对at24c02的芯片引脚进行设置*/
sbit SCL = P0^6;
sbit SDA = P0^7;

//unsigned char addr = 0x01;//器件起始地址

void Delay_i2c(void)
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();	
}


/*i2c初始化设置*/
void InitI2C(void)
{
	 SDA = 1;
	 SCL = 1;
}

/*I2C起始信号*/
void i2c_start_fun()
{
 	SDA = 1;
	Delay_i2c();
	SCL = 1;
	Delay_i2c();
	SDA = 0;
	Delay_i2c();
	SCL = 0;
}

/*i2c停止信号*/
void i2c_stop_fun()
{
 	SCL = 0;
	Delay_i2c();
	SDA = 0;
	Delay_i2c();
	SCL = 1;
	Delay_i2c();
	SDA = 1;
	Delay_i2c();
}




/*i2c向器件即eeprom中写入一个字节*/
void i2c_write_data(unsigned char byte)//i2c先发送高位再发送低位
{
 	unsigned char i;
	unsigned char j;
	unsigned char mask = 0x80;
	for(i = 0;i < 8;i++)
	{
		SCL = 0; //只有在scl为低电平的时候，sda的状态才能发生变化
		Delay_i2c();
	 	if(0 == (mask&byte)) //必须得是判断0 == (mask&byte),如果是判断1 == (mask&byte)的话，那么会出错
		{
		 	SDA = 0;
		}
		else
		{
		 	SDA = 1;
		}
		mask >>= 1;
		Delay_i2c(); 
		SCL = 1;	//在sda发送数据时，scl必须保持高电平
		Delay_i2c();
	}
	SCL = 0;  //最后SCL经历了一个从低电平到高电平再到低电平的变化,而SDA的状态一直不变
	SDA = 1;
	Delay_i2c();    //还缺少接受从机应答位的语句
	SCL = 1;
	j = SDA;
	Delay_i2c();
	SCL = 0;		
}

/*从eeprom中读取一个字节*/
unsigned char i2c_read_data()
{
 	unsigned char i;
	unsigned char byte = 0;
	for(i = 0;i < 8;i++)
	{
		SCL = 0;
		SDA = 1;//每一次必须先将SDA置高电平
		Delay_i2c();
		SCL = 1;
		Delay_i2c();
		byte <<= 1;	//先左移一位,如果左移放在if语句后面的话，当i=7的时候，最后还会执行一句左移，那么最高位将会被移出去
	 	if(1 == SDA)
		{
			byte = byte | 0x01;		 	
		}					   
		Delay_i2c();
	}
	SCL = 0;
	SDA = 1;
	Delay_i2c();
	SCL = 1;
	Delay_i2c();
	SCL = 0;
	return byte;
}




/*i2c向eeprom中写入数据*/
void i2c_write_eeprom(unsigned char addr,unsigned char databyte)
{
 	i2c_start_fun();//在写函数的开始需要一个起始信号
	i2c_write_data(0xa0);//先写入器件即eeprom的地址

	i2c_write_data(addr);//再写入数据存储地址

	i2c_write_data(databyte);//最后写入要发送的数据

	i2c_stop_fun();//在写函数的最后需要一个停止信号

}


/*i2c从eeprom中读取数据*/
unsigned char i2c_read_eeprom(unsigned char addr)
{
	unsigned char databyte;
 	i2c_start_fun();
	i2c_write_data(0xa0);

	i2c_write_data(addr);

	i2c_start_fun();
	i2c_write_data(0xa1);

	databyte = i2c_read_data();
	i2c_stop_fun();
	return databyte;
}