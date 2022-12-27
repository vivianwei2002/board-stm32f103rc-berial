#ifndef __I2C_MASTER_H__
#define __I2C_MASTER_H__

#include <stdio.h>
#include <stdlib.h>

#define LOG(format, ...) printf("[ line: %d | file: %s | function : %s ] " format "\r\n", __LINE__, __FILE__, __FUNCTION__, ##__VA_ARGS__)

///////////////////////////////////////////////////// config

#define CONFIG_MCU_STM32 1
#define CONFIG_MCU_ESP32 0

#define CONFIG_SOFTWARE_I2C 1
#define CONFIG_HARDWARE_I2C 0

#define CONFIG_ADDRESS_SCANNER 1  // 地址扫描

#define CONFIG_READ_WRITE_UINT8   0
#define CONFIG_READ_WRITE_UINT16  0
#define CONFIG_READ_WRITE_BITWISE 0  // 位操作

/////////////////////////////////////////////////////

#if CONFIG_HARDWARE_I2C

#if CONFIG_MCU_STM32
#include "i2c.h"
typedef I2C_HandleTypeDef i2c_t;
#elif CONFIG_MCU_ESP32
#endif
#elif CONFIG_SOFTWARE_I2C
#include "softi2c.h"
typedef soft_i2c i2c_t;
#endif

// types

typedef enum {
    I2C_OK  = 0,
    I2C_ERR = 1,
} i2c_state_t;

// functions

#if CONFIG_ADDRESS_SCANNER

uint8_t i2c_scan(i2c_t* i2c);

#endif

uint8_t i2c_check(i2c_t* i2c, uint8_t dev);  // is device ready

i2c_state_t i2c_read_mem(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len);
i2c_state_t i2c_write_mem(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len);
i2c_state_t i2c_write_mem_ex(i2c_t* i2c, uint8_t dev, uint8_t* buff /* reg,data */, uint16_t len);

#if CONFIG_READ_WRITE_UINT8

i2c_state_t i2c_read_1byte(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t* data);
uint8_t     i2c_read_1byte_fast(i2c_t* i2c, uint8_t dev, uint8_t reg);

i2c_state_t i2c_write_1byte(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t data);

#endif

#if CONFIG_READ_WRITE_UINT16

i2c_state_t i2c_read_2byte(i2c_t* i2c, uint8_t dev, uint8_t reg_lsb, uint8_t reg_msb, uint16_t* data);
uint16_t    i2c_read_2byte_fast(i2c_t* i2c, uint8_t dev, uint8_t reg_lsb, uint8_t reg_msb);
i2c_state_t i2c_read_2byte_lsb_msb(i2c_t* i2c, uint8_t dev, uint8_t reg, uint16_t* data);
uint16_t    i2c_read_2byte_lsb_msb_fast(i2c_t* i2c, uint8_t dev, uint8_t reg);
i2c_state_t i2c_read_2byte_msb_lsb(i2c_t* i2c, uint8_t dev, uint8_t reg, uint16_t* data);
uint16_t    i2c_read_2byte_msb_lsb_fast(i2c_t* i2c, uint8_t dev, uint8_t reg);

#endif

#if CONFIG_READ_WRITE_BITWISE

// read/write bit with index
i2c_state_t i2c_read_bit(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t idx, uint8_t* data);
uint8_t     i2c_read_bit_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t idx);
i2c_state_t i2c_write_bit(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t idx, uint8_t data);

// read/write bits with start and length
i2c_state_t i2c_read_bits(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len, uint8_t* data);
uint8_t     i2c_read_bits_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len);
i2c_state_t i2c_write_bits(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len, uint8_t data);

// read/write bits with start and end
i2c_state_t i2c_read_bits_ex(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end, uint8_t* data);
uint8_t     i2c_read_bits_ex_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end);
i2c_state_t i2c_write_bits_ex(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end, uint8_t data);

// read/write bits with mask
i2c_state_t i2c_read_mask(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t mask, uint8_t* data);
uint8_t     i2c_read_mask_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t mask);
i2c_state_t i2c_write_mask(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t mask, uint8_t data);

#endif

#endif