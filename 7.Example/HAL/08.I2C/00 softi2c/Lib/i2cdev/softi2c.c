#include "softi2c.h"

void soft_i2c_init(soft_i2c* i2c)
{
    i2c->SDA_OUT();
    i2c->SDA_1();
    i2c->SCL_1();
}

void soft_i2c_start(soft_i2c* i2c)
{
    i2c->SDA_OUT();
    i2c->SDA_1();  // sda=1
    i2c->delay();
    i2c->SCL_1();  // scl=1
    i2c->delay();
    i2c->SDA_0();  // sda=0
    i2c->delay();
    i2c->SCL_0();  // scl=0
    i2c->delay();
}

void soft_i2c_stop(soft_i2c* i2c)
{
    i2c->SDA_OUT();
    i2c->SCL_0();  // scl=0
    i2c->delay();
    i2c->SDA_0();  // sda=0
    i2c->delay();
    i2c->SCL_1();  // scl=1
    i2c->delay();
    i2c->SDA_1();  // sda=1
    i2c->delay();
}

void soft_i2c_send_ack(soft_i2c* i2c)
{
    i2c->SDA_OUT();
    i2c->SCL_0();  // scl=0
    i2c->delay();
    i2c->SDA_0();  // sda=0
    i2c->delay();
    i2c->SCL_1();  // scl=1
    i2c->delay();
    i2c->SCL_0();  // scl=0
    i2c->delay();
}

void soft_i2c_send_nack(soft_i2c* i2c)
{
    i2c->SDA_OUT();
    i2c->SCL_0();  // scl=0
    i2c->delay();
    i2c->SDA_1();  // sda=1
    i2c->delay();
    i2c->SCL_1();  // scl=1
    i2c->delay();
    i2c->SCL_0();  // scl=0
    i2c->delay();
}

// return 0:ack,1:nack
uint8_t soft_i2c_recv_ack(soft_i2c* i2c)
{
    uint8_t timeout = 0;

    i2c->SDA_1();  // sda=1
    i2c->delay();
    i2c->SCL_1();  // scl=1
    i2c->delay();

    i2c->SDA_IN();
    while (i2c->SDA())
        if (++timeout > 250)
            return 1;  // nack

    i2c->SCL_0();  // scl=0
    i2c->delay();

    return 0;  // ack
}

void soft_i2c_send_byte(soft_i2c* i2c, uint8_t data)
{
    uint8_t mask = 0x80;
    i2c->SDA_OUT();
    do {
        (data & mask) ?
            i2c->SDA_1() :  // sda = 1
            i2c->SDA_0();   // sda = 0
        i2c->delay();
        i2c->SCL_1();  // scl = 1
        i2c->delay();
        i2c->SCL_0();  // scl = 0
        i2c->delay();
    } while (mask >>= 1);
}

// return 0:success,1:fail
uint8_t soft_i2c_recv_byte(soft_i2c* i2c)
{
    uint8_t mask = 0x80, data = 0x00;
    i2c->SDA_IN();
    do {
        i2c->SCL_1();
        i2c->delay();
        if (i2c->SDA())
            data |= mask;
        i2c->SCL_0();
        i2c->delay();
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

    soft_i2c_send_byte(i2c, dev | WRITE);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), SOFT_I2C_ERR;

    soft_i2c_send_byte(i2c, reg);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), SOFT_I2C_ERR;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | READ);
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

    soft_i2c_send_byte(i2c, dev | WRITE);
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
