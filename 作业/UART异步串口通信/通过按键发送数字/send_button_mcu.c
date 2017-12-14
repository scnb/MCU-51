#include<reg52.h>



/*���̶�ӦIO������*/
unsigned char temp;//�����洢P2�ڵ�״̬
static unsigned char value = 5;//������¼����һ�����̱�����,ͬʱҲ��Ҫ����Է��͵�����
static unsigned char pre_value;


/*����ʱ����*/
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




/*�������ɨ�躯��*/
void matrix_button_scan_uart_send()
{
	/*��һ��*/
	P2 = 0xfe;
	temp = P2;
	temp = temp & 0x0f;
	if(0x0f != temp)
	{
		Delay(10);
		temp = P2;
		temp = temp & 0x0f;
		if(0x0f != temp)
		{
			temp = P2;
			switch(temp)
			{
				case 0xee:value = 0;break;
			 	case 0xde:value = 1;break;
				case 0xbe:value = 2;break;
				case 0x7e:value = 3;break;
			}
		}
	}
	/*��  ��  ��*/
	P2 = 0xfd;
	temp = P2;
	temp = temp & 0x0f;
	if(0x0f != temp)
	{
		Delay(10);
		temp = P2;
		temp = temp & 0x0f;
		if(0x0f != temp)
		{
			temp = P2;
			switch(temp)
			{
				case 0xed:value = 4;break;
			 	case 0xdd:value = 5;break;
				case 0xbd:value = 6;break;
				case 0x7d:value = 7;break;
			}
		}
	}
	/*��  ��  ��*/
	P2 = 0xfb;
	temp = P2;
	temp = temp & 0x0f;
	if(0x0f != temp)
	{
		Delay(10);
		temp = P2;
		temp = temp & 0x0f;
		if(0x0f != temp)
		{
			temp = P2;
			switch(temp)
			{
				case 0xeb:value = 8;break;
			 	case 0xdb:value = 9;break;
				case 0xbb:value = 10;break;
				case 0x7b:value = 11;break;
			}
		}
	}
	/*��  ��  ��*/
	P2 = 0xf7;
	temp = P2;
	temp = temp & 0x0f;
	if(0x0f != temp)
	{
		Delay(10);
		temp = P2;
		temp = temp & 0x0f;
		if(0x0f != temp)
		{
			temp = P2;
			switch(temp)
			{
				case 0xe7:value = 12;break;
			 	case 0xd7:value = 13;break;
				case 0xb7:value = 14;break;
				case 0x77:value = 15;break;
			}
		}
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
		pre_value = value;
		while(1)
		{
	 		matrix_button_scan_uart_send();
			if(pre_value != value)
			{
				break;
			}
		}
		if(pre_value != value)//2016/11/21�賿һ��26�֣����ڰ��������㶨�ˣ�Ҫ�밴һ�ΰ���������һ�����ݣ��Ͳ������ж����������ݣ���ֻ����if�жϣ�value��ֵ�Ƿ�ı��ˣ�Ŀǰ����ʹ���жϵ�ԭ��δ֪
		{
		 	SBUF = value;	  //ע�ⷢ�����ݵ�ʱ�����ж��ǲ����ʵģ���ΪTIֻ���������Ѿ�����SBUF��ŻᱻӲ����1�����Լ�ʹ���жϵĻ�ֻ�����������TI=1���ܽ����жϣ������ǲ����ʵ�
			while(!TI);   //�ڽ������ݵ�ʱ��ʹ���ж���˳������ģ���Ϊ���ⲿ���ݷ��͹��������浽SBUF�к�RI����1�������жϣ�ͨ���������ݽ��ձ�־λ��Ȼ���������ĺ�����ͨ���ж����ݽ��ձ�־λ��ֵ�������Ƿ�ִ����Ӧ�ĳ���
			TI = 0;
		}
	}
}

/*void interrupt_4() interrupt 4
{
 	SBUF = value;
	while(0 == TI);
}
*/