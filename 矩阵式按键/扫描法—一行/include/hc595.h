#ifndef _HC595_H_
#define _HC595_H_

sbit HC595_SCK = P1^1;
sbit HC595_RCK = P1^0;
sbit HC595_DATA = P1^4;

extern void Send_Data(unsigned char Data_One,unsigned char Data_Two);

#endif