#include <reg52.h>
#include <INTRINS.H>
#define uchar unsigned char 
#define uint unsigned int 
#define ulint unsigned long 
sbit I2C_SCL_CK = P0^6;
sbit I2C_SDA_DA = P0^7;

/*****************************************************************************
** 函数名称：Delay_I2C
** 功能描述：延时子程序
******************************************************************************/
void Delay_I2C(void)
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();	
}

/*****************************************************************************
** 函数名称：InitI2C
** 功能描述：配置模拟I2C的IO端口
** 可移植性：注意IO口需支持漏开输出
******************************************************************************/
void InitI2C(void)
{
	 I2C_SDA_DA = 1;
	 I2C_SCL_CK = 1;
}

/*****************************************************************************
** 函数名称：I2C_Start_Fun
** 功能描述：发送I2C总线起始状态
** 输    入：无
** 输    出：无
** 全局变量：无
** 调用模块：Delay_I2C()
******************************************************************************/
void I2C_Start_Fun(void)
{
	I2C_SDA_DA = 1;    
	Delay_I2C();      // 延时子程序
	I2C_SCL_CK = 1;
	Delay_I2C();
	I2C_SDA_DA = 0;
	Delay_I2C();
	I2C_SCL_CK = 0;
}

/*****************************************************************************
** 函数名称：I2C_Stop_Fu
** 功能描述：发送I2C总线停止起始状态
** 输    入：无
** 输    出：无
** 全局变量：无
** 调用模块：Delay_I2C()
** 可移植性：直接移植
******************************************************************************/
void I2C_Stop_Fu(void)
{
	I2C_SCL_CK = 0;
        Delay_I2C();
	I2C_SDA_DA = 0;
	Delay_I2C();
	I2C_SCL_CK = 1;
	Delay_I2C();
	I2C_SDA_DA = 1;
	Delay_I2C();
}

/*****************************************************************************
** 函数名称：I2C_Write_Data
** 功能描述：向I2C总线发送一个字节数据，并检测应答
** 输    入：待发送字节byte
** 输    出：无
** 全局变量：无
** 调用模块：Delay_I2C()
** 可移植性：直接移植
******************************************************************************/
void I2C_Write_Data(uchar Byte)
{
	uchar mask;
	uchar i;
	uchar j;

	mask = 0x80;
	for(i = 0; i < 8; i++)
	{
		I2C_SCL_CK = 0;
		Delay_I2C();
		if((mask & Byte) == 0)
		{
			I2C_SDA_DA = 0;
		}
		else
		{
			I2C_SDA_DA = 1;
		}
		mask >>= 1;
		Delay_I2C();
		I2C_SCL_CK = 1;
		Delay_I2C();
	}
	
	I2C_SCL_CK = 0;
	I2C_SDA_DA = 1;
	Delay_I2C();
	I2C_SCL_CK = 1;
	j = I2C_SDA_DA;
	Delay_I2C();
	I2C_SCL_CK = 0;	
}


	
/*****************************************************************************
** 函数名称：I2C_Read_LData
** 功能描述：从I2C总线读取最后一个字节数据，并发送非应答位
** 输    入：无
** 输    出：接收到的字节Byte
** 全局变量：无
** 调用模块：Delay_I2C()
** 可移植性：直接移植
******************************************************************************/
uchar I2C_Read_LData(void)
{
	uchar Byte;
	uchar i;

	Byte = 0;
	for(i = 0; i < 8; i++)
	{
		I2C_SCL_CK = 0;
		I2C_SDA_DA = 1;
		Delay_I2C();
		I2C_SCL_CK = 1;
		Delay_I2C();
		Byte <<= 1;	 
		if(I2C_SDA_DA == 1)
		{
			Byte |= 0x01;
		}
		Delay_I2C();
	} 
	I2C_SCL_CK = 0;
	I2C_SDA_DA = 1;
	Delay_I2C();
	I2C_SCL_CK = 1;
	Delay_I2C();
	I2C_SCL_CK = 0;
	
	return Byte;
}

/*****************************************************************************
** 函数名称：I2C_Read_Ack
** 功能描述：从I2C总线读取一个字节数据，并发送应答
** 输    入：无
** 输    出：接收到的字节byte
** 全局变量：无
** 调用模块：Delay_us()
** 可移植性：直接移植
******************************************************************************/
uchar I2C_Read_Ack(void)
{
	uchar i;
	uchar Byte;
	
	Byte = 0;
	for(i=0;i<8;i++)
	{
		I2C_SCL_CK = 0;
		I2C_SDA_DA = 1;
		Delay_I2C();
		I2C_SCL_CK = 1;
		Delay_I2C();
		Byte <<= 1;
		if(I2C_SDA_DA==1) {Byte |= 0x01;}
		Delay_I2C();
	}
	I2C_SCL_CK = 0;
	Delay_I2C();
	I2C_SDA_DA = 0;
	Delay_I2C();
	I2C_SCL_CK = 1;
	Delay_I2C();
	I2C_SCL_CK = 0;
	
	return Byte;
}	
 
/*****************************************************************************
** 函数名称：read_eeprom
** 功能描述：读取EEPROM数据函数（可多片共存）
** 输    入：E2中目的地址addr
** 输    出：读取的数据
** 全局变量：无
** 调用模块：I2CStart()，I2CSend()，I2CStop()
******************************************************************************/
uchar read_eeprom(uchar addr)
{
	uchar databyte;

	I2C_Start_Fun();
	I2C_Write_Data(0xa0);
	I2C_Write_Data(addr);
	I2C_Start_Fun();
	I2C_Write_Data(0xa1);
	databyte = I2C_Read_LData();
	I2C_Stop_Fu();

	return databyte;
	
}

/*****************************************************************************
** 函数名称：write_eeprom
** 功能描述：读取EEPROM数据函数（可多片共存）
** 输    入：E2中目的地址addr
** 输    出：读取的数据
******************************************************************************/
void write_eeprom(uchar addr, uchar databyte)
{
	I2C_Start_Fun();
	I2C_Write_Data(0xa0);
	I2C_Write_Data(addr);
	I2C_Write_Data(databyte);
	I2C_Stop_Fu();

}

/*****************************************************************************
** 函数名称：UART_init
** 功能描述：串口初始化
******************************************************************************/
void UART_init(void)
{
	SCON = 0x50;
	TMOD = 0x20;
	TH1  = 0xFD;
	TL1  = 0xFD;
	TR1  = 1;
}

/*****************************************************************************
** 函数名称：UART_Send_Byte
** 功能描述：串口发送程序
******************************************************************************/
void UART_Send_Byte(uchar dat)
{
	SBUF = dat;
	while(!TI);
	TI = 0;
}

/*****************************************************************************
** 函数名称：主函数main
** 功能描述：读写EEPROM
** 输    入：
** 输    出：
******************************************************************************/
main()
{
	uchar addr = 0x01, databyte = 0x56,read_data;
	uchar c = 0;
//	uint i;
	
	UART_init();
	InitI2C();
		
	read_data = read_eeprom(addr);
	UART_Send_Byte(read_data);
	read_data += 5;
	if(databyte >= 0xff) {databyte = 0;}
	write_eeprom(addr, read_data);
	 
	while(1){};		
}

