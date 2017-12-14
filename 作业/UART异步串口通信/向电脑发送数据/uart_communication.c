#include<reg52.h>

/*��������Է��͵�����*/
char *s = "shangchao";


void Delay(int i)
{
 	unsigned char j;
	for(;i>=0;i--)
	{
	 	for(j = 0;j < 110;j++)
		{
		}
	}
}

/*�������ݵĺ���*/
/*void Send_uart(char string[])
{
	unsigned char i = 0;
 	for(i = 0;i < 9;i++)
	{
	 	SBUF = string[i];
		while(!TI);
		Delay(500);
		TI = 0;
	}
} */

/*ָ����ʽ*/
void Send_uart(char *string)
{
	unsigned char i = 0;
 	while(*(string+i) != '\0')
	{
	 	SBUF = string[i];
		while(!TI);
		Delay(1000);// û���ر����˼��ֻ������ÿ���ַ�֮�䷢�͵���һЩ���Ӷ���������������ͬʱ��������
		i++;
		TI = 0;
	}
}


/*��ʱ����ʼ��  �жϳ�ʼ��    ���ڿ��ƼĴ���SCON��ʼ��*/
void UartInit(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
//	AUXR &= 0xBF;		//��ʱ��1ʱ��ΪFosc/12,��12T
//	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xFD;		//�趨��ʱ��ֵ
	TH1 = 0xFD;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	EA = 1;
	ES = 1;
}




void main()
{
 	UartInit();
	while(1)
	{
		TI = 0;
	 	Send_uart(s);
		Delay(1000);
	}
}


void interrupt_4() interrupt 4
{
 	TI = 0;
}