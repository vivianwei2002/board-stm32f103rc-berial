#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__

#include <stdio.h>
#include "delay.h"

// i2c master

#define WRITE 0x00
#define READ 0x01

typedef enum {
    I2C_OK  = 0,
    I2C_ERR = 1,
} soft_i2c_state;

#if 0
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
#endif

typedef struct soft_i2c {
    uint8_t (*SDA)(void);   // read SDA level
    void (*SDA_IN)(void);   // set SDA input mode
    void (*SDA_OUT)(void);  // set SDA output mode
    void (*SDA_0)(void);    // write SDA low
    void (*SDA_1)(void);    // write SDA high
    void (*SCL_0)(void);    // write SCL low
    void (*SCL_1)(void);    // write SCL high
    void (*delay)(void);    // delay
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

uint8_t soft_i2c_scan(soft_i2c* i2c);
uint8_t soft_i2c_check(soft_i2c* i2c, uint8_t dev);  // is device ready

soft_i2c_state soft_i2c_read_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len);
soft_i2c_state soft_i2c_write_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len);

soft_i2c_state soft_i2c_read_1byte(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data);
soft_i2c_state soft_i2c_read_2byte(soft_i2c* i2c, uint8_t dev, uint8_t reg_lsb, uint8_t reg_msb, uint16_t* data);

soft_i2c_state soft_i2c_write_1byte(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t data);

soft_i2c_state soft_i2c_read_bit(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t bit, uint8_t* data);
soft_i2c_state soft_i2c_write_bit(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t bit, uint8_t data);

soft_i2c_state soft_i2c_read_bits(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len, uint8_t* data);
soft_i2c_state soft_i2c_write_bits(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len, uint8_t data);

soft_i2c_state soft_i2c_read_bits_ex(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end, uint8_t* data);
soft_i2c_state soft_i2c_write_bits_ex(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end, uint8_t data);

soft_i2c_state soft_i2c_read_mask(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t mask, uint8_t* data);
soft_i2c_state soft_i2c_write_mask(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t mask, uint8_t data);

#endif