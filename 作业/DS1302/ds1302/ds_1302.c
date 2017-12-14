#include<reg52.h>
#include<INTRINS.h>
sbit RS = P1^2; 
sbit RW = P1^3;
sbit E =  P1^4;
sbit if_busy = P0^7;

sbit RST = P3^7;
sbit SCL = P3^5;
sbit SDA = P3^6;

#define write_addr 0x80;
#define read_addr  0x81;

unsigned char time[] = {30,19,22,25,1,4,17};//定义ds1302初始化的时间

/*设置串口波特率*/
void uart_init(void)
{
    TMOD = 0x21;        // 定时器1工作在方式2（自动重装）
    SCON = 0x50;        // 10位uart，允许串行接受

    TH1 = 0xFD;
    TL1 = 0xFD;

    TR1 = 1;
}

/*uart通信发送字节*/
void UART_Send_Byte(unsigned char dat)
{
	SBUF = dat;
	while (TI == 0);
	TI = 0;
}

/*uart通信发送（转换成ascii码形式）*/
void UART_Send_Dat(unsigned char dat)
{
	UART_Send_Byte(dat/16 + '0');
	UART_Send_Byte(dat%16 + '0');
}

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

void Init_1602()
{
	E = 0;
	write_command_1602(0x38);
	write_command_1602(0x06);
	write_command_1602(0x0c);
	write_command_1602(0x01);
}

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

/*写保护函数*/
void write_protect()
{
	reset_1302();
	RST = 1;
	write_data_1302(0x8e);
	write_data_1302(0x80);
	SDA = 0;
	RST = 0;
}

/*清除写保护*/
void clear_write_protect()
{
 	reset_1302();
	RST = 1;
	write_data_1302(0x8e);
	write_data_1302(0);
	SDA = 0;
	RST = 0;
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

/*向1602中写入字符串*/
void write_string(unsigned char addr_start,unsigned char display)
{

	write_command_1602(addr_start);

	write_data_1602(display);
}

void delay(unsigned int n)
{
	while (n--);
}



void main()
{
 	Init_1602();
	uart_init();
	set_time_ds1302(&time);
	while(1)
	{
		read_time_ds1302(&time);
		write_string(0x80,'D');
		write_string(0x81,'A');
		write_string(0x82,'T');
		write_string(0x83,'E');											
		write_string(0x85,time[6]/10+'0');
		write_string(0x86,time[6]%10+'0');
		write_string(0x87,'-');
		write_string(0x88,time[4]/10+'0');
		write_string(0x89,time[4]%10+'0');
		write_string(0x8a,'-');
		write_string(0x8b,time[3]/10+'0');
		write_string(0x8c,time[3]%10+'0');
	
		write_string(0xc0,'T');
		write_string(0xc1,'I');
		write_string(0xc2,'M');
		write_string(0xc3,'E');
		write_string(0xc5,time[2]/10+'0');
		write_string(0xc6,time[2]%10+'0');
		write_string(0xc7,':');
		write_string(0xc8,time[1]/10+'0');
		write_string(0xc9,time[1]%10+'0');
		write_string(0xca,':');
		write_string(0xcb,time[0]/10+'0');
		write_string(0xcc,time[0]%10+'0');
	}
}