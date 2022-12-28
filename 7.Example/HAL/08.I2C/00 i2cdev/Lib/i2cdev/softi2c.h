#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__

// i2c master

#include <stdio.h>
#include "delay.h"
#include "config.h"
#include "gpio.h"

typedef enum {
    SOFT_I2C_OK  = 0,
    SOFT_I2C_ERR = 1,
} soft_i2c_state;

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
    soft_i2c_id ID;
#if CONFIG_MCU_STM32
    GPIO_TypeDef* SDA_Port;
    uint32_t      SDA_Pin;
    GPIO_TypeDef* SCL_Port;
    uint32_t      SCL_Pin;
#elif CONFIG_MCU_ESP32
#elif CONFIG_MCU_RP2040
#endif
    uint32_t Interval;  // us
} soft_i2c;

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

soft_i2c_state soft_i2c_read_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len);
soft_i2c_state soft_i2c_write_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len);

#endif