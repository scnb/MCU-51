#include<reg52.h>

sbit BUZZ = P3^4;

unsigned char T0RH = 0;
unsigned char T0RL = 0;


void start_buzz(unsigned int frequency)//Ƶ�ʺܸߵģ�һ����KHzΪ��λ				
{
	unsigned int reload;
 	reload = 65536 - 11059200/12/(frequency*2);
	T0RH = (unsigned char)(reload>>8);//����reload�ĸ�8λ��ʵ�������Ȱ�reload����8λ��ʹ��8λ��ɵ�8λ�����ͨ��unsigned char��ǿ�Ʊ�����8λ
	T0RL = (unsigned char)reload;  //ֻ����reload�ĵ�8λ����Ϊunsigned char��ֻռһ���ֽ�
	TH0 = 0xFF;
	TL0 = 0xFE;
	//TH0 = T0RH;	   //Ϊʲô������ô����ֵ�أ�
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
	unsigned int i;//i�ļ����ﵽ��40000�࣬���Ա���Ҫ��int   unsigned int ��0��65536
	TMOD = 0x01;										   //unsigned char ��0��255
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