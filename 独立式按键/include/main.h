#ifndef _MAIN_H_
#define _MAIN_H_

//main函数中包含了基本的定义，比如数组什么的

#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};
#define  Lcd_Bus  P0
//typedef unsigned char uchar;
//typedef unsigned int uint;
#define uchar unsigned char
#define uint unsigned int


uchar k,direction;
bit Dis_Flag = 0;
bit flag5=0;			   //flag5是外部中断1的标志位 flag1是步进标志
uchar p,dengji;            //定时次数 
bit flag1=0;
//systemtime realtime;
bit first=1;	


sbit  P1_4 = P1^4;
sbit  P1_3 = P1^3;
sbit  P1_2 = P1^2;
sbit  P1_5 = P1^5;
sbit  P2_7 = P2^7;


uchar  dis[20] = {'0','1','2','3','4','5','6','7','8','9',};
uchar  dd[10] = {0};






code unsigned char Led8_One_Array[16] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char Led8_Two_Array[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

unsigned char Display[2] = {0x00,0x00};		//会用在Send_Data函数里控制显示哪两个led


uchar num;
uint delay_time;
uchar *zt = dd;	



#endif







