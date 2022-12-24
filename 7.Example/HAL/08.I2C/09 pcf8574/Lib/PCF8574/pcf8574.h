
#ifndef __PCF8574_H
#define __PCF8574_H

#include "i2c.h"

#define PCF8574_DEV (0x20 << 1)  // <----- dev
#define PCF8574_I2C hi2c2        // <----- hi2c

uint8_t PCF8574_ReadPort();
void    PCF8574_WritePort(uint8_t value);
uint8_t PCF8574_ReadPin(uint8_t bit);
void    PCF8574_WritePin(uint8_t bit, uint8_t value);

#endif
