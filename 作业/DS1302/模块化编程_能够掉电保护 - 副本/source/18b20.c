#include "reg52.h"
#include"INTRINS.h"
#include "inc/18b20.h"

sbit _18b20_IO = P2^4;

void TempDelay (unsigned char us)
{
   while(us--);
}

void Delay(unsigned int count) //��ʱ�Ӻ���
{
   unsigned int i;
   while(count)
   {
      i=200;
      while(i>0)
      i--;
      count--;
   }
}

/*void Delay1ms(int i)
{
    int j;
    for(; i > 0; i--)
	{
         for(j = 0; j < 110; j++);
    }
} */


/*��λ�ӳ���*/
void ds_reset()
{
 	_18b20_IO = 0;
	TempDelay(80);
	_nop_();
	_18b20_IO = 1;
	TempDelay(5);
	_nop_();
	_nop_();
	_nop_();

	TempDelay(20);
	_nop_();
	_nop_();
	_18b20_IO = 1;			
}

/*18b20д����*/
void ds_write_byte(unsigned char dat)
{
	unsigned char i;
	for(i = 0;i < 8;i++)
	{
		if(1 == (dat&0x01))
		{
			_18b20_IO = 0;
			_nop_();
			_nop_();
	 		_18b20_IO = 1;
			TempDelay(5);
		}
		else
		{
	 		_18b20_IO = 0;
			TempDelay(8);
			_18b20_IO = 1;
			_nop_();
			_nop_();
		}
		dat >>= 1;
	}
}

/*18b20������*/
unsigned char ds_read_byte()
{
	unsigned char temp;
	unsigned char dat=0;
	unsigned char i;
	for(i = 0;i < 8;i++)
	{
	 	_18b20_IO = 0;
		_nop_();
		_18b20_IO = 1;
		_nop_();
		_nop_();
		temp = _18b20_IO;
		TempDelay(10);
		dat = (temp << 7)|(dat>>1);
	}	
	return dat;
}

/*��ʼ������*/
void ds18b20()
{
   ds_reset(); 
   Delay(1);  //Լ2ms
   ds_write_byte(0xcc);
   ds_write_byte(0x44);
}


/*��18b20�ж�ȡ�¶�*/
unsigned int get_temperature()
{
   float wendu;
   unsigned char a,b;
   unsigned int temp;
   ds_reset();
   Delay(1);              //Լ2ms
   ds_write_byte(0xcc);
   ds_write_byte(0xbe);
   a=ds_read_byte();
   b=ds_read_byte();
   temp=b;
   temp<<=8;
   temp=temp|a;
   wendu=temp*0.0625;   //23.4*10 = 234 ���ڵ�wendu����ʵ�ʵ��¶�ֵ 
   temp = (unsigned int)(wendu*10+0.5); //���������ʮ����Ϊ�˷������ȡ��ʮλ����λ��С����λ����0.5��Ϊ�˲�������Ϊ�ֲ���˵����ǡ�0.5��
   return temp;
}


unsigned int int_to_shiwei(unsigned int temp)
{
	unsigned int shiwei;
	shiwei = (temp%1000)/100;//ȡ����λ��ֵ
	return shiwei;
} 

unsigned int int_to_gewei(unsigned int temp)
{
	unsigned int gewei;
	gewei = (temp%100)/10;//ȡ��ʮλ��ֵ
	return gewei;
}

unsigned int int_to_xiaoshuwei(unsigned int temp)
{
	unsigned int xiaoshuwei;
	xiaoshuwei = temp%10;//ȡ����λ��ֵ
	return xiaoshuwei;
}
			