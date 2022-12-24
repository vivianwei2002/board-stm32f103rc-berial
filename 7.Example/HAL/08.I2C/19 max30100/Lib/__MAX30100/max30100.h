#ifndef __MAX30100_H
#define __MAX30100_H

#include "i2c.h"

#define MAX30100_I2C hi2c2
#define MAX30100_DEV (0x57 << 1)

#define INTERRUPT_REG 0X00
#define INTERRUPT_REG_A_FULL (0X01 << 7)
#define INTERRUPT_REG_TEMP_RDY (0X01 << 6)
#define INTERRUPT_REG_HR_RDY (0X01 << 5)
#define INTERRUPT_REG_SPO2_RDY (0X01 << 4)
#define INTERRUPT_REG_PWR_RDY (0X01 << 0)

#define SAMPLES_PER_SECOND 50

void MAX30100_Init(void);
void MAX30100_ReadFIFO(uint16_t* hr, uint16_t* spo2);

#endif /*__MAX30100_h*/
