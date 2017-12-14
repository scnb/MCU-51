#ifndef __LCD1602_H__
#define __LCD1602_H__

extern void IF_busy();
extern void write_command_1602(unsigned char command);
extern void write_data_1602(unsigned char Data);
extern void Init_1602();
extern void write_string(unsigned char addr_start,unsigned char display);

#endif