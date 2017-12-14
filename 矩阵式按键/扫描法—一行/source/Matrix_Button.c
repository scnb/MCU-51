//这个程序可以在第一行与第一列之间转换，需改动是P2_0 = 0还是P2_4 = 0 ,然后再改变中断函数里的十六进制数值
//这个程序也可以改成四行里的任意一行，四列里的任意一列


#include <reg51.h>
#include "include/delay.h"
#include "include/main.h"
#include "intrins.h"
#include "include/hc595.h"


void Key_Check_Dis() interrupt 0 using 0       //中断程序
{
	
	//P2 = 0xf0;							   //如果写上这句话，那么所有的按键都可以被按下去
	if(0x0e == (P2&0x0f))				//行0xe0	   //如果是0x0f,则是列；如果是0xf0，则是行
	{
		Display[0] = Shu_Duan_Chose[1];	 
		Send_Data(Display[0],0xfe);
	}
	else if(0x0d == (P2&0x0f))				//行0xd0
	{
		Display[0] = Shu_Duan_Chose[5];
		Send_Data(Display[0],0xfe);
	}
	else if(0x0b == (P2&0x0f))				//行0xb0
	{
		Display[0] = Shu_Duan_Chose[9];
		Send_Data(Display[0],0xfe);
	}
	else if(0x07 == (P2&0x0f))			   //行0x70
	{
	    Display[0] = Shu_Duan_Chose[13];
		Send_Data(Display[0],0xfe);
	}
	//Send_Data(Display[0],0xfe);
}

/*void Init_Show()
{
	Send_Data(0x3f,0xfe);
} */




void main()
{
	Send_Data(0x3f,0xfe);
	EA = 1;
	EX0 = 1;
	IT0 = 0;
	TR0 = 0;
	//Init_Show();
	
	//P2_0 = 0;			//此定义说明是低四位的P2^0口置成了低电位 ,所以是第一行
	P2_4 = 0;			//此定义说明是高四位的P2^4口置成了低电位，所以是第一列
	P0 = 0xff;
	P1 = 0xff;
	while(1)
	{
		Delay1ms(2000);
		Delay1ms(2000);
	}
}