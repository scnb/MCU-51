#include<reg52.h>
/*对寄存器的设定*/
sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;
/*对数字旋转编码器的三个键的设定*/
sbit B_Key = P2^1;							 //旋转编码器三个接口定义
sbit A_Key = P2^0;
sbit Down_Key = P2^2;

code unsigned char Shu_Ma_Duan[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
code unsigned char Shu_Ma_Wei[8] = {0xfe,0xfd,0xfb,0xf7,0x0f,0xdf,0xbf,0x7f};
unsigned char Show[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};        //应该再试一试把八个值都置成0x00
//经过实验得到把Show里的值都改成0x00也是可以的
unsigned char Last_B_Value,Current_B_Value;
unsigned char Down_Flag = 0;			//当时没有加这句	   
unsigned char Down_Key_Value = 0;		//当时没有加这句	   //记录按键3的值，当旋转编码器按下时把值置1
//根本就没有这一句unsigned char Counter = 0;
//根本就没有这一句unsigned char k = 0;

void Send_Data(unsigned char Shu_Duan,unsigned char Shu_Wei);
//void Data_Of_Key();
void Time0_Init()			  //定时器0初始化
{
	TMOD = 0X01;
	TH0 = 0XF8;
	TL0 = 0XF0;                //经计算得到是2ms一次 其实约是1.96ms
	ET0 = 1;
	EA = 1;
	TR0 = 1;                 //当时忘了加这一句，必须要把TR0(TR1)置1
}

void Display()
{
	static unsigned char i;
	Show[0] = Shu_Ma_Duan[Down_Key_Value/100];
	Show[1] = Shu_Ma_Duan[Down_Key_Value%100/10];
	Show[2] = Shu_Ma_Duan[Down_Key_Value%10];
	if(i < 2)					//保证只有数码管的前三位，即0,1,2三位显示数字
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
	if(1 == Down_Flag)                          //其实Down_Flag的作用就是在if语句当中的判断   
	{
		Down_Flag = 0;                 //重新把Down_Flag清零，以免重复进入if语句
		//这句在while语句里   Current_B_Value = B_Key;
		if((0 == Last_B_Value)&&(1 == Current_B_Value))      //检测编码器是否顺时针转动
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
		if((1 == Last_B_Value)&&(0 == Current_B_Value))	   //检测编码器是否逆时针转动
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

void Timer0_interrupt()  interrupt 1     //设置中断,当时也忘了写这个中断函数
{
	//考虑一下中断应该再什么样的情况下进行
	//IT0 = 0;		           //这两个语句已经在定时器函数里定义了
	//TR0 = 1;	
	TH0 = 0XF8;				   //每次在进入中断的时候都要把TH0 和TL0重新定义一下，防止值被修改
	TL0 = 0XF0;				   //2ms
	Display();
	if(!Down_Key)   //已经了解          //不明白这句判断的作用，其实是不了解Down_Key这个参数是在什么情况下等于1，什么情况下等于0
	{
		Down_Key_Value = 0;	   //根据实验以及分析可得，当按下时为0，所以此时进入if语句，把该值变成1
	}
}	



void main()						   //主函数
{
	Time0_Init();
	while(1)
	{
		Data_Of_Key();	
	}

}




	
		
	
	
	
	
	
	






