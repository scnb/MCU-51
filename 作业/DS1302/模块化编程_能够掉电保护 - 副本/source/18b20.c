#include "reg52.h"
#include"INTRINS.h"
#include "inc/18b20.h"

sbit _18b20_IO = P2^4;

void TempDelay (unsigned char us)
{
   while(us--);
}

void Delay(unsigned int count) //延时子函数
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


/*复位子程序*/
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

/*18b20写操作*/
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

/*18b20读操作*/
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

/*初始化程序*/
void ds18b20()
{
   ds_reset(); 
   Delay(1);  //约2ms
   ds_write_byte(0xcc);
   ds_write_byte(0x44);
}


/*从18b20中读取温度*/
unsigned int get_temperature()
{
   float wendu;
   unsigned char a,b;
   unsigned int temp;
   ds_reset();
   Delay(1);              //约2ms
   ds_write_byte(0xcc);
   ds_write_byte(0xbe);
   a=ds_read_byte();
   b=ds_read_byte();
   temp=b;
   temp<<=8;
   temp=temp|a;
   wendu=temp*0.0625;   //23.4*10 = 234 现在的wendu就是实际的温度值 
   temp = (unsigned int)(wendu*10+0.5); //在这里乘以十就是为了方便后面取出十位、个位和小数点位，加0.5是为了补偿误差，因为手册里说误差是±0.5度
   return temp;
}


unsigned int int_to_shiwei(unsigned int temp)
{
	unsigned int shiwei;
	shiwei = (temp%1000)/100;//取到百位的值
	return shiwei;
} 

unsigned int int_to_gewei(unsigned int temp)
{
	unsigned int gewei;
	gewei = (temp%100)/10;//取到十位的值
	return gewei;
}

unsigned int int_to_xiaoshuwei(unsigned int temp)
{
	unsigned int xiaoshuwei;
	xiaoshuwei = temp%10;//取到个位的值
	return xiaoshuwei;
}
			