
#include "MAX30100.h"

uint16_t rawIRValue;
uint16_t rawRedValue;

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
    MAX30100_SetMode(DEFAULT_MODE);                                            //往地址0x06的寄存器写0x02（仅打开心率测量模块）
    MAX30100_SetLedsPulseWidth(DEFAULT_PULSE_WIDTH);                           //往地址0x07的寄存器写0x03（16位AD转换，脉冲宽度1.6ms）
    MAX30100_SetSamplingRate(DEFAULT_SAMPLING_RATE);                           //往地址0x07的寄存器写0x01（设置采样率为100）
    MAX30100_SetLedsCurrent(DEFAULT_IR_LED_CURRENT, DEFAULT_RED_LED_CURRENT);  //往地址0x09的寄存器写0xff（控制Red_led,IR_led电流各为50ma）
    MAX30100_SetHighresModeEnabled(1);                                         //使能血氧饱和度的ADC的分辨率为16位，1.6ms LED脉冲宽度
}

void MAX30100_SetMode(Mode mode) {
    MAX30100_WriteByte(MAX30100_REG_MODE_CONFIGURATION, mode);
}

void MAX30100_SetLedsPulseWidth(LEDPulseWidth ledPulseWidth) {
    uint8_t previous = MAX30100_ReadByte(MAX30100_REG_SPO2_CONFIGURATION);                   // reg 0x07
    MAX30100_WriteByte(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | ledPulseWidth);  // reg 0x07 w
}

void MAX30100_SetSamplingRate(SamplingRate samplingRate) {
    uint8_t previous = MAX30100_ReadByte(MAX30100_REG_SPO2_CONFIGURATION);                         // reg 0x07
    MAX30100_WriteByte(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (samplingRate << 2));  // reg 0x07 w
}

void MAX30100_SetLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent) {
    MAX30100_WriteByte(MAX30100_REG_LED_CONFIGURATION, redLedCurrent << 4 | irLedCurrent);  // reg 0x09
}

void MAX30100_SetHighresModeEnabled(uint8_t enable) {
    uint8_t previous = MAX30100_ReadByte(MAX30100_REG_SPO2_CONFIGURATION);
    MAX30100_WriteByte(MAX30100_REG_SPO2_CONFIGURATION, enable ? (previous | MAX30100_SPC_SPO2_HI_RES_EN) : (previous & ~MAX30100_SPC_SPO2_HI_RES_EN));
}

void MAX30100_Update() {
    MAX30100_ReadFIFO();
}

void MAX30100_ReadFIFO(void) {
    uint8_t buff[4];
    MAX30100_ReadBytes(MAX30100_REG_FIFO_DATA, buff, 4);

    // Warning: the values are always left-aligned
    rawIRValue  = (buff[0] << 8) | buff[1];
    rawRedValue = (buff[2] << 8) | buff[3];

    // printf("rawIRValue =  %d \n", rawIRValue);
    // printf("rawRedValue =  %d \n", rawRedValue);
}

void MAX30100_ResetFifo() {
    MAX30100_WriteByte(MAX30100_REG_FIFO_WRITE_POINTER, 0);
    MAX30100_WriteByte(MAX30100_REG_FIFO_READ_POINTER, 0);
    MAX30100_WriteByte(MAX30100_REG_FIFO_OVERFLOW_COUNTER, 0);
}

uint8_t MAX30100_GetPartId() {  // return 0x11
    return MAX30100_ReadByte(0xff);
}

void MAX30100_StartTemperatureSampling() {
    uint8_t modeConfig = MAX30100_ReadByte(MAX30100_REG_MODE_CONFIGURATION) | MAX30100_MC_TEMP_EN;
    MAX30100_WriteByte(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

uint8_t MAX30100_IsTemperatureReady() {
    return !(MAX30100_ReadByte(MAX30100_REG_MODE_CONFIGURATION) & MAX30100_MC_TEMP_EN);
}

float MAX30100_RetrieveTemperature() {
    int8_t tempInteger = MAX30100_ReadByte(MAX30100_REG_TEMPERATURE_DATA_INT);
    float  tempFrac    = MAX30100_ReadByte(MAX30100_REG_TEMPERATURE_DATA_FRAC);
    return tempFrac * 0.0625 + tempInteger;
}

void MAX30100_Shutdown() {
    uint8_t modeConfig = MAX30100_ReadByte(MAX30100_REG_MODE_CONFIGURATION) | MAX30100_MC_SHDN;
    MAX30100_WriteByte(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

void MAX30100_Resume() {
    uint8_t modeConfig = MAX30100_ReadByte(MAX30100_REG_MODE_CONFIGURATION) & ~MAX30100_MC_SHDN;
    MAX30100_WriteByte(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}