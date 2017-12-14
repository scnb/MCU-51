#include<reg52.h>
#include<INTRINS.H>

sbit SCL = P0^6;
sbit SDA = P0^7;


sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;


unsigned char addr = 0x01,read_data; 

unsigned char second = 59,minute = 59,hour = 11,day = 25,month = 11,week = 3,year = 16;

unsigned char time[] = {50,59,23,31,12,3,16};//����ds1302��ʼ����ʱ��

code unsigned char Duan_choose[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Wei_choose[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};



/*****************************************************************************
** �������ƣ�Delay_I2C
** ������������ʱ�ӳ���
******************************************************************************/
void Delay_i2c(void)
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();	
}

void Delay(unsigned int i)
{
 	unsigned char j;
	for(;i>0;i--)
	{
	 	for(j = 0;j < 110;j++)
		{}
	}
}

void InitI2C(void)
{
	 SDA = 1;
	 SCL = 1;
}

/*I2C��ʼ�ź�*/
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

/*i2cֹͣ�ź�*/
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




/*i2c��������eeprom��д������*/
void i2c_write_data(unsigned char byte)//i2c�ȷ��͸�λ�ٷ��͵�λ
{
 	unsigned char i;
	unsigned char j;
	unsigned char mask = 0x80;
	for(i = 0;i < 8;i++)
	{
		SCL = 0; //ֻ����sclΪ�͵�ƽ��ʱ��sda��״̬���ܷ����仯
		Delay_i2c();
	 	if(0 == (mask&byte)) //��������ж�0 == (mask&byte),������ж�1 == (mask&byte)�Ļ�����ô�����
		{
		 	SDA = 0;
		}
		else
		{
		 	SDA = 1;
		}
		mask >>= 1;
		Delay_i2c(); 
		SCL = 1;	//��sda��������ʱ��scl���뱣�ָߵ�ƽ
		Delay_i2c();
	}
	SCL = 0;  //���SCL������һ���ӵ͵�ƽ���ߵ�ƽ�ٵ��͵�ƽ�ı仯,��SDA��״̬һֱ����
	SDA = 1;
	Delay_i2c();    //��ȱ�ٽ��ܴӻ�Ӧ��λ�����
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
		SDA = 1;//ÿһ�α����Ƚ�SDA�øߵ�ƽ
		Delay_i2c();
		SCL = 1;
		Delay_i2c();
		byte <<= 1;	//������һλ,������Ʒ���if������Ļ�����i=7��ʱ����󻹻�ִ��һ�����ƣ���ô���λ���ᱻ�Ƴ�ȥ
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




/*i2c��eeprom��д������*/
void i2c_write_eeprom(unsigned char addr,unsigned char databyte)
{
 	i2c_start_fun();//��д�����Ŀ�ʼ��Ҫһ����ʼ�ź�
	i2c_write_data(0xa0);//��д��������eeprom�ĵ�ַ

	i2c_write_data(addr);//��д�����ݴ洢��ַ

	i2c_write_data(databyte);//���д��Ҫ���͵�����

	i2c_stop_fun();//��д�����������Ҫһ��ֹͣ�ź�

}


/*i2c��eeprom�ж�ȡ����*/
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





/*��ʱ����ʼ��  �жϳ�ʼ��    ���ڿ��ƼĴ���SCON��ʼ��*/
void UART_init(void)
{
	SCON = 0x50;
	TMOD = 0x20;
	TH1  = 0xFD;
	TL1  = 0xFD;
	TR1  = 1;
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	EA = 1;
	ES = 1;
}

/*void UART_Send_Byte(unsigned char dat)
{
	SBUF = dat;
	while(!TI);
	TI = 0;
} */


void Send_data(unsigned char duan,unsigned char wei)
{
	unsigned char i;	   //������hc595��λ�Ĵ��������в���������������ܵ�ԭ����ʱ����д����
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




void main()
{
	unsigned char i,j;
	UART_init();
	InitI2C();	
	i2c_write_eeprom(0x01,time[0]);
	i2c_write_eeprom(0x02,time[1]);
	i2c_write_eeprom(0x03,time[2]);
	i2c_write_eeprom(0x04,time[3]);
	i2c_write_eeprom(0x05,time[4]);
	i2c_write_eeprom(0x06,time[5]);
	i2c_write_eeprom(0x06,time[6]);



	/*while(1)
	{
		for(i = 10000;i>0;i--)
		{
			for(j = 0;j < 110;j++)
			{			
				Send_data(Duan_choose[second1],Wei_choose[0]);
				Send_data(Duan_choose[second2],Wei_choose[1]);
			}
		}
		Delay(1000);	

		for(i = 10000;i>0;i--)
		{
			for(j = 0;j < 110;j++)
			{			
				Send_data(Duan_choose[minute1],Wei_choose[0]);
				Send_data(Duan_choose[minute2],Wei_choose[1]);
			}
		}
		Delay(1000);
	
		for(i = 10000;i>0;i--)
		{
			for(j = 0;j < 110;j++)
			{			
				Send_data(Duan_choose[hour1],Wei_choose[0]);
				Send_data(Duan_choose[hour2],Wei_choose[1]);
			}
		}
		Delay(1000);
	
		for(i = 10000;i>0;i--)
		{
			for(j = 0;j < 110;j++)
			{			
				Send_data(Duan_choose[day1],Wei_choose[0]);
				Send_data(Duan_choose[day2],Wei_choose[1]);
			}
		}
		Delay(1000);
		
		for(i = 10000;i>0;i--)
		{
			for(j = 0;j < 110;j++)
			{			
				Send_data(Duan_choose[month1],Wei_choose[0]);
				Send_data(Duan_choose[month2],Wei_choose[1]);
			}
		}
		Delay(1000);
		
		for(i = 10000;i>0;i--)
		{
			for(j = 0;j < 110;j++)
			{			
				Send_data(Duan_choose[week1],Wei_choose[0]);
				Send_data(Duan_choose[week2],Wei_choose[1]);
			}
		}
		Delay(1000);
		
		for(i = 10000;i>0;i--)
		{
			for(j = 0;j < 110;j++)
			{			
				Send_data(Duan_choose[year1],Wei_choose[0]);
				Send_data(Duan_choose[year2],Wei_choose[1]);
			}
		}
		Delay(1000);
	}
	  */

	



/*	UART_Send_Byte(i2c_read_eeprom(0x01)+'0');
	UART_Send_Byte(i2c_read_eeprom(0x02)+'0');
	UART_Send_Byte(i2c_read_eeprom(0x03)+'0');
	UART_Send_Byte(i2c_read_eeprom(0x04)+'0');
	UART_Send_Byte(i2c_read_eeprom(0x05)+'0');
	UART_Send_Byte(i2c_read_eeprom(0x06)+'0');
	UART_Send_Byte(i2c_read_eeprom(0x08)+'0');
	UART_Send_Byte(i2c_read_eeprom(0x09)+'0');
	UART_Send_Byte(i2c_read_eeprom(0x0a)+'0');
	UART_Send_Byte(i2c_read_eeprom(0x0b)+'0');
	UART_Send_Byte(i2c_read_eeprom(0x0c)+'0');
	UART_Send_Byte(i2c_read_eeprom(0x0d)+'0');
*/
}

