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
	TR0 = 1;		  //使定时器0开始运行
	while(!TF0);
	TR0 = 0;          //关闭定时器0
	TH0 = 0xff;
	TL0 = 0xbb;
	TF0 = 0;			//T0溢出标志位
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
		TF0 = 0;    //准备工作，溢出标志位清零，在每一次循环操作之前都要清零
		Send_Data(0);  //发送起始位	在Send_Data()函数里自带最后的溢出标志位清零，但是当第一次使用这个函数时，就需要单独的在前面加上一句清零
		a = Send_Value[0];
		for(i = 0;i < 8;i++)
		{
			Send_Data((bit)(a&0x01));
			a = a >> 1;  //使a右移，因为是从低位向高位发，所以每一次都从最右边与0x01相与，是1则1，是0则0
		}
		Send_Data(1);  //发送停止位
		Delay(2000);	
	}
	return 0;
}