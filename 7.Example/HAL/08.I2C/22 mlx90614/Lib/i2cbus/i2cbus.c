#include "i2cbus.h"

#if CONFIG_SOFTWARE_I2C

///////////////////////////////////////////////////////////// softi2c

// config <

#if CONFIG_MCU_STM32

// soft_i2c i2c = {
//     .ID       = SOFT_I2C_ID_1,
//     .SCL_Port = GPIOB,
//     .SCL_Pin  = GPIO_PIN_10,
//     .SDA_Port = GPIOB,
//     .SDA_Pin  = GPIO_PIN_11,
//     .Interval = 6,
// };

#define SDA_L(i2c) HAL_GPIO_WritePin(i2c->SDA_Port, i2c->SDA_Pin, GPIO_PIN_RESET)
#define SDA_H(i2c) HAL_GPIO_WritePin(i2c->SDA_Port, i2c->SDA_Pin, GPIO_PIN_SET)

#define SCL_L(i2c) HAL_GPIO_WritePin(i2c->SCL_Port, i2c->SCL_Pin, GPIO_PIN_RESET)
#define SCL_H(i2c) HAL_GPIO_WritePin(i2c->SCL_Port, i2c->SCL_Pin, GPIO_PIN_SET)

#define SDA(i2c) HAL_GPIO_ReadPin(i2c->SDA_Port, i2c->SDA_Pin)

#define Delay(i2c) HAL_Delay_us(i2c->Interval)

static GPIO_InitTypeDef GPIO_InitStruct = {
    .Pull  = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_HIGH,
};

void SDA_IN(soft_i2c* i2c)
{
    GPIO_InitStruct.Pin  = i2c->SDA_Pin,
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT,
    HAL_GPIO_Init(i2c->SDA_Port, &GPIO_InitStruct);
}

void SDA_OUT(soft_i2c* i2c)
{
    GPIO_InitStruct.Pin  = i2c->SDA_Pin,
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD,
    HAL_GPIO_Init(i2c->SDA_Port, &GPIO_InitStruct);
}

#elif CONFIG_MCU_ESP32
#elif CONFIG_MCU_RP2040
#endif

// > config

/////////////////////////////////////////////////////////////

/*! read / write */

#define SOFT_I2C_WRITE 0x00
#define SOFT_I2C_READ  0x01

void soft_i2c_init(soft_i2c* i2c)
{
    SDA_OUT(i2c);
    SDA_H(i2c);
    SCL_H(i2c);
}

void soft_i2c_start(soft_i2c* i2c)
{
    SDA_OUT(i2c);
    SDA_H(i2c);  // sda=1
    Delay(i2c);
    SCL_H(i2c);  // scl=1
    Delay(i2c);
    SDA_L(i2c);  // sda=0
    Delay(i2c);
    SCL_L(i2c);  // scl=0
    Delay(i2c);
}

void soft_i2c_stop(soft_i2c* i2c)
{
    SDA_OUT(i2c);
    SCL_L(i2c);  // scl=0
    Delay(i2c);
    SDA_L(i2c);  // sda=0
    Delay(i2c);
    SCL_H(i2c);  // scl=1
    Delay(i2c);
    SDA_H(i2c);  // sda=1
    Delay(i2c);
}

void soft_i2c_send_ack(soft_i2c* i2c)
{
    SDA_OUT(i2c);
    SCL_L(i2c);  // scl=0
    Delay(i2c);
    SDA_L(i2c);  // sda=0
    Delay(i2c);
    SCL_H(i2c);  // scl=1
    Delay(i2c);
    SCL_L(i2c);  // scl=0
    Delay(i2c);
}

void soft_i2c_send_nack(soft_i2c* i2c)
{
    SDA_OUT(i2c);
    SCL_L(i2c);  // scl=0
    Delay(i2c);
    SDA_H(i2c);  // sda=1
    Delay(i2c);
    SCL_H(i2c);  // scl=1
    Delay(i2c);
    SCL_L(i2c);  // scl=0
    Delay(i2c);
}

// return 0:ack,1:nack
uint8_t soft_i2c_recv_ack(soft_i2c* i2c)
{
    uint8_t timeout = 0;

    SDA_H(i2c);  // sda=1
    Delay(i2c);
    SCL_H(i2c);  // scl=1
    Delay(i2c);

    SDA_IN(i2c);
    while (SDA(i2c))
        if (++timeout > 250)
            return 1;  // nack

    SCL_L(i2c);  // scl=0
    Delay(i2c);

    return 0;  // ack
}

void soft_i2c_send_byte(soft_i2c* i2c, uint8_t data)
{
    uint8_t mask = 0x80;
    SDA_OUT(i2c);
    do {
        (data & mask) ?
            SDA_H(i2c) :  // sda = 1
            SDA_L(i2c);   // sda = 0
        Delay(i2c);
        SCL_H(i2c);  // scl = 1
        Delay(i2c);
        SCL_L(i2c);  // scl = 0
        Delay(i2c);
    } while (mask >>= 1);
}

