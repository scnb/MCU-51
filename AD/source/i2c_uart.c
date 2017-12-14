#include<reg52.h>
#include "1602.h"
#include<INTRINS.H>

sbit SCL = P0^6;
sbit SDA = P0^7;




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

/*��ȡӦ��λ����ֵ*/
void i2c_read_ack()
{
 	SCL = 1;
	Delay5us();
	while(1==SDA);
	SCL = 0;
	Delay5us();
}	

/*i2c��������д������*/
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




/*i2c��pcf8591��д������*/
void i2c_write_pcf8591(unsigned char ctr_byte,unsigned char databyte)
{
 	i2c_start_fun();//��д�����Ŀ�ʼ��Ҫһ����ʼ�ź�
	i2c_write_data(0x90);//��д��������pcf8591�ĵ�ַ
//	i2c_read_ack();
	i2c_write_data(ctr_byte);//��д�������	
//	i2c_read_ack();
	i2c_write_data(databyte);//���д��Ҫ���͵�����
//  i2c_read_ack();
	i2c_stop_fun();//��д�����������Ҫһ��ֹͣ�ź�

}


/*i2c��pcf8591�ж�ȡ����*/
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
		 	num =  i2c_read_pcf8591(0);	   //��ȡ��һ·��ѹ5�Σ�����ֻ�������һ�Σ���ȡ���ȶ�ֵ
		}
		voltage=(float)num*5/256;//�ο���ѹ��5�����Գ���5�����̶ܿ�
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


