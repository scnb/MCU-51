#include<reg52.h>
#include "1602.h"
#include<INTRINS.H>

sbit SCL = P0^6;
sbit SDA = P0^7;




/*****************************************************************************
** 函数名称：Delay_I2C
** 功能描述：延时子程序
******************************************************************************/
void Delay_i2c(void)
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();	
}

void Delay5us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 11;
	while (--i);
}


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

/*读取应答位返回值*/
void i2c_read_ack()
{
 	SCL = 1;
	Delay5us();
	while(1==SDA);
	SCL = 0;
	Delay5us();
}	

/*i2c向器件中写入数据*/
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




/*i2c向pcf8591中写入数据*/
void i2c_write_pcf8591(unsigned char ctr_byte,unsigned char databyte)
{
 	i2c_start_fun();//在写函数的开始需要一个起始信号
	i2c_write_data(0x90);//先写入器件即pcf8591的地址
//	i2c_read_ack();
	i2c_write_data(ctr_byte);//再写入控制字	
//	i2c_read_ack();
	i2c_write_data(databyte);//最后写入要发送的数据
//  i2c_read_ack();
	i2c_stop_fun();//在写函数的最后需要一个停止信号

}


/*i2c从pcf8591中读取数据*/
unsigned char i2c_read_pcf8591(unsigned char ctr_byte)
{
	unsigned char databyte;
 	i2c_start_fun();
	i2c_write_data(0x90);
	//i2c_read_ack();
	i2c_write_data(ctr_byte);
	//i2c_read_ack();
	i2c_start_fun();
	i2c_write_data(0x91);
	//i2c_read_ack();
	databyte = i2c_read_data();
	i2c_stop_fun();
	return databyte;
}






void main()
{
	unsigned char num=0,i;
	unsigned char temp[7];
	float voltage;
	LCD_Init();
	DelayMs(20);
	LCD_Clear();
	while(1)
	{
	 	for(i=0;i<5;i++)
		{
		 	num =  i2c_read_pcf8591(0);	   //读取第一路电压5次，但是只保留最后一次，以取得稳定值
		}
		voltage=(float)num*5/256;//参考电压是5，所以乘以5除以总刻度
		sprintf(temp,"V0 %3.2f ",voltage);
		write_string(temp);

		for(i=0;i<5;i++)
		{
		 	num=i2c_read_pcf8591(1);
		}
		voltage=(float)num*5/256;
		sprintf(temp,"V1 %3.2f ",voltage);
		write_string(8,0,temp);
	}
}


