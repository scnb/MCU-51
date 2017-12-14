#ifndef _MAIN_H_
#define _MAIN_H_

code unsigned char Shu_Duan_Chose[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Shu_Wei_Chose[16]  = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

unsigned char Display[8] = {0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f};

unsigned char T0RH = 0;	   //定时器0的定时时间计算出来的16进制数的高四位
unsigned char T0RL = 0;	   //定时器0的定时时间计算出来的16进制数的低四位

sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;
sbit P2_4 = P2^4;
sbit P2_5 = P2^5;
sbit P2_6 = P2^6;
sbit P2_7 = P2^7;

void Delay(unsigned int x);  //后加的
unsigned char dis_buf;		//后加的
unsigned char temp;			//后加的
unsigned char key;		   //后加的
#define ScanKeyPort P2    //后加的
unsigned char Key_Value;  //后加的

#endif