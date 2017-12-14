#include<reg52.h>

unsigned char Tx_Char[] = "shangchao\n";

void Scon_Timer1_Init()		        //SCON�Ͷ�ʱ��1��ʼ��
{
	TMOD = 0X20;	//���ö�ʱ��1������ʽΪ2
	SCON = 0X50;	//���ô��п��ƼĴ���������ʽΪ1
	TH1 = 0XFD;	   //����ʱ��1ѡ������ʽ1ʱ��������8λ�ļ�������һ����TH1,һ����TL1
	TL1 = 0XFD;	   //��������������������Ȼ�����ǿ�ʼ���������Ӷ��ﵽ��ʱ��Ŀ��
	TR1 = 1;			 //���ƶ�ʱ��1�������
}

void Delay1ms(int i)					 //��ʱ����
{
    int j;
    for(; i > 0; i--)
    {
         for(j = 0; j < 110; j++);
    }
}

void Byte_Send(unsigned char byte)
{
	//SBUF = byte;
	TI = 0;
	while(TI == 0)		  //��һ��ǳ���Ҫ,��ʹ���ַ�����һ��һ��(����)�����������һ�����
	{
		SBUF = byte;
	}
	//TI = 0;
}

void String_Send(unsigned char *Tx_Data)
{	  
	while(*Tx_Data != '\0')
	{
		Byte_Send(*Tx_Data++);
	}
}


void main()							 //������
{						 
	Scon_Timer1_Init();
	while(1)
	{
		String_Send(Tx_Char);
		Delay1ms(1000);
	}
}



