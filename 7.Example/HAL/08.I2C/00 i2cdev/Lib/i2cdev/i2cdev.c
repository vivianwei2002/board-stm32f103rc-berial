#include "i2cdev.h"

// core<

inline i2c_state_t i2c_read_mem(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len)
{
#if CONFIG_HARDWARE_I2C
#if CONFIG_MCU_STM32
    return (HAL_I2C_Mem_Read(i2c, dev, reg, I2C_MEMADD_SIZE_8BIT, data, len, 0xFF) == HAL_OK) ? I2C_OK : I2C_ERR;
#elif CONFIG_MCU_ESP32
#elif CONFIG_MCU_RP2040
#endif
#elif CONFIG_SOFTWARE_I2C
    return (soft_i2c_read_mem(i2c, dev, reg, data, len) == SOFT_I2C_OK) ? I2C_OK : I2C_ERR;
#endif
}

inline i2c_state_t i2c_write_mem(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len)
{
#if CONFIG_HARDWARE_I2C
#if CONFIG_MCU_STM32
    return (HAL_I2C_Mem_Write(i2c, dev, reg, I2C_MEMADD_SIZE_8BIT, data, len, 0xFF) == HAL_OK) ? I2C_OK : I2C_ERR;
#elif CONFIG_MCU_ESP32
#elif CONFIG_MCU_RP2040
#endif
#elif CONFIG_SOFTWARE_I2C
    return (soft_i2c_write_mem(i2c, dev, reg, data, len) == SOFT_I2C_OK) ? I2C_OK : I2C_ERR;
#endif
}

// >core

i2c_state_t i2c_write_mem_ex(i2c_t* i2c, uint8_t dev, uint8_t* buff /* reg,data */, uint16_t len)
{
    for (uint8_t i = 0; i < len; i += 2)
        if (i2c_write_mem(i2c, dev, buff[i], &buff[i + 1], 1) == I2C_ERR)
            return I2C_ERR;
    return I2C_OK;
}

uint8_t i2c_check(i2c_t* i2c, uint8_t dev)
{
#if CONFIG_HARDWARE_I2C
#if CONFIG_MCU_STM32
    return (HAL_I2C_IsDeviceReady(i2c, dev, 5, 0xFF) == HAL_OK) ? I2C_OK : I2C_ERR;
#elif CONFIG_MCU_ESP32
#endif
#elif CONFIG_SOFTWARE_I2C
    return (soft_i2c_check(i2c, dev) == I2C_OK) ? I2C_OK : I2C_ERR;
#endif
}

#if CONFIG_ADDRESS_SCANNER

uint8_t i2c_scan(i2c_t* i2c)
{
    uint8_t addr_7bits, cnt = 0;
    printf("7bits address:\r\n");
    for (addr_7bits = 0; addr_7bits < 0x80; ++addr_7bits) {
        if (i2c_check(i2c, addr_7bits << 1) == I2C_OK) {
            printf("0x%2x(%d)\r\n", addr_7bits, addr_7bits);
            ++cnt;
        }
    }
    printf("%d devices scanned\r\n", cnt);
    return cnt;
}

#endif

#if CONFIG_READ_WRITE_UINT8

i2c_state_t i2c_read_1byte(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t* data)
{
    return i2c_read_mem(i2c, dev, reg, data, 1);
}

uint8_t i2c_read_1byte_fast(i2c_t* i2c, uint8_t dev, uint8_t reg)
{
    uint8_t buff;
    if (i2c_read_1byte(i2c, dev, reg, &buff) == I2C_ERR)
        return I2C_ERR;
    return buff;
}

i2c_state_t i2c_write_1byte(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t data)
{
    return i2c_write_mem(i2c, dev, reg, &data, 1);
}

#endif

#if CONFIG_READ_WRITE_UINT16

i2c_state_t i2c_read_2byte(i2c_t* i2c, uint8_t dev, uint8_t reg_lsb, uint8_t reg_msb, uint16_t* data)
{
    uint8_t lsb, msb;
    if (i2c_read_mem(i2c, dev, reg_lsb, &lsb, 1) == I2C_ERR)
        return I2C_ERR;
    if (i2c_read_mem(i2c, dev, reg_msb, &msb, 1) == I2C_ERR)
        return I2C_ERR;
    *data = (msb << 8) | lsb;
    return I2C_OK;
}

uint16_t i2c_read_2byte_fast(i2c_t* i2c, uint8_t dev, uint8_t reg_lsb, uint8_t reg_msb)
{
    uint16_t buff;
    if (i2c_read_2byte(i2c, dev, reg_lsb, reg_msb, &buff) == I2C_ERR)
        return I2C_ERR;
    return buff;
}

