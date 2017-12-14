#include<reg52.h>

sbit BUZZ = P3^4;

unsigned char T0RH = 0;
unsigned char T0RL = 0;


void start_buzz(unsigned int frequency)//频率很高的，一般以KHz为单位				
{
	unsigned int reload;
 	reload = 65536 - 11059200/12/(frequency*2);
	T0RH = (unsigned char)(reload>>8);//保留reload的高8位，实际上是先把reload右移8位，使高8位变成低8位，最后通过unsigned char来强制保留低8位
	T0RL = (unsigned char)reload;  //只保留reload的低8位，因为unsigned char型只占一个字节
	TH0 = 0xFF;
	TL0 = 0xFE;
	//TH0 = T0RH;	   //为什么不能这么赋初值呢？
	//TL0 = T0RL;

	TR0 = 1;
	ET0 = 1;
}

void stop_buzz()
{
	TR0 = 0;
	ET0 = 0;
}



void main()
{
	unsigned int i;//i的计数达到了40000多，所以必须要用int   unsigned int 从0到65536
	TMOD = 0x01;										   //unsigned char 从0到255
	EA = 1;
 	while(1)
	{
		start_buzz(7000);
		for(i=0; i<40000; i++);
		stop_buzz();
		for(i=0; i<40000; i++);
		start_buzz(1000);			 	
		for(i=0; i<40000; i++);
		stop_buzz();
		for(i=0; i<40000; i++);				 	
	}
}


void interrupt_timer0() interrupt 1
{
 	TH0 = T0RH;
	TL0 = T0RL;
	BUZZ = ~BUZZ;
}