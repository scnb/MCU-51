#include<reg52.h>
#include<INTRINS.H>

sbit SCL = P0^6;
sbit SDA = P0^7;

/*定义从电脑接收的数据存储变量*/
unsigned char receive_data;//只接收数字，然后在数码管上显示
unsigned char receive_flag = 0;//接收成功标志位

sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;

unsigned char addr = 0x01,read_data; 

unsigned char value_high = 0;
unsigned char value_low = 0;
unsigned char temp = 0;

code unsigned char Duan_choose[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Wei_choose[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

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




/*i2c向器件即eeprom中写入数据*/
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




void Send_data(unsigned char duan,unsigned char wei)
{
	unsigned char i;	   //还是用hc595移位寄存器来进行操作，参照着数码管的原理与时序来写程序
 	HC595_RCK = 0;
	for(i = 0;i < 8;i++)
	{
	 	HC595_SCK = 0;
		if(0 != (duan&0x80))
		{
		 	HC595_DATA = 1;
		}
		else
		{
		 	HC595_DATA = 0;
		}
		HC595_SCK = 1;
		duan = duan << 1;
	}
	for(i = 0;i < 8;i++)
	{
	 	HC595_SCK = 0;
		if(0 != (wei&0x80))
		{
		 	HC595_DATA = 1;
		}
		else 
		{
		 	HC595_DATA = 0;
		}
		HC595_SCK = 1;
		wei = wei << 1;
	}
	HC595_RCK = 1;
}




/*定时器初始化  中断初始化    串口控制寄存器SCON初始化*/
void UART_init(void)
{
	SCON = 0x50;
	TMOD = 0x20;
	TH1  = 0xFD;
	TL1  = 0xFD;
	TR1  = 1;
	ET1 = 0;		//禁止定时器1中断
	EA = 1;
	ES = 1;
}

void UART_Send_Byte(unsigned char dat)
{
	SBUF = dat;
	while(!TI);
	TI = 0;
}

void Receive_fun()
{
	if(1 == receive_flag) //在while中不停的检测receive_flag是否为1,当为1的时候说明已经进入过中断，并且进入中断的条件是SBUF已经接收到数据
	{
	 	receive_flag = 0;
	
	} 	
}



void main()
{
	UART_init();
	InitI2C();	
	read_data = i2c_read_eeprom(addr);
	UART_Send_Byte(read_data);
	temp = read_data << 4;
	value_low = temp >> 4;
	value_high = read_data >> 4;
	while(1)
	{ 	
		Receive_fun();
		Send_data(Duan_choose[value_high],Wei_choose[0]);
		Send_data(Duan_choose[value_low],Wei_choose[1]);	
	}
			
}


void interrupt_4() interrupt 4
{
	//Send_data(Duan_choose[0],Wei_choose[0]);
 	RI = 0;
	receive_data = SBUF;
	i2c_write_eeprom(addr,receive_data);
	receive_flag = 1;
}