#include <reg52.h>	
#include "intrins.h"



void delay()
{	
	unsigned int i = 200;
	while(i--);
}

//���������������
void Step_motor(unsigned long angle)
{
	unsigned char index = 0;//�����±�����ֵ
	unsigned char tmp = 0;
	unsigned long beats = 0;//תangle�Ƕ���Ҫ�Ľ�����
	unsigned char Beats_Code[8] = {0x0e,0x0c,0x0d,0x09,0x0b,0x03,0x07,0x06};//����ģʽ�������˳���������ת����ÿ��11.25��ĽǶ�ת��
	beats = (angle*4096)/360;  //angle���� Ȧ��*360�㣬4096��64*64����һ��64��תһȦҪ��64����
	while(beats--)             //��360��ֳ���8�֣�һ����������8�ģ�����һȦһ���� 64�ģ�
	{						   //�ڶ���64�Ǽ��ٱȵ�Լ����ֵ������ȷ��ֵӦ����63.684����64������Ļ�������ۼ����
		tmp = P2;
		tmp = tmp & 0xf0;//������λ��0
		tmp = tmp | Beats_Code[index];//������Ԫ�صĵ���λ����tmp
		P2 = tmp;
		index++;
		index = index & 0x07;
		delay();						   //ΪʲôҪ���Եڶ���64�أ���Ϊ��������Ƶ��ǵ�������С���Ǹ����֣��������ת63.684Ȧ����ߵĴ���ֲ�ת1Ȧ
	}
}							   
 	
void main()
{
 	Step_motor(360*1);
	while(1);
}