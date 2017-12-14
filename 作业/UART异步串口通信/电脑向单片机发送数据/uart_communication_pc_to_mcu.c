 #include<reg52.h>

/*����ӵ��Խ��յ����ݴ洢����*/
unsigned char receive_data;//ֻ�������֣�Ȼ�������������ʾ
unsigned char receive_flag = 0;//���ճɹ���־λ

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

sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;

code unsigned char Duan_choose[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
code unsigned char Wei_choose[8] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};


void Send_data(unsigned char duan,unsigned char wei)
{
	unsigned char i;	   //������hc595��λ�Ĵ��������в���������������ܵ�ԭ����ʱ����д����
 	HC595_RCK = 0;
	for(i = 0;i < 8;i++)
	{
	 	HC595_SCK = 0;
		if(0 != (duan&0x80))
		{
		 	HC595_DATA = 1;
		}
		else
		{
		 	HC595_DATA = 0;
		}
		HC595_SCK = 1;
		duan = duan << 1;
	}
	for(i = 0;i < 8;i++)
	{
	 	HC595_SCK = 0;
		if(0 != (wei&0x80))
		{
		 	HC595_DATA = 1;
		}
		else 
		{
		 	HC595_DATA = 0;
		}
		HC595_SCK = 1;
		wei = wei << 1;
	}
	HC595_RCK = 1;
}



/*�ӵ��Խ������ݵĺ���*/
void Receive_fun()
{
	if(1 == receive_flag) //��while�в�ͣ�ļ��receive_flag�Ƿ�Ϊ1,��Ϊ1��ʱ��˵���Ѿ�������жϣ����ҽ����жϵ�������SBUF�Ѿ����յ�����
	{
	 	receive_flag = 0;
		Send_data(Duan_choose[receive_data],Wei_choose[0]);
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
		Receive_fun();
	}
}


void interrupt_4() interrupt 4
{
	Send_data(Duan_choose[0],Wei_choose[0]);
 	RI = 0;
	receive_data = SBUF;
	//Send_data(Duan_choose[receive_data],Wei_choose[0]);
	receive_flag = 1;
}