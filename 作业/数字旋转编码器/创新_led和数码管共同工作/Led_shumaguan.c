/*该程序以A脚为高电平（1）为基准，通过检测B脚上一次与当前的状态变化，来判断旋转编码器是顺时针还是逆时针旋转
并相应地增加或减少计数值，并通过display函数将计数值在数码管上动态扫描显示*/

#include<reg52.h>
/*旋转编码器三个引脚与IO口的设置*/
sbit press_down = P2^2; //按下
sbit pin_A = P2^0;	   //A脚
sbit pin_B = P2^1;	   //B脚

/*数码管寄存器的设置*/
sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;


/*16个led的寄存器的设置*/
sbit HC595_SCK1 = P0^5;
sbit HC595_RCK1 = P0^6;
sbit HC595_DATA1 = P0^7;


unsigned char Last_B_value;//记录B脚上一次的状态，是0还是1
unsigned char Current_B_value;//记录B脚当前的状态，是0还是1
unsigned char Rotate_flag;//检测是否旋转了  Rotate的意思是旋转，所以可以理解为旋转标志位
unsigned char value_shumaguan = 0;//记录计数值
unsigned char value_led = 0;

/*数码管段选和位选表*/
code unsigned char Shu_Ma_Duan[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; //0--9
code unsigned char Shu_Ma_Wei[8] = {0xfb,0xfd,0xfe,0xf7,0x0f,0xdf,0xbf,0x7f};//注意该位码表第一个和第三个变化了位置，只是程序的特殊需要
unsigned char display_arry[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*16个led的片选表*/
code unsigned char Led_One[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
code unsigned char Led_Two[16] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*发送数据函数声明*/
//void Send_Data(unsigned char Shu_Duan,unsigned char Shu_Wei);

/*发送数据函数*/
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

/*led的数据发送函数*/
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

/*将计数值通过数码管动态扫描显示的函数*/
void display()
{
 	static unsigned char i;
	display_arry[0] = Shu_Ma_Duan[value_shumaguan%10];  //因为display_arry数组需要改变值，所以不能定义成code类型
	display_arry[1] = Shu_Ma_Duan[value_shumaguan%100/10];
	display_arry[2] = Shu_Ma_Duan[value_shumaguan/100];
	if(i < 2)
	{
	 	i++;
	}
	else
	{
	 	i = 0;//这一个语句还承担着第一次为i赋值的功能，因为之前只定义了i但是并没有赋值
	}
	Send_data_shumaguan(display_arry[i],Shu_Ma_Wei[i]);
	Send_data_led(Led_One[15-value_led],Led_Two[15-value_led]);
}

/*定时器0的初始化程序     必须要把定时器的值调到完美的状态，即没旋转一下编码器，数码管和led瞬时实时响应，经过多次调试发现设定为1ms时最合适*/
void Timer0Init(void)		//1毫秒@11.0592MHz	  
{
	//AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA = 1;
	ET0 = 1;
}




void Rotate_encoder()
{
 	Last_B_value = pin_B;
	while(!pin_A)	 //注意是while而不是if,当pin_A是1时，跳出循环,因为以A脚为高电平为基准
	{
	 	Rotate_flag = 1;
		Current_B_value = pin_B;
	}
	if(1 == Rotate_flag)//先在外部if中判断是否旋转，再在内部两个if中分别判断是顺时针还是逆时针
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

/*主函数*/
void main()
{
	Timer0Init();
	while(1)
	{
	 	Rotate_encoder();
	}
}

/*定时器0的中断函数*/
void interrupt_1() interrupt 1
{
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	display();
 	if(!press_down)
	{
		value_shumaguan = 0;
		value_led = 0;
	}
}