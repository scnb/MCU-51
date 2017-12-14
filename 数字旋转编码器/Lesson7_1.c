#include<reg52.h>
/*�ԼĴ������趨*/
sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;
/*��������ת�����������������趨*/
sbit B_Key = P2^1;							 //��ת�����������ӿڶ���
sbit A_Key = P2^0;
sbit Down_Key = P2^2;

code unsigned char Shu_Ma_Duan[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
code unsigned char Shu_Ma_Wei[8] = {0xfe,0xfd,0xfb,0xf7,0x0f,0xdf,0xbf,0x7f};
unsigned char Show[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};        //Ӧ������һ�԰Ѱ˸�ֵ���ó�0x00
//����ʵ��õ���Show���ֵ���ĳ�0x00Ҳ�ǿ��Ե�
unsigned char Last_B_Value,Current_B_Value;
unsigned char Down_Flag = 0;			//��ʱû�м����	   
unsigned char Down_Key_Value = 0;		//��ʱû�м����	   //��¼����3��ֵ������ת����������ʱ��ֵ��1
//������û����һ��unsigned char Counter = 0;
//������û����һ��unsigned char k = 0;

void Send_Data(unsigned char Shu_Duan,unsigned char Shu_Wei);
//void Data_Of_Key();
void Time0_Init()			  //��ʱ��0��ʼ��
{
	TMOD = 0X01;
	TH0 = 0XF8;
	TL0 = 0XF0;                //������õ���2msһ�� ��ʵԼ��1.96ms
	ET0 = 1;
	EA = 1;
	TR0 = 1;                 //��ʱ���˼���һ�䣬����Ҫ��TR0(TR1)��1
}

void Display()
{
	static unsigned char i;
	Show[0] = Shu_Ma_Duan[Down_Key_Value/100];
	Show[1] = Shu_Ma_Duan[Down_Key_Value%100/10];
	Show[2] = Shu_Ma_Duan[Down_Key_Value%10];
	if(i < 2)					//��ֻ֤������ܵ�ǰ��λ����0,1,2��λ��ʾ����
	{
		i++;
	}
	else
	{
		i = 0;
	}
	Send_Data(Show[i],Shu_Ma_Wei[i]);
}


void Send_Data(unsigned char Shu_Duan,unsigned char Shu_Wei)
{
    unsigned char i;
	HC595_RCK = 0;
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
		if(0 != (Shu_Duan&0x80))
		{
			HC595_DATA = 1;
		}
		else
		{
			HC595_DATA = 0;
		}
		HC595_SCK = 1;
		Shu_Duan = Shu_Duan << 1;
	}
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
		if(0 != (Shu_Wei&0x80))
		{
			HC595_DATA = 1;
		}
		else
		{
			HC595_DATA = 0;
		}
		HC595_SCK = 1;
		Shu_Wei = Shu_Wei << 1;
	}
	HC595_RCK = 1;
}

void Data_Of_Key()
{
	Last_B_Value = B_Key;
	while(!A_Key)
	{
		Current_B_Value = B_Key;
		Down_Flag = 1;
	}
	if(1 == Down_Flag)                          //��ʵDown_Flag�����þ�����if��䵱�е��ж�   
	{
		Down_Flag = 0;                 //���°�Down_Flag���㣬�����ظ�����if���
		//�����while�����   Current_B_Value = B_Key;
		if((0 == Last_B_Value)&&(1 == Current_B_Value))      //���������Ƿ�˳ʱ��ת��
		{
			if(Down_Key_Value < 255)
			{
				Down_Key_Value++;
			}
			else
			{
				Down_Key_Value = 0;
			}
		}
		if((1 == Last_B_Value)&&(0 == Current_B_Value))	   //���������Ƿ���ʱ��ת��
		{
			if(Down_Key_Value > 0)
			{
				Down_Key_Value--;
				
		    }
			else
			{
				Down_Key_Value = 255;
			}
		}
	}
}

void Timer0_interrupt()  interrupt 1     //�����ж�,��ʱҲ����д����жϺ���
{
	//����һ���ж�Ӧ����ʲô��������½���
	//IT0 = 0;		           //����������Ѿ��ڶ�ʱ�������ﶨ����
	//TR0 = 1;	
	TH0 = 0XF8;				   //ÿ���ڽ����жϵ�ʱ��Ҫ��TH0 ��TL0���¶���һ�£���ֵֹ���޸�
	TL0 = 0XF0;				   //2ms
	Display();
	if(!Down_Key)   //�Ѿ��˽�          //����������жϵ����ã���ʵ�ǲ��˽�Down_Key�����������ʲô����µ���1��ʲô����µ���0
	{
		Down_Key_Value = 0;	   //����ʵ���Լ������ɵã�������ʱΪ0�����Դ�ʱ����if��䣬�Ѹ�ֵ���1
	}
}	



void main()						   //������
{
	Time0_Init();
	while(1)
	{
		Data_Of_Key();	
	}

}




	
		
	
	
	
	
	
	






