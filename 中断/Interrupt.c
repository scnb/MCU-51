#include<reg52.h>

sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;

typedef unsigned char uchar;
typedef unsigned int  uint;

//uchar counter,k;����Ĳ���
uchar k = 0;
uint counter=0;//ͬʱ����counter���˳�ֵ


code unsigned char Shu_Duan_Xuan[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
code unsigned char Shu_Wei_Xuan[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
unsigned char Led8_Eight[8]={0x00};

void Shu_Ma_Data(unsigned char Shu_Ma_Duan,unsigned char Shu_Ma_Wei)
{
	uchar i;
	HC595_RCK = 0;
	for(i=0;i<8;i++)
	{
		HC595_SCK = 0;
		if(0 != (Shu_Ma_Duan&0x80))
		{
			HC595_DATA = 1;
		}
		else
		{
			HC595_DATA = 0;
		}
		HC595_SCK=1;
	    Shu_Ma_Duan = Shu_Ma_Duan << 1;	   //��16���Ƶ�ÿһλ���в���
	}
	for(i=0;i<8;i++)
	{
		HC595_SCK = 0;
		if( 0 != (Shu_Ma_Wei&0x80))
		{
			HC595_DATA = 1;
		}
		else
		{
			HC595_DATA = 0;
		}
		HC595_SCK = 1;
	    Shu_Ma_Wei = Shu_Ma_Wei << 1;
	}
	 HC595_RCK = 1;
}

void Time1_Init()  //��ʱ��1��ʼ��
{
	TMOD = 0X10;
	TH1 = 0XFC;   //TH1��Ϊ��ʱ��1�ڷ�ʽ1�����µ�16λ�������ĸ�8λ ��0xfcֵ��ͨ����ʽ����ʱ1s�������
	TL1 = 0X66;	  //TL1��Ϊ��ʱ��1�ڷ�ʽ1�����µ�16λ�������ĵ�8λ
	TR1 = 1;
}

void int_Init()	  //�жϳ�ʼ��
{
	EA = 1;		  //�ж��ܿ���
	ET1 = 1;	  //��ʱ��1�ж��µ�IE����
}

void main()		  //������
{
	//Shu_Ma_Data(uchar Shu_Ma_Duan,uchar Shu_Ma_Wei);
	Time1_Init();
	int_Init();
	//interrupt();  //����Ϊ������ʼ��
	while(1);//(1)��ʡ��
}


void interrupt_time_1() interrupt 3  //�жϳ���	�������interrupt 3����д��
{
	//Shu_Ma_Data(uchar,uchar);
	static uint sec = 0;	 //��uint����uchar�������65536
	counter++;	                //�ú���Ӧ����Ƕ����һ��whileѭ���counter���ϵļӼ�
	TH1 = 0XFC;				   //ÿ��ѭ����ʼ֮ǰҪ��16λ���������¸�ֵ
	TL1 = 0X66;
	if(1000 == counter)
	{
		counter = 0;
		sec++;
		Led8_Eight[7] = sec%10;
		Led8_Eight[6] = sec/10%10;
		Led8_Eight[5] = sec/100%10;
		Led8_Eight[4] = sec/1000%10;
		Led8_Eight[3] = sec/10000%10;
	}
	if(k < 7)
	{
		k++;
	}		
	else
	{
		k = 0;
	}
	Shu_Ma_Data(Shu_Duan_Xuan[Led8_Eight[k]],Shu_Wei_Xuan[k]);
}