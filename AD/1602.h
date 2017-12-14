#ifndef __1602_H_
#define __1602_H_

void IF_busy()
void Delay5us()
void write_command(unsigned char command);
void write_data(unsigned char Data);
void Init_1602();
void write_string(unsigned char addr_start, unsigned char table[]);

#endif