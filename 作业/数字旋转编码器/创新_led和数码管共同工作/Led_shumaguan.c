/*�ó�����A��Ϊ�ߵ�ƽ��1��Ϊ��׼��ͨ�����B����һ���뵱ǰ��״̬�仯�����ж���ת��������˳ʱ�뻹����ʱ����ת
����Ӧ�����ӻ���ټ���ֵ����ͨ��display����������ֵ��������϶�̬ɨ����ʾ*/

#include<reg52.h>
/*��ת����������������IO�ڵ�����*/
sbit press_down = P2^2; //����
sbit pin_A = P2^0;	   //A��
sbit pin_B = P2^1;	   //B��

/*����ܼĴ���������*/
sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;


/*16��led�ļĴ���������*/
sbit HC595_SCK1 = P0^5;
sbit HC595_RCK1 = P0^6;
sbit HC595_DATA1 = P0^7;


unsigned char Last_B_value;//��¼B����һ�ε�״̬����0����1
unsigned char Current_B_value;//��¼B�ŵ�ǰ��״̬����0����1
unsigned char Rotate_flag;//����Ƿ���ת��  Rotate����˼����ת�����Կ������Ϊ��ת��־λ
unsigned char value_shumaguan = 0;//��¼����ֵ
unsigned char value_led = 0;

/*����ܶ�ѡ��λѡ��*/
code unsigned char Shu_Ma_Duan[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; //0--9
code unsigned char Shu_Ma_Wei[8] = {0xfb,0xfd,0xfe,0xf7,0x0f,0xdf,0xbf,0x7f};//ע���λ����һ���͵������仯��λ�ã�ֻ�ǳ����������Ҫ
unsigned char display_arry[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*16��led��Ƭѡ��*/
code unsigned char Led_One[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
code unsigned char Led_Two[16] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*�������ݺ�������*/
//void Send_Data(unsigned char Shu_Duan,unsigned char Shu_Wei);

/*�������ݺ���*/
void Send_data_shumaguan(unsigned char Shu_Duan,unsigned char Shu_Wei)
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

/*led�����ݷ��ͺ���*/
void Send_data_led(unsigned char Led_one,unsigned char Led_two)
{
    unsigned char i;
	HC595_RCK1 = 0;
	for(i = 0;i < 8;i++)
	{
		HC595_SCK1 = 0;
		if(0 != (Led_one&0x80))
		{
			HC595_DATA1 = 1;
		}
		else
		{
			HC595_DATA1 = 0;
		}
		HC595_SCK1 = 1;
		Led_one = Led_one << 1;
	}
	for(i = 0;i < 8;i++)
	{
		HC595_SCK1 = 0;
		if(0 != (Led_two&0x80))
		{
			HC595_DATA1 = 1;
		}
		else
		{
			HC595_DATA1 = 0;
		}
		HC595_SCK1 = 1;
		Led_two = Led_two << 1;
	}
	HC595_RCK1 = 1;
}

/*������ֵͨ������ܶ�̬ɨ����ʾ�ĺ���*/
void display()
{
 	static unsigned char i;
	display_arry[0] = Shu_Ma_Duan[value_shumaguan%10];  //��Ϊdisplay_arry������Ҫ�ı�ֵ�����Բ��ܶ����code����
	display_arry[1] = Shu_Ma_Duan[value_shumaguan%100/10];
	display_arry[2] = Shu_Ma_Duan[value_shumaguan/100];
	if(i < 2)
	{
	 	i++;
	}
	else
	{
	 	i = 0;//��һ����仹�е��ŵ�һ��Ϊi��ֵ�Ĺ��ܣ���Ϊ֮ǰֻ������i���ǲ�û�и�ֵ
	}
	Send_data_shumaguan(display_arry[i],Shu_Ma_Wei[i]);
	Send_data_led(Led_One[15-value_led],Led_Two[15-value_led]);
}

/*��ʱ��0�ĳ�ʼ������     ����Ҫ�Ѷ�ʱ����ֵ����������״̬����û��תһ�±�����������ܺ�led˲ʱʵʱ��Ӧ��������ε��Է����趨Ϊ1msʱ�����*/
void Timer0Init(void)		//1����@11.0592MHz	  
{
	//AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x66;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
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
		 	if(value_led < 15)
			{
			 	value_led++;
			}
			else
			{
			 	value_led = 0;
			}
			if(value_shumaguan < 255)
			{
			 	value_shumaguan++;
			}
			else
			{
			 	value_shumaguan = 0;
			}
		}
	 	if((Last_B_value==1)&&(Current_B_value==0))
		{
		 	if(value_led > 0)
			{
			 	value_led--;
			}
			else
			{
			 	value_led = 15;
			}
			if(value_shumaguan > 0)
			{
			 	value_shumaguan--;
			}
			else
			{
			 	value_shumaguan = 255;
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
	TL0 = 0x66;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	display();
 	if(!press_down)
	{
		value_shumaguan = 0;
		value_led = 0;
	}
}