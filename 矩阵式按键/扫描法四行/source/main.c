#include<reg51.h>
#include "include/hc595.h"
#include "include/delay.h"
#include "include/main.h"
#include "intrins.h"

//补充一个函数，本来我是没有的
void Delay(unsigned int tt)	  //用来延时?
{
	while(tt--);          //当tt自减到0是就退出循环
}

//再补充一个函数，需要加一个定时器
void Timer0_Init(unsigned int ms)
{
	unsigned long tmp;
	tmp = 11059200 / 12;		 //以下三步为计算定时时长的算法
	tmp =(tmp * ms) / 1000;
	tmp = 65536 - tmp;
	tmp = tmp + 18;         //补偿中断响应延时造成的误差
	T0RH = (unsigned char)(tmp >> 8); //定时器重载值拆分为高低字节,其实就是求16进制数
	T0RL = (unsigned char)tmp;
	TMOD &= 0XF0;	 //将T0的控制位(低四位)清零
	TMOD |= 0X01;	 //将T0的模式设置为方式1
	TH0 = T0RH;		 //加载T0重载值，其实和正常算法是一样的
	TL0 = T0RL;		  //所谓重载值，就是当定时器定时时间结束，即溢出的时候，重新赋值，再次进入定时状态
	ET0 = 1;   //定时器0的中断
	TR0 = 1;   //使定时器0运行
		
}





unsigned char Scan_Key()
{
	unsigned char Key_Temp,Row_Temp,Col_Temp;    //依次存放P2的值（16进制）,行信息，列信息
	P2 = 0xf0;	   //给低四位送0
	Key_Temp = P2;
	if((Key_Temp&0xf0)^0xf0)   //判断到底有没有键被按下，先与运算把低四位置0，
	                           //在异或运算判断高四位是不是都是1，只要高四位有一个0，
							   //if判断就能通过
	{
	 	Row_Temp = 0xfe;         //赋初值，能不能赋成别的开始进行行上的操作
		//如果一行一行的检测，应该用低四位来判断是哪一行
		while((Row_Temp&0x10) != 0x00)  //在第一行到第四行间循环，判断是不是在这四行内，例如第一行0B1111 1110,和0B0001 0000取与运算，得到0B0001 0000，然后把Key_Temp左移四次，直到第四行0B1111 0111，取与运算还是0x10，再左移一次变成0B1110 1111，这时取与运算则变成0x00
		{
			P2 = Row_Temp;		 //P2到底是由软件决定的，还是硬件决定的？
			if((P2&0xf0)^0xf0)	 //P2继承了现在的行的信息
			{
				Col_Temp = (P2&0xf0)|0x0f; //与运算保持了P2的高四位，所以是哪列，那个位置就是0(取反后变为1)或运算使低四位置1，方便取反
				return ((~Col_Temp) + (~Row_Temp));	
			}
			else
			{
				Row_Temp = (Row_Temp << 1) | 0x01;	   //将列值左移以为，并且将右边补的0取或变成1
			}
			Delay(10);
		}
	}
	else
	{
		return (0);
	}
}




void Interrupt() interrupt 1 //用的是定时器0中断
{
	static unsigned char k = 0;
	//static unsigned char i;
	
	TH0 = T0RH;
	TL0 = T0RL;
	
	Send_Data(Display[0],Shu_Wei_Chose[k]);
	if(k < 7)
	{										//把这一段程序屏蔽掉之后，就只有第一位数码管显示
		//i++;
		k++;
	}	
	
	else									 //把位选换成k之后，8位数码管一同显示
	{
		//i = 0;
		k = 0;
	} 
}







void main()
{
	//unsigned int Key_Value;
	unsigned char Key_Data;
	P0 = 0xff;	 //为什么要把P0和P1设置成0xff
	P1 = 0xff;   
	Delay(10);
	Timer0_Init(1);
	EA = 1;
	P2_4 = 0;   //为什么要单独设定P2^4为0呢？
	while(1)
	{
		Key_Data = Scan_Key();
		if(Key_Data != 0)
		{
			switch(Key_Data)
			{
				case 0x11:Key_Value = 0;break;
				case 0x21:Key_Value = 1;break;
				case 0x41:Key_Value = 2;break;
				case 0x81:Key_Value = 3;break;		

				case 0x12:Key_Value = 4;break;
				case 0x22:Key_Value = 5;break;
				case 0x42:Key_Value = 6;break;
				case 0x82:Key_Value = 7;break;

				case 0x14:Key_Value = 8;break;
				case 0x24:Key_Value = 9;break;
				case 0x44:Key_Value = 10;break;
				case 0x84:Key_Value = 11;break;

				case 0x18:Key_Value = 12;break;
				case 0x28:Key_Value = 13;break;
				case 0x48:Key_Value = 14;break;
				case 0x88:Key_Value = 15;break;
			}
		}
		Key_Data = 0;    //把Key_Data清零，防止重复进入if
		//Send_Data(Shu_Duan_Chose[Key_Value],0xfe);
		Display[0] = Shu_Duan_Chose[Key_Value];
	}
}