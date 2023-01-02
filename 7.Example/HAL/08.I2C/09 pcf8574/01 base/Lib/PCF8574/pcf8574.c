

#include "pcf8574.h"

uint8_t pcf8574_read_port()
{
    uint8_t value;
    HAL_I2C_Master_Receive(&PCF8574_I2C, PCF8574_DEV, &value, 1, 0xFF);
    return value;
}

void pcf8574_write_port(uint8_t value)
{
    HAL_I2C_Master_Transmit(&PCF8574_I2C, PCF8574_DEV, &value, 1, 0xFF);
}

uint8_t pcf8574_read_pin(uint8_t bit)
{
    return (pcf8574_read_port() >> bit) & 0x01;
}

void pcf8574_write_pin(uint8_t bit, uint8_t value)
{
    uint8_t data;
    data = pcf8574_read_port();
    if (value == 0)
        data &= ~(1 << bit);
    else
        data |= 1 << bit;
    pcf8574_write_port(data);
}
