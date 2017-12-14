
#include<reg52.h>
#include "inc/i2ceeprom.h"
#include<INTRINS.h>

/*��at24c02��оƬ���Ž�������*/
sbit SCL = P0^6;
sbit SDA = P0^7;

//unsigned char addr = 0x01;//������ʼ��ַ

void Delay_i2c(void)
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();	
}


/*i2c��ʼ������*/
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




/*i2c��������eeprom��д��һ���ֽ�*/
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

/*��eeprom�ж�ȡһ���ֽ�*/
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