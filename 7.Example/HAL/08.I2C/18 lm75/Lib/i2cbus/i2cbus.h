#ifndef __I2C_BUS_H__
#define __I2C_BUS_H__

// <! i2c master

#include "utils.h"

// <! config

#include "config.h"

#define CONFIG_SOFTWARE_I2C 0
#define CONFIG_HARDWARE_I2C 1

#define CONFIG_ADDRESS_SCANNER 1  // 地址扫描

#define CONFIG_READ_UINT8   1
#define CONFIG_READ_UINT16  1
#define CONFIG_READ_BITWISE 1  // 位操作

#define CONFIG_WRITE_UINT8   1
#define CONFIG_WRITE_UINT16  1
#define CONFIG_WRITE_BITWISE 1

// <! types

typedef enum {
    I2C_OK  = 0,
    I2C_ERR = 1,
} i2c_state_t;

#if CONFIG_HARDWARE_I2C  // hwi2c

#if CONFIG_MCU_STM32
#include "i2c.h"
typedef I2C_HandleTypeDef i2c_t;
#elif CONFIG_MCU_ESP32
#elif CONFIG_MCU_RP2040

#endif

#elif CONFIG_SOFTWARE_I2C  // swi2c

#include "gpio.h"

typedef enum {
    SOFT_I2C_ID_1 = 1,
    SOFT_I2C_ID_2,
    SOFT_I2C_ID_3,
    SOFT_I2C_ID_4,
    SOFT_I2C_ID_5,
    SOFT_I2C_ID_6,
    SOFT_I2C_ID_7,
    SOFT_I2C_ID_8,
} soft_i2c_id;

#undef SDA_Pin
#undef SCL_Pin

typedef struct {  // id
    soft_i2c_id   ID;
#if CONFIG_MCU_STM32
    GPIO_TypeDef* SDA_Port;
    uint32_t      SDA_Pin;
    GPIO_TypeDef* SCL_Port;
    uint32_t      SCL_Pin;
#elif CONFIG_MCU_ESP32
#elif CONFIG_MCU_RP2040
#endif
    uint32_t      Interval;  // us
} soft_i2c;

typedef soft_i2c i2c_t;

#endif

///////////////////////////////////////////////////////////// softi2c

#if CONFIG_SOFTWARE_I2C
// <! soft i2c

// base

void    soft_i2c_init(soft_i2c* i2c);
void    soft_i2c_start(soft_i2c* i2c);
void    soft_i2c_stop(soft_i2c* i2c);
void    soft_i2c_send_ack(soft_i2c* i2c);
void    soft_i2c_send_nack(soft_i2c* i2c);
uint8_t soft_i2c_recv_ack(soft_i2c* i2c);
void    soft_i2c_send_byte(soft_i2c* i2c, uint8_t data);
uint8_t soft_i2c_recv_byte(soft_i2c* i2c);

// advance

uint8_t soft_i2c_check(soft_i2c* i2c, uint8_t dev);  // is device ready

i2c_state_t soft_i2c_read_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len);
i2c_state_t soft_i2c_write_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len);

#endif

/////////////////////////////////////////////////////////////

// core <

uint8_t i2c_check(i2c_t* i2c, uint8_t dev);  // is device ready

i2c_state_t i2c_read_mem(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len);
i2c_state_t i2c_write_mem(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len);

// > core

// expand <

i2c_state_t i2c_write_mem_ex(i2c_t* i2c, uint8_t dev, uint8_t* buff /* reg,data */, uint16_t len);

#if CONFIG_ADDRESS_SCANNER

// printf 7bits address
uint8_t i2c_scan(i2c_t* i2c);

#endif

#if CONFIG_READ_UINT8

i2c_state_t i2c_read_1byte(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t* data);
uint8_t     i2c_read_1byte_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t def /* default */);

#endif

#if CONFIG_WRITE_UINT8

i2c_state_t i2c_write_1byte(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t data);

#endif

#if CONFIG_READ_UINT16

i2c_state_t i2c_read_2byte(i2c_t* i2c, uint8_t dev, uint8_t lsb, uint8_t msb, uint16_t* data);
uint16_t    i2c_read_2byte_fast(i2c_t* i2c, uint8_t dev, uint8_t lsb, uint8_t msb, uint16_t def);

i2c_state_t i2c_read_2byte_lsb_msb(i2c_t* i2c, uint8_t dev, uint8_t reg, uint16_t* data);
uint16_t    i2c_read_2byte_lsb_msb_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint16_t def);

i2c_state_t i2c_read_2byte_msb_lsb(i2c_t* i2c, uint8_t dev, uint8_t reg, uint16_t* data);
uint16_t    i2c_read_2byte_msb_lsb_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint16_t def);

#endif

#if CONFIG_WRITE_UINT16

i2c_state_t i2c_write_2byte_lsb_msb(i2c_t* i2c, uint8_t dev, uint8_t reg, uint16_t data);
i2c_state_t i2c_write_2byte_msb_lsb(i2c_t* i2c, uint8_t dev, uint8_t reg, uint16_t data);

#endif

#if CONFIG_READ_BITWISE

i2c_state_t i2c_read_bit(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t idx, uint8_t* data);
uint8_t     i2c_read_bit_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t idx, uint8_t def);

i2c_state_t i2c_read_bits(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len, uint8_t* data);
uint8_t     i2c_read_bits_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len, uint8_t def);

i2c_state_t i2c_read_bits_ex(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end, uint8_t* data);
uint8_t     i2c_read_bits_ex_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end, uint8_t def);

i2c_state_t i2c_read_mask(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t mask, uint8_t* data);
uint8_t     i2c_read_mask_fast(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t mask, uint8_t def);

#endif

#if CONFIG_WRITE_BITWISE

i2c_state_t i2c_write_bits(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len, uint8_t data);

i2c_state_t i2c_write_bit(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t idx, uint8_t data);

i2c_state_t i2c_write_bits_ex(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end, uint8_t data);

i2c_state_t i2c_write_mask(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t mask, uint8_t data);

#endif

// > core

#endif