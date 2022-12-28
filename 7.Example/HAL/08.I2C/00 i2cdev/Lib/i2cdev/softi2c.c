#include "softi2c.h"

/////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////

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
    return ack == 0 ? SOFT_I2C_OK : SOFT_I2C_ERR;
}

/////////////////////////////////////////////////////

soft_i2c_state soft_i2c_read_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len)
{
    dev &= 0xFE;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | SOFT_I2C_WRITE);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), SOFT_I2C_ERR;

    soft_i2c_send_byte(i2c, reg);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), SOFT_I2C_ERR;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | SOFT_I2C_READ);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), SOFT_I2C_ERR;

    while (--len) {
        *data++ = soft_i2c_recv_byte(i2c);
        soft_i2c_send_ack(i2c);
    }

    *data = soft_i2c_recv_byte(i2c);
    soft_i2c_send_nack(i2c);

    return soft_i2c_stop(i2c), SOFT_I2C_OK;
}

soft_i2c_state soft_i2c_write_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len)
{
    dev &= 0xFE;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | SOFT_I2C_WRITE);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), SOFT_I2C_ERR;

    soft_i2c_send_byte(i2c, reg);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), SOFT_I2C_ERR;

    while (len--) {
        soft_i2c_send_byte(i2c, *data++);
        if (soft_i2c_recv_ack(i2c))
            return soft_i2c_stop(i2c), SOFT_I2C_ERR;
    }

    return soft_i2c_stop(i2c), SOFT_I2C_OK;
}
