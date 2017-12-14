#ifndef __DS1302_H__
#define __DS1302_H__


extern void reset_1302();
extern void write_data_1302(unsigned char dat);
extern unsigned char read_data_1302();
extern void write_protect();
extern void clear_write_protect();
extern void write_ds1302(unsigned char addr,unsigned char dat);
extern unsigned char read_ds1302(unsigned char addr);
extern void set_time_ds1302(unsigned char *timedata);
extern void read_time_ds1302(unsigned char *timedata);


#endif