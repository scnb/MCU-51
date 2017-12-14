#include<reg51.h>
#include "include/hc595.h"
#include "include/delay.h"
#include "include/main.h"
#include "intrins.h"

//����һ����������������û�е�
void Delay(unsigned int tt)	  //������ʱ?
{
	while(tt--);          //��tt�Լ���0�Ǿ��˳�ѭ��
}

//�ٲ���һ����������Ҫ��һ����ʱ��
void Timer0_Init(unsigned int ms)
{
	unsigned long tmp;
	tmp = 11059200 / 12;		 //��������Ϊ���㶨ʱʱ�����㷨
	tmp =(tmp * ms) / 1000;
	tmp = 65536 - tmp;
	tmp = tmp + 18;         //�����ж���Ӧ��ʱ��ɵ����
	T0RH = (unsigned char)(tmp >> 8); //��ʱ������ֵ���Ϊ�ߵ��ֽ�,��ʵ������16������
	T0RL = (unsigned char)tmp;
	TMOD &= 0XF0;	 //��T0�Ŀ���λ(����λ)����
	TMOD |= 0X01;	 //��T0��ģʽ����Ϊ��ʽ1
	TH0 = T0RH;		 //����T0����ֵ����ʵ�������㷨��һ����
	TL0 = T0RL;		  //��ν����ֵ�����ǵ���ʱ����ʱʱ��������������ʱ�����¸�ֵ���ٴν��붨ʱ״̬
	ET0 = 1;   //��ʱ��0���ж�
	TR0 = 1;   //ʹ��ʱ��0����
		
}





unsigned char Scan_Key()
{
	unsigned char Key_Temp,Row_Temp,Col_Temp;    //���δ��P2��ֵ��16���ƣ�,����Ϣ������Ϣ
	P2 = 0xf0;	   //������λ��0
	Key_Temp = P2;
	if((Key_Temp&0xf0)^0xf0)   //�жϵ�����û�м������£���������ѵ���λ��0��
	                           //����������жϸ���λ�ǲ��Ƕ���1��ֻҪ����λ��һ��0��
							   //if�жϾ���ͨ��
	{
	 	Row_Temp = 0xfe;         //����ֵ���ܲ��ܸ��ɱ�Ŀ�ʼ�������ϵĲ���
		//���һ��һ�еļ�⣬Ӧ���õ���λ���ж�����һ��
		while((Row_Temp&0x10) != 0x00)  //�ڵ�һ�е������м�ѭ�����ж��ǲ������������ڣ������һ��0B1111 1110,��0B0001 0000ȡ�����㣬�õ�0B0001 0000��Ȼ���Key_Temp�����ĴΣ�ֱ��������0B1111 0111��ȡ�����㻹��0x10��������һ�α��0B1110 1111����ʱȡ����������0x00
		{
			P2 = Row_Temp;		 //P2����������������ģ�����Ӳ�������ģ�
			if((P2&0xf0)^0xf0)	 //P2�̳������ڵ��е���Ϣ
			{
				Col_Temp = (P2&0xf0)|0x0f; //�����㱣����P2�ĸ���λ�����������У��Ǹ�λ�þ���0(ȡ�����Ϊ1)������ʹ����λ��1������ȡ��
				return ((~Col_Temp) + (~Row_Temp));	
			}
			else
			{
				Row_Temp = (Row_Temp << 1) | 0x01;	   //����ֵ������Ϊ�����ҽ��ұ߲���0ȡ����1
			}
			Delay(10);
		}
	}
	else
	{
		return (0);
	}
}




void Interrupt() interrupt 1 //�õ��Ƕ�ʱ��0�ж�
{
	static unsigned char k = 0;
	//static unsigned char i;
	
	TH0 = T0RH;
	TL0 = T0RL;
	
	Send_Data(Display[0],Shu_Wei_Chose[k]);
	if(k < 7)
	{										//����һ�γ������ε�֮�󣬾�ֻ�е�һλ�������ʾ
		//i++;
		k++;
	}	
	
	else									 //��λѡ����k֮��8λ�����һͬ��ʾ
	{
		//i = 0;
		k = 0;
	} 
}







void main()
{
	//unsigned int Key_Value;
	unsigned char Key_Data;
	P0 = 0xff;	 //ΪʲôҪ��P0��P1���ó�0xff
	P1 = 0xff;   
	Delay(10);
	Timer0_Init(1);
	EA = 1;
	P2_4 = 0;   //ΪʲôҪ�����趨P2^4Ϊ0�أ�
	while(1)
	{
		Key_Data = Scan_Key();
		if(Key_Data != 0)
		{
			switch(Key_Data)
			{
				case 0x11:Key_Value = 0;break;
				case 0x21:Key_Value = 1;break;
				case 0x41:Key_Value = 2;break;
				case 0x81:Key_Value = 3;break;		

				case 0x12:Key_Value = 4;break;
				case 0x22:Key_Value = 5;break;
				case 0x42:Key_Value = 6;break;
				case 0x82:Key_Value = 7;break;

				case 0x14:Key_Value = 8;break;
				case 0x24:Key_Value = 9;break;
				case 0x44:Key_Value = 10;break;
				case 0x84:Key_Value = 11;break;

				case 0x18:Key_Value = 12;break;
				case 0x28:Key_Value = 13;break;
				case 0x48:Key_Value = 14;break;
				case 0x88:Key_Value = 15;break;
			}
		}
		Key_Data = 0;    //��Key_Data���㣬��ֹ�ظ�����if
		//Send_Data(Shu_Duan_Chose[Key_Value],0xfe);
		Display[0] = Shu_Duan_Chose[Key_Value];
	}
}