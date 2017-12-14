#include<reg52.h>
sbit HC595_SCK = P0^5;
sbit HC595_RCK = P0^6;
sbit HC595_DATA = P0^7;

code unsigned char Led_One[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
code unsigned char Led_Two[16] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void Send_data(unsigned char Led8_one,unsigned char Led8_two)
{
	unsigned char i;
 	HC595_RCK = 0;	 //并行输出控制线，先给成低电平，等最后数据都传完了，给高电平从Q0-Q7并行输出
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0; //移位控制线，高电平触发，即从0变1时触发
	 	if(0 != (Led8_one&0x80))
		{
		 	HC595_DATA = 1;	 //数据线，根据实际情况，给高电平或低电平
		}
		else
		{
		 	HC595_DATA = 0;
		}
		Led8_one = Led8_one << 1;
		HC595_SCK = 1;	 //将HC595_DATA的数据向低位移动
	}
	for(i = 0;i < 8;i++)
	{
		HC595_SCK = 0;
	 	if(0 != (Led8_two&0x80))
		{
		 	HC595_DATA = 1;
		}
		else
		{
		 	HC595_DATA = 0;
		}
		Led8_two = Led8_two << 1;
		HC595_SCK = 1;
	}
	HC595_RCK = 1;
}		

void Delay_1ms(int i)
{
 	int j;
	for(;i > 0;i--)
	{
	 	for(j = 0;j < 110;j++)
		{}
	}
}		  	

void main()
{
	int k;		   //原来是unsigned char k，这样是错误的，因为k是无符号型的时候，
	while(1)	   //在执行第二个for循环的时候，最后一次k=1，k=k-2，得到k=-1，这与无符号相矛盾导致后面的循环不能执行吗，一直卡在第二个for循环
	{			   //当k成为负值时，便会取其补码，总是正的，卡在第二个for循环里出不来
		//用for循环来简化程序
		for(k = 15;k >= 9;)	 //1,3,5,7
		{
		 	Send_data(Led_One[k],Led_Two[8]);
			Delay_1ms(1000);
			k = k-2;
		}				  
		for(k = 7;k >= 1;)		 //9,11,13,15
		{
		 	Send_data(Led_One[0],Led_Two[k]);
			Delay_1ms(1000);  
			k = k-2;
		} 
		for(k = 14;k >= 8;)     //2,4,6,8
		{
		 	Send_data(Led_One[k],Led_Two[8]);
			Delay_1ms(1000);
			k = k-2;
		}
		for(k = 6;k >= 0;)		//10,12,14,16
		{
		 	Send_data(Led_One[0],Led_Two[k]);
			Delay_1ms(1000);
			k = k-2;
		}                                       
		/*Send_data(Led_One[15],Led_Two[8]); //1
		Delay_1ms(1000);
		Send_data(Led_One[13],Led_Two[8]);	  //3
		Delay_1ms(1000);
		Send_data(Led_One[11],Led_Two[8]);		 //5
		Delay_1ms(1000);
		Send_data(Led_One[9],Led_Two[8]);			//7
		Delay_1ms(1000);
		Send_data(Led_One[0],Led_Two[7]);			   //9
		Delay_1ms(1000);
		Send_data(Led_One[0],Led_Two[5]);				  //11
		Delay_1ms(1000);
		Send_data(Led_One[0],Led_Two[3]);					 //13
		Delay_1ms(1000);
		Send_data(Led_One[0],Led_Two[1]);						 //15
		Delay_1ms(1000);
		Send_data(Led_One[14],Led_Two[8]);	 //2
		Delay_1ms(1000);
		Send_data(Led_One[12],Led_Two[8]);	//4
		Delay_1ms(1000);
		Send_data(Led_One[10],Led_Two[8]);	//6
		Delay_1ms(1000);
		Send_data(Led_One[8],Led_Two[8]);	//8
		Delay_1ms(1000);
		Send_data(Led_One[0],Led_Two[6]);	//10
		Delay_1ms(1000);
		Send_data(Led_One[0],Led_Two[4]);  //12
		Delay_1ms(1000);
		Send_data(Led_One[0],Led_Two[2]);	//14
		Delay_1ms(1000);
		Send_data(Led_One[0],Led_Two[0]);  //16
		Delay_1ms(1000);  */
	}
}