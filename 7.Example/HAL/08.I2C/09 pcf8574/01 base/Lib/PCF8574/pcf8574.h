
#ifndef __PCF8574_H__
#define __PCF8574_H__

#include "i2c.h"

#define PCF8574_DEV (0x20 << 1)  // <----- dev
#define PCF8574_I2C hi2c2        // <----- hi2c

uint8_t pcf8574_read_port();
void    pcf8574_write_port(uint8_t value);
uint8_t pcf8574_read_pin(uint8_t bit);
void    pcf8574_write_pin(uint8_t bit, uint8_t value);

#endif