// return 0:success,1:fail
uint8_t soft_i2c_recv_byte(soft_i2c* i2c)
{
    uint8_t mask = 0x80, data = 0x00;
    SDA_IN(i2c);
    do {
        SCL_H(i2c);
        Delay(i2c);
        if (SDA(i2c))
            data |= mask;
        SCL_L(i2c);
        Delay(i2c);
    } while (mask >>= 1);
    return data;
}

uint8_t soft_i2c_check(soft_i2c* i2c, uint8_t dev)
{
    uint8_t ack;
    soft_i2c_start(i2c);
    soft_i2c_send_byte(i2c, dev);
    ack = soft_i2c_recv_ack(i2c);
    soft_i2c_stop(i2c);
    return ack == 0 ? I2C_OK : I2C_ERR;
}

/////////////////////////////////////////////////////

i2c_state_t soft_i2c_read_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len)
{
    dev &= 0xFE;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | SOFT_I2C_WRITE);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), I2C_ERR;

    soft_i2c_send_byte(i2c, reg);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), I2C_ERR;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | SOFT_I2C_READ);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), I2C_ERR;

    while (--len) {
        *data++ = soft_i2c_recv_byte(i2c);
        soft_i2c_send_ack(i2c);
    }

    *data = soft_i2c_recv_byte(i2c);
    soft_i2c_send_nack(i2c);

    return soft_i2c_stop(i2c), I2C_OK;
}

i2c_state_t soft_i2c_write_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len)
{
    dev &= 0xFE;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | SOFT_I2C_WRITE);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), I2C_ERR;

    soft_i2c_send_byte(i2c, reg);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), I2C_ERR;

    while (len--) {
        soft_i2c_send_byte(i2c, *data++);
        if (soft_i2c_recv_ack(i2c))
            return soft_i2c_stop(i2c), I2C_ERR;
    }

    return soft_i2c_stop(i2c), I2C_OK;
}

#endif

///////////////////////////////////////////////////////////// i2c function

// core <

inline uint8_t i2c_check(i2c_t* i2c, uint8_t dev)
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

inline i2c_state_t i2c_read_mem(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len)
{
#if CONFIG_HARDWARE_I2C
#if CONFIG_MCU_STM32
    return (HAL_I2C_Mem_Read(i2c, dev, reg, I2C_MEMADD_SIZE_8BIT, data, len, 0xFF) == HAL_OK) ? I2C_OK : I2C_ERR;
#elif CONFIG_MCU_ESP32
#elif CONFIG_MCU_RP2040
#endif
#elif CONFIG_SOFTWARE_I2C
    return soft_i2c_read_mem(i2c, dev, reg, data, len);
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
    return soft_i2c_write_mem(i2c, dev, reg, data, len);
#endif
}

// > core

/////////////////////////////////////////////////////////////

// expand <

i2c_state_t i2c_write_mem_ex(i2c_t* i2c, uint8_t dev, uint8_t* buff /* reg,data */, uint16_t len)
{
    for (uint8_t i = 0; i < len; i += 2)
        if (i2c_write_mem(i2c, dev, buff[i], &buff[i + 1], 1) == I2C_ERR)
            return I2C_ERR;
    return I2C_OK;
}

#if CONFIG_ADDRESS_SCANNER

uint8_t i2c_scan(i2c_t* i2c)
{
    uint8_t addr_7bits, cnt = 0;
    println("7bits address:");
    for (addr_7bits = 0; addr_7bits < 0x80; ++addr_7bits)
        if (i2c_check(i2c, addr_7bits << 1) == I2C_OK)
            println("[%03d] 0x%02x(%d)", ++cnt, addr_7bits, addr_7bits);
    println("--------------");
    return cnt;
}

#endif

#if CONFIG_READ_UINT8

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

#endif

#if CONFIG_WRITE_UINT8

i2c_state_t i2c_write_1byte(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t data)
{
    return i2c_write_mem(i2c, dev, reg, &data, 1);
}

#endif

#if CONFIG_READ_UINT16

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

#if CONFIG_WRITE_UINT16

i2c_state_t i2c_write_2byte_lsb_msb(i2c_t* i2c, uint8_t dev, uint8_t reg, uint16_t data)
{
    uint8_t buff[2] = {data & 0xFF, data >> 8};
    return i2c_write_mem(i2c, dev, reg, buff, 2);
}

i2c_state_t i2c_write_2byte_msb_lsb(i2c_t* i2c, uint8_t dev, uint8_t reg, uint16_t data)
{
    uint8_t buff[2] = {data >> 8, data & 0xFF};
    return i2c_write_mem(i2c, dev, reg, buff, 2);
}

#endif

#if CONFIG_READ_BITWISE

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

#endif

#if CONFIG_WRITE_BITWISE

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

i2c_state_t i2c_write_bits_ex(i2c_t* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end, uint8_t data)
{
    return i2c_write_bits(i2c, dev, reg, start, end - start, data);
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

// > expand
