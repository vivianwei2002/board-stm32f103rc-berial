
#ifndef __MAX30100_H_
#define __MAX30100_H_

#include "i2c.h"

#define MAX30100_I2C hi2c2
#define MAX30100_DEV (0x57 << 1)

void    MAX30100_WriteByte(uint8_t reg, uint8_t data);
uint8_t MAX30100_ReadByte(uint8_t reg);

void MAX30100_Init();
void MAX30100_ReadFIFO(uint32_t* led_red, uint32_t* led_ir);

#endif /* MAX30100_H_ */