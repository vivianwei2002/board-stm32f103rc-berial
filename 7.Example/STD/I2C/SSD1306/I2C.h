#ifndef __IIC_H
#define __IIC_H

void IIC_Init(void);
void OLED_WriteCommand(uint8_t cmd);
void OLED_WriteData(uint8_t data);

#endif
