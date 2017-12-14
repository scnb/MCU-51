/*�ó�����A��Ϊ�ߵ�ƽ��1��Ϊ��׼��ͨ�����B����һ���뵱ǰ��״̬�仯�����ж���ת��������˳ʱ�뻹����ʱ����ת
����Ӧ�����ӻ���ټ���ֵ����ͨ��display����������ֵ��������϶�̬ɨ����ʾ*/

#include<reg52.h>
/*��ת����������������IO�ڵ�����*/
sbit press_down = P2^2; //����
sbit pin_A = P2^0;	   //A��
sbit pin_B = P2^1;	   //B��

/*16��led�ļĴ���������*/
sbit HC595_SCK = P0^5;
sbit HC595_RCK = P0^6;
sbit HC595_DATA = P0^7;

unsigned char Last_B_value;//��¼B����һ�ε�״̬����0����1
unsigned char Current_B_value;//��¼B�ŵ�ǰ��״̬����0����1
unsigned char Rotate_flag;//����Ƿ���ת��  Rotate����˼����ת�����Կ������Ϊ��ת��־λ
unsigned char value = 0;//��¼����ֵ

/*16��led��Ƭѡ��*/
code unsigned char Led_One[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
code unsigned char Led_Two[16] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*�������ݺ�������*/
//void Send_Data(unsigned char Shu_Duan,unsigned char Shu_Wei);

/*�������ݺ���*/
void Send_data(unsigned char Shu_Duan,unsigned char Shu_Wei)
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

/*������ֵͨ������ܶ�̬ɨ����ʾ�ĺ���*/
void display()
{
	Send_data(Led_One[15-value],Led_Two[15-value]);
}

/*��ʱ��0�ĳ�ʼ������*/
void Timer0Init(void)		//1����@11.0592MHz
{
	//AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xDC;
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	EA = 1;
	ET0 = 1;
}


void Rotate_encoder()
{
 	Last_B_value = pin_B;
	while(!pin_A)	 //ע����while������if,��pin_A��1ʱ������ѭ��,��Ϊ��A��Ϊ�ߵ�ƽΪ��׼
	{
	 	Rotate_flag = 1;
		Current_B_value = pin_B;
	}
	if(1 == Rotate_flag)//�����ⲿif���ж��Ƿ���ת�������ڲ�����if�зֱ��ж���˳ʱ�뻹����ʱ��
	{
		Rotate_flag = 0;
	 	if((Last_B_value==0)&&(Current_B_value==1))
		{
		 	if(value < 15)
			{
			 	value++;
			}
			else
			{
			 	value = 0;
			}
		}
	 	if((Last_B_value==1)&&(Current_B_value==0))
		{
		 	if(value > 0)
			{
			 	value--;
			}
			else
			{
			 	value = 15;
			}
		}
	}
}

/*������*/
void main()
{
	Timer0Init();
	while(1)
	{
	 	Rotate_encoder();
	}
}

/*��ʱ��0���жϺ���*/
void interrupt_1() interrupt 1
{
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xDC;
	display();
 	if(!press_down)
	{
		value = 0;
	}
}