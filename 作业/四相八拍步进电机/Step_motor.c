#include <reg52.h>	
#include "intrins.h"



void delay()
{	
	unsigned int i = 200;
	while(i--);
}

//步进电机驱动函数
void Step_motor(unsigned long angle)
{
	unsigned char index = 0;//数组下标索引值
	unsigned char tmp = 0;
	unsigned long beats = 0;//转angle角度需要的节拍数
	unsigned char Beats_Code[8] = {0x0e,0x0c,0x0d,0x09,0x0b,0x03,0x07,0x06};//八拍模式绕组控制顺序表，就是让转子以每拍11.25°的角度转动
	beats = (angle*4096)/360;  //angle就是 圈数*360°，4096是64*64，第一个64是转一圈要有64个拍
	while(beats--)             //（360°分成了8分，一分里面又有8拍，所以一圈一共有 64拍）
	{						   //第二个64是减速比的约等于值，更精确的值应该是63.684，用64来计算的话会产生累计误差
		tmp = P2;
		tmp = tmp & 0xf0;//将低四位置0
		tmp = tmp | Beats_Code[index];//将数组元素的低四位赋给tmp
		P2 = tmp;
		index++;
		index = index & 0x07;
		delay();						   //为什么要乘以第二个64呢，因为程序里控制的是电机里边最小的那个齿轮，这个齿轮转63.684圈，外边的大齿轮才转1圈
	}
}							   
 	
void main()
{
 	Step_motor(360*1);
	while(1);
}