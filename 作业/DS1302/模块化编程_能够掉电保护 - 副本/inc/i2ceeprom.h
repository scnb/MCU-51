#ifndef __I2CEEPROM_H__
#define __I2CEEPROM_H__

extern  void InitI2C();
extern  void i2c_start_fun();
extern  void i2c_stop_fun();
extern  void i2c_write_data(unsigned char byte);
extern  unsigned char i2c_read_data();
extern  void i2c_write_eeprom(unsigned char addr,unsigned char databyte);
extern  unsigned char i2c_read_eeprom(unsigned char addr);


#endif