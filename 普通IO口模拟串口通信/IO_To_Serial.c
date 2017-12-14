#include<reg52.h>
unsigned char Send_Value[1] = {0x59};

void Send_Data(bit i)
{
	if(0 != i)
	{
		TXD = 1;
	}
	else
	{
		TXD = 0;
	}
	TR0 = 1;		  //ʹ��ʱ��0��ʼ����
	while(!TF0);
	TR0 = 0;          //�رն�ʱ��0
	TH0 = 0xff;
	TL0 = 0xbb;
	TF0 = 0;			//T0�����־λ
}

void Delay(unsigned int i)
{
	unsigned char j;
	for(;i > 0;i--)
	{
		for(j = 0;j < 110;j++);
	}
}




int main()
{
	unsigned char i;
	unsigned char a = 0;
	TMOD |= 0x01;
	TH0 = 0XFF;
	TL0 = 0XBB;
	TXD = 1;
	while(1)
	{
		TF0 = 0;    //׼�������������־λ���㣬��ÿһ��ѭ������֮ǰ��Ҫ����
		Send_Data(0);  //������ʼλ	��Send_Data()�������Դ����������־λ���㣬���ǵ���һ��ʹ���������ʱ������Ҫ��������ǰ�����һ������
		a = Send_Value[0];
		for(i = 0;i < 8;i++)
		{
			Send_Data((bit)(a&0x01));
			a = a >> 1;  //ʹa���ƣ���Ϊ�Ǵӵ�λ���λ��������ÿһ�ζ������ұ���0x01���룬��1��1����0��0
		}
		Send_Data(1);  //����ֹͣλ
		Delay(2000);	
	}
	return 0;
}