

#include "pcf8574.h"

uint8_t PCF8574_ReadPort() {
    uint8_t value;
    HAL_I2C_Master_Receive(&PCF8574_I2C, PCF8574_DEV, &value, 1, 0xFF);
    return value;
}

void PCF8574_WritePort(uint8_t value) {
    HAL_I2C_Master_Transmit(&PCF8574_I2C, PCF8574_DEV, &value, 1, 0xFF);
}

uint8_t PCF8574_ReadPin(uint8_t bit) {
    return PCF8574_ReadPort() & (1 << bit);
}

void PCF8574_WritePin(uint8_t bit, uint8_t value) {
    uint8_t data;
    data = PCF8574_ReadPort();
    if (value == 0)
        data &= ~(1 << bit);
    else
        data |= 1 << bit;
    PCF8574_WritePort(data);
}
