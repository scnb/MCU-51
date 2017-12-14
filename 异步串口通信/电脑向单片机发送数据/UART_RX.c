#include<reg52.h>

sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA =P1^4;

code unsigned char Shu_Duan[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x77,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Shu_Wei[8] =  {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

unsigned char Disbuf[8] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07};

unsigned char Rx_Data = 0;

unsigned char TORH = 0;		   //��������ֵ�ĸ���λ
unsigned char TORL = 0;		   //��������ֵ�ĵ���λ

void Timer0_Init(unsigned int ms)				//��ʱ��0��ʼ��
{
	unsigned long tmp;
	tmp = 11059200 / 12;	//��ʱ������Ƶ��
	tmp =(tmp * ms) / 1000;
	tmp = 65536 - tmp;
	tmp = tmp + 18;         //��18��Ϊ�˲������
	TORH = (unsigned char)(tmp >> 8);		//��tmpת����16������ȡ�������λ����Ϊ����ֵ
	TORL = (unsigned char)tmp;		//��tmpת����16����������0x0fʹ�����λ��0��ʣ�µ���λ����Ϊ����ֵ
	TMOD &= 0XF0;          //����TMOD ����ֵ��������0xf0�����ָ���λ���䣬�õ���λ���0
	TMOD |= 0X01;          //����0x01��ʹ����λ���0001�����T0���ó��˹�����ʽ1��
	TH0 = TORH;				//��Ϊ�����������ͬʱ�õ��˶�ʱ��0�Ͷ�ʱ��1
	TL0 = TORL;
	TR0 = 1;
	ET0 = 1;			  //��ʱ��0�����ж��ӳ�������ҪʹET0 = 1
}

void Scon_Timer1_Init()		        //SCON�Ͷ�ʱ��1��ʼ��
{
	TMOD &= 0X0F;
	TMOD |= 0X20;	//���ö�ʱ��1������ʽΪ2
	SCON = 0X50;	//���ô��п��ƼĴ���������ʽΪ1
	TH1 = 0XFD;	   //����ʱ��1ѡ������ʽ1ʱ��������8λ�ļ�������һ����TH1,һ����TL1
	TL1 = 0XFD;	   //��������������������Ȼ�����ǿ�ʼ���������Ӷ��ﵽ��ʱ��Ŀ��
	//ET1 = 1;
	TR1 = 1;			 //���ƶ�ʱ��1�������
	//ES = 1;				//�����жϿ���
}


void Send_Data(unsigned char Data_One,unsigned char Data_Two)  //����ܷ�������
{
	unsigned char i;
	HC595_RCK = 0;
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
		if(0 != (Data_One&0x80))
		{
			HC595_DATA = 1;
		}
		else
		{
			HC595_DATA = 0;
		}
		HC595_SCK = 1;
		Data_One = Data_One << 1;
	}
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
		if(0 != (Data_Two&0x80))
		{
			HC595_DATA = 1;
		}
		else
		{
			HC595_DATA = 0;
		}
		HC595_SCK = 1;
		Data_Two = Data_Two << 1;
	}
	HC595_RCK = 1;
}


void Delay1ms(int i)					 //��ʱ����
{
    int j;
    for(; i > 0; i--)
    {
         for(j = 0; j < 110; j++);
    }
}

void main()							 //������
{						 
	unsigned char k = 0;
	EA = 1;							  //�����ж�
	ES = 1;							  //�������ж�
	Timer0_Init(1);
	Scon_Timer1_Init();
	
	while(1)
	{
		Disbuf[0] = Shu_Duan[Rx_Data>>4];		 //������	 
		Disbuf[1] = Shu_Duan[Rx_Data&0x0f];
	
		for(k = 0;k < 8;k++)
		{
			Send_Data(Disbuf[k],Shu_Wei[k]);
			Delay1ms(1);
		}
	}
}

void interrupt_Timer0() interrupt 1	  //�Զ����ؼ������ĸ�λ�͵�λ
{
	TH0 = TORH;
	TL0 = TORL;
}

void interrupt_Serial() interrupt 4	  //���п��жϺ���,�жϽ��շ������
{
	if(1 == TI)				//TI�Ǵ��ڷ��ͱ�־���������1ʱ��־�����������(У��ֹͣλ)
	{
		TI = 0;			   //��TI��0���ܼ������з�������
		REN = 1;		   //REN��ʹ�ܴ��ڽ��գ���REN����1ʱ�������������
	}	
	if(1 == RI)			  //RI�Ǵ��ڽ��ձ�־���������1ʱ�������������(����ֹͣλ)
	{
		RI = 0;			  //��RI��0ʱ���ܼ�����������
		Rx_Data = SBUF;
	 
		SBUF = Rx_Data;	

		REN = 0;
	}
}
