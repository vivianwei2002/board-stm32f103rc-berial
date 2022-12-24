

#include "max30100.h"

void MAX30100_WriteByte(uint8_t reg, uint8_t data) {
    HAL_I2C_Mem_Write(&MAX30100_I2C, MAX30100_DEV, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xFF);
}

uint8_t MAX30100_ReadByte(uint8_t reg) {
    uint8_t buff;
    HAL_I2C_Mem_Read(&MAX30100_I2C, MAX30100_DEV, reg, I2C_MEMADD_SIZE_8BIT, &buff, 1, 0xFF);
    return buff;
}

void MAX30100_ReadBytes(uint8_t reg, uint8_t* buff, uint8_t len) {
    HAL_I2C_Mem_Read(&MAX30100_I2C, MAX30100_DEV, reg, I2C_MEMADD_SIZE_8BIT, buff, len, 0xFF);
}

void MAX30100_Init() {
}

void MAX30100_ReadFIFO(uint32_t* led_red /*__OUT__*/, uint32_t* led_ir /*__OUT__*/) {
}
