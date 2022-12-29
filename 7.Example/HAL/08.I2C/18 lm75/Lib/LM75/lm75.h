#ifndef _LM75_I2C_H_
#define _LM75_I2C_H_

#include "i2cbus/i2cbus.h"

// lm75 address (A2A1A0)
enum {
    LM75_ADDRESS_A000 = 0x48,
    LM75_ADDRESS_A001,
    LM75_ADDRESS_A010,
    LM75_ADDRESS_A011,
    LM75_ADDRESS_A100,
    LM75_ADDRESS_A101,
    LM75_ADDRESS_A110,
    LM75_ADDRESS_A111,
};

#define LM75_DEV (LM75_ADDRESS_A000 << 1)  // <---
#define LM75_I2C hi2c2                     // <---

// chip register
#define LM75_REG_CONF  0x01  // configure
#define LM75_REG_TEMP  0x00  // temperature
#define LM75_REG_TOS   0x03  // overtemperature shutdown, Tth(ots) , default = 80°C
#define LM75_REG_THYST 0x02  // hysteresis, Thys , default = 75°C

// fault queue
typedef enum {
    LM75_FAULT_QUEUE_1 = 0,
    LM75_FAULT_QUEUE_2 = 1,
    LM75_FAULT_QUEUE_3 = 2,
    LM75_FAULT_QUEUE_6 = 3,
} lm75_fault_queue_t;

// os polarity / os active level
typedef enum {
    LM75_OS_POLARITY_LOW  = 0,
    LM75_OS_POLARITY_HIGH = 1,
} lm75_os_polarity_t;

// mode
typedef enum {
    LM75_MODE_NORMAL   = 0,
    LM75_MODE_SHUTDOWN = 1,
} lm75_device_mode_t;

// os operation
typedef enum {
    LM75_OS_OPERATION_COMPARATOR = 0,  // 比较器
    LM75_OS_OPERATION_INTERRUPT  = 1,  // 中断
} lm75_os_operation_mode_t;

////////////////////////////////////////////

#define lm75_read_1byte(reg)                   i2c_read_1byte_fast(&LM75_I2C, LM75_DEV, reg)
#define lm75_read_2byte(reg)                   i2c_read_2byte_msb_lsb_fast(&LM75_I2C, LM75_DEV, reg)
#define lm75_write_1byte(reg, data)            i2c_write_1byte(&LM75_I2C, LM75_DEV, reg, data)
#define lm75_write_2byte(reg, data)            i2c_write_2byte_msb_lsb(&LM75_I2C, LM75_DEV, reg, data)
#define lm75_read_bit(reg, index)              i2c_read_bit_fast(&LM75_I2C, LM75_DEV, reg, index)
#define lm75_write_bit(reg, index, data)       i2c_write_bit(&LM75_I2C, LM75_DEV, reg, index, data)
#define lm75_read_bits(reg, start, len)        i2c_read_bits_fast(&LM75_I2C, LM75_DEV, reg, start, len)
#define lm75_write_bits(reg, start, len, data) i2c_write_bits(&LM75_I2C, LM75_DEV, reg, start, len, data)

////////////////////////////////////////////

float lm75_get_limit(uint8_t reg);
void  lm75_set_limit(uint8_t reg, float value);

// limits

#define lm75_get_hysteresis()           lm75_get_limit(LM75_REG_THYST)
#define lm75_set_hysteresis(hysteresis) lm75_set_limit(LM75_REG_THYST, hysteresis)

#define lm75_get_over_temperature_threshold()          lm75_get_limit(LM75_REG_TOS)
#define lm75_set_over_temperature_threshold(threshold) lm75_set_limit(LM75_REG_TOS, threshold)

// bit[0]
#define lm75_get_device_mode()     lm75_read_bit(LM75_REG_CONF, 0)
#define lm75_set_device_mode(mode) lm75_write_bit(LM75_REG_CONF, 0, mode)

// bit[1]
#define lm75_get_operation_mode()     lm75_read_bit(LM75_REG_CONF, 1)
#define lm75_set_operation_mode(mode) lm75_write_bit(LM75_REG_CONF, 1, mode)

// bit[2]
#define lm75_get_os_polarity()     lm75_read_bit(LM75_REG_CONF, 2)
#define lm75_set_os_polarity(mode) lm75_write_bit(LM75_REG_CONF, 2, mode)

// bit[4,3]
#define lm75_get_fault_queue()            lm75_read_bits(LM75_REG_CONF, 3, 2);
#define lm75_set_fault_queue(fault_queue) lm75_write_bits(LM75_REG_CONF, 3, 2, fault_queue);

// temp

float lm75_read_temp(void);

#endif
