
#include "MAX30100.h"

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

void MAX30100_ReadFIFO(uint16_t* hr, uint16_t* spo2) {
    uint8_t buff[4];
    MAX30100_ReadBytes(0x05, buff, 4);
    *hr   = ((uint16_t)buff[0] << 8) | buff[1];
    *spo2 = ((uint16_t)buff[2] << 8) | buff[3];
}

void MAX30100_Init(void) {
    MAX30100_WriteByte(0x06, 0x0b);  // mode configuration : temp_en[3]      MODE[2:0]=010 HR only enabled    011 SP02 enabled
    // MAX30100_WriteByte(0x06, 0x0a);           // MODE[2:0]=010 HR only enabled     when used is mode ,the red led is not used.
    MAX30100_WriteByte(0x01, 0xF0);           // open all of interrupt
    MAX30100_WriteByte(INTERRUPT_REG, 0x00);  // all interrupt clear
    MAX30100_WriteByte(0x09, 0x33);           // r_pa=3,ir_pa=3

#if (SAMPLES_PER_SECOND == 50)
    MAX30100_WriteByte(0x07, 0x43);  // SPO2_SR[4:2]=000   50 per second    LED_PW[1:0]=11  16BITS
#elif (SAMPLES_PER_SECOND == 100)
    MAX30100_WriteByte(0x07, 0x47);  // SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS
#elif (SAMPLES_PER_SECOND == 200)
    MAX30100_WriteByte(0x07, 0x4F);
#elif (SAMPLES_PER_SECOND == 400)
    MAX30100_WriteByte(0x07, 0x53);
#endif

    MAX30100_WriteByte(0x02, 0x00);  // set FIFO write Pointer reg = 0x00 for clear it
    MAX30100_WriteByte(0x03, 0x00);  // set Over Flow Counter  reg = 0x00 for clear it
    MAX30100_WriteByte(0x04, 0x0F);  // set FIFO Read Pointer  reg = 0x0f for
                                     // waitting  write pointer eq read pointer   to   interrupts  INTERRUPT_REG_A_FULL
}
