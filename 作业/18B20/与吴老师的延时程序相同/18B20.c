#include<reg52.h>
#include"INTRINS.h"

sbit _18b20_IO = P2^4;

sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;

code unsigned char Duan_choose[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Wei_choose[16] =  {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
unsigned int temp;
unsigned char _display[4] = {0};



void TempDelay (unsigned char us)
{
   while(us--);
}

void delay(unsigned int count) //��ʱ�Ӻ���
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

void Delay1ms(int i)
{
    int j;
    for(; i > 0; i--)
	{
         for(j = 0; j < 110; j++);
    }
}


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
/*	if(0 == _18b20_IO)
	{
	 	reset_flag = 1;//������λ�ɹ�
	}
	else
	{
	 	reset_flag = 0;//������λʧ�ܣ����������޷�����
	}
*/
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
   delay(1);  //Լ2ms
   ds_write_byte(0xcc);
   ds_write_byte(0x44);
}


/*��18b20�ж�ȡ�¶�*/
unsigned int get_temperature()
{
   float wendu;
   unsigned char a,b;
   ds_reset();
   delay(1);              //Լ2ms
   ds_write_byte(0xcc);
   ds_write_byte(0xbe);
   a=ds_read_byte();
   b=ds_read_byte();
   temp=b;
   temp<<=8;
   temp=temp|a;
   wendu=temp*0.0625;   //23.4*10 = 234  
   temp = (unsigned int)(wendu*10+0.5); 
   return temp;
}

/*����ܷ��ͳ���*/
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

/*�������ʾ����*/
void int_to_hex(unsigned int temp)
{
	_display[0] = Duan_choose[(temp%1000)/100];//ȡ����λ��ֵ
	_display[1] = Duan_choose[(temp%100)/10];//ȡ��ʮλ��ֵ
	_display[2] = Duan_choose[temp%10];//ȡ����λ��ֵ
	_display[3] = 0x39;
	_display[1] |= 0x80;
} 

/*������*/
void main()
{
 	unsigned char i;
	unsigned char a;
	while(1)
	{
	    ds18b20();          //12λת��ʱ�����Ϊ750ms
	    for(a=10;a>0;a--)
	    {
		    get_temperature();
			int_to_hex(temp);
			for(i = 0;i < 4;i++)
		    {
	             Send_data(_display[i], Wei_choose[i]);   
			     Delay1ms(1);
	        }	  
	     }
	 }
}				