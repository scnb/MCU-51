
#ifndef __18B20_H__
#define	__18B20_H__

extern void TempDelay (unsigned char us);
extern void delay(unsigned int count);
extern void Delay1ms(int i);
extern void ds_reset();
extern void ds_write_byte(unsigned char dat);
extern unsigned char ds_read_byte();
extern void ds18b20();
extern unsigned int get_temperature();
extern unsigned int int_to_shiwei(unsigned int temp);
extern unsigned int int_to_gewei(unsigned int temp);
extern unsigned int int_to_xiaoshuwei(unsigned int temp);
#endif