i2c_state_t i2c_read_2byte_lsb_msb(i2c_t* i2c, uint8_t dev, uint8_t reg, uint16_t* data)
{
    uint8_t buff[2];
    if (i2c_read_mem(i2c, dev, reg, buff, 2) == I2C_ERR)
        return I2C_ERR;
    *data = (buff[1] << 8) | buff[0];
    return I2C_OK;
}

uint16_t i2c_read_2byte_lsb_msb_fast(i2c_t* i2c, uint8_t dev, uint8_t reg)
{
    uint16_t buff;
    if (i2c_read_2byte_lsb_msb(i2c, dev, reg, &buff) == I2C_ERR)
        return I2C_ERR;
    return buff;
}

i2c_state_t i2c_read_2byte_msb_lsb(i2c_t* i2c, uint8_t dev, uint8_t reg, uint16_t* data)
{
    uint8_t buff[2];
    if (i2c_read_mem(i2c, dev, reg, buff, 2) == I2C_ERR)
        return I2C_ERR;
    *data = (buff[0] << 8) | buff[1];
    return I2C_OK;
}

uint16_t i2c_read_2byte_msb_lsb_fast(i2c_t* i2c, uint8_t dev, uint8_t reg)
{
    uint16_t buff;
    if (i2c_read_2byte_msb_lsb(i2c, dev, reg, &buff) == I2C_ERR)
        return I2C_ERR;
    return buff;
}

#endif

#if CONFIG_READ_WRITE_BITWISE

i2c_state_t i2c_read_bit(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t idx /* index */, uint8_t* data)
{
    uint8_t buff;

    if (i2c_read_mem(i2c, dev, reg, &buff, 1) == I2C_ERR)
        return I2C_ERR;

    *data = (buff >> (idx & 0x07)) & 0x01;

    return I2C_ERR;
}

uint8_t i2c_read_bit_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t idx)
{
    uint8_t buff;
    if (i2c_read_bit(i2c, dev, reg, idx, &buff) == I2C_ERR)
        return I2C_ERR;
    return buff;
}

i2c_state_t i2c_write_bit(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t idx, uint8_t data)
{
    uint8_t buff;

    if (i2c_read_mem(i2c, dev, reg, &buff, 1) == I2C_ERR)
        return I2C_ERR;

    idx &= 0x07;

    if (data) {
        buff |= (1 << idx);
    } else {
        buff &= ~(1 << idx);
    }

    return i2c_write_mem(i2c, dev, reg, &buff, 1);
}

i2c_state_t i2c_read_bits(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len, uint8_t* data)
{
    uint8_t buff;

    if (i2c_read_mem(i2c, dev, reg, &buff, 1) == I2C_ERR)
        return I2C_ERR;

    *data = buff & (0xFF >> (8 - len) << start);
    *data >>= start;

    return I2C_OK;
}

uint8_t i2c_read_bits_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len)
{
    uint8_t buff;
    if (i2c_read_bits(i2c, dev, reg, start, len, &buff) == I2C_ERR)
        return I2C_ERR;
    return buff;
}

i2c_state_t i2c_write_bits(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len, uint8_t data)
{
    uint8_t buff, mask;

    if (i2c_read_mem(i2c, dev, reg, &buff, 1) == I2C_ERR)
        return I2C_ERR;

    mask = 0xFF >> (8 - len) << start;
    data <<= start;

    buff &= ~mask;
    buff |= mask & data;

    return i2c_write_mem(i2c, dev, reg, &buff, 1);
}

i2c_state_t i2c_read_bits_ex(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end, uint8_t* data)
{
    return i2c_read_bits(i2c, dev, reg, start, end - start, data);
}

uint8_t i2c_read_bits_ex_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end)
{
    uint8_t buff;
    if (i2c_read_bits_ex(i2c, dev, reg, start, end, &buff) == I2C_ERR)
        return I2C_ERR;
    return buff;
}

i2c_state_t i2c_write_bits_ex(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end, uint8_t data)
{
    return i2c_write_bits(i2c, dev, reg, start, end - start, data);
}

i2c_state_t i2c_read_mask(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t mask, uint8_t* data)
{
    uint8_t buff;

    if (i2c_read_mem(i2c, dev, reg, &buff, 1))
        return I2C_ERR;

    *data = buff & mask;

    return I2C_OK;
}

uint8_t i2c_read_mask_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t mask)
{
    uint8_t buff;
    if (i2c_read_mask(i2c, dev, reg, mask, &buff) == I2C_ERR)
        return I2C_ERR;
    return buff;
}

i2c_state_t i2c_write_mask(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t mask, uint8_t data)
{
    uint8_t buff;

    if (i2c_read_mem(i2c, dev, reg, &buff, 1))
        return I2C_ERR;

    buff &= ~mask;  // clear area where are set 1

    buff |= (mask & data);  // set value

    return i2c_write_mem(i2c, dev, reg, &buff, 1);
}

#endif