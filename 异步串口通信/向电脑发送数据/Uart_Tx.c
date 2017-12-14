#include<reg52.h>
/*
sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;  //Ŀǰ��������ò�����Щ����

code unsigned char Shu_Duan_Chose[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Shu_Wei_Chose[16]  = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
code unsigned char Led8_One[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char Led8_Two[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  */
unsigned char Given_Data = 0x55;

void Serial_Timer1_Init()
{
	TMOD = 0x20;		  //��ʱ��1���ù�����ʽ2����0��ʼ��������ܹ��Զ���װ
	SCON = 0X50;          //ѡ���п��ƼĴ����Ĺ�����ʽ��ѡ��Ϊ����ģʽ1,ʮ������Ϊ0B0101 0000����SM0 = 0��SM1 = 1,SM2 = 0,REN = 1������ģʽ1λ8λ�Ĵ������������ɶ�ʱ�������ʱ�����
	TH1 = 0xfd;          // 256 - 11059200/(12*32*9600)	����Ϊ������ʽ2����8λ������������2^8=256��32�ǵ�Ƭ���Ĳ����ʣ�12��һ��ָ�����ڣ���ʱ�����ڵ�12��
	TL1 = 0XFD;
	TR1 = 1;			 //TR1 = 1ʹ�ö�ʱ��1���Թ���
	ES = 1;				  //�������жϿ���	 ָ���Ǵ��ڵ��ж���
	//ET1 = 1;			 //��ʱ��1���жϿ���
	EA = 1;            //�ж������ܿ���λ
}

/*void Uart_Tx_Init()							//uart
{
	SCON = 0x50;
	PCON = 0X80;
	REN = 1;
}  */                  //�������������ݿ���ֱ��д�������Ǹ��������������ֿ�дҲ�ǿ��Եģ�Ҫ��һ��


void Delay1ms(int i)
{
	int j;
	for(;i > 0;i--)
	{
		for(j = 0;j < 110;j++);
	}
}

void Delay()
{
	unsigned char Num = 20000;
	while(Num--)
	{
	}
} 



void Send_Data(unsigned char Data)
{
	SBUF = Data;		
}



void main()
{
	//Send_Data(unsigned char Data);
	/*TMOD = 0X20;
	SCON = 0X50;
	TH1 = TL1 = 0xfd;                                 //256 - 11059200/(12*32*9600)
	EA = 1;
	ES = 1;
	TR1 = 1;
	ET1 = 1;
	Uart_Tx_Init();	 */     //��Щ��д�ڳ�ʼ���������ˣ�Ӧ����һ�Ե���д��main�����������
	//Delay1ms(int i);
	//Delay();
	
	
	Serial_Timer1_Init();
	
	while(1)
	{
		//SBUF = 0X55;	   //Ҫ����д�����ݷ��ͺ�������while��һֱִ��
		Send_Data(Given_Data++);	  //ÿѭ��һ��֮���1
		Delay1ms(1000);
		//Delay();
	}
}


void interrupt_Uart_Tx() interrupt 4	   //�ж��Ƿ������ݵ��жϣ�����������������������·���
{
	//EA = 1;
	//ES = 1;
	if(1 == TI)		   //TI ����Ƿ��ѷ������ݣ����ѷ��ͣ���TI�ᱻӲ����1��
						//����Ҫ�������0�������½��ŷ���
	{
		TI = 0;

	}
	if(1 == RI)
	{
		RI = 0;
	}
}

/*void interrupt_SendData() interrupt 3		//�����Ͳ���Ҫ�ĺ�����Ӧ��дһ����������
											�ĺ������������жϳ����﷢�ͺ���
											д�жϳ������Ϊ�˼���Ƿ��ѷ������ݣ������ͣ����TI�����ͱ�־λ���㣩
											�������ܼ�����������
											
{
	EA = 1;
	ES = 1;
	SBUF = 0x55;
} */