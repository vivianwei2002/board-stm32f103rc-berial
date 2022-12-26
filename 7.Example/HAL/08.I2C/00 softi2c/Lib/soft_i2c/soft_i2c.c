#include "soft_i2c.h"

void soft_i2c_init(soft_i2c* i2c) {
    i2c->SDA_OUT();
    i2c->SDA_1();
    i2c->SCL_1();
}

void soft_i2c_start(soft_i2c* i2c) {
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

void soft_i2c_stop(soft_i2c* i2c) {
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

void soft_i2c_send_ack(soft_i2c* i2c) {
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

void soft_i2c_send_nack(soft_i2c* i2c) {
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
uint8_t soft_i2c_recv_ack(soft_i2c* i2c) {
    uint8_t timeout = 0;

    i2c->SDA_1();  // sda=1
    i2c->delay();
    i2c->SCL_1();  // scl=1
    i2c->delay();

    i2c->SDA_IN();
    while (i2c->SDA()) {
        if (++timeout > 250) {
            return 1;  // nack
        }
    }

    i2c->SCL_0();  // scl=0
    i2c->delay();

    return 0;  // ack
}

void soft_i2c_send_byte(soft_i2c* i2c, uint8_t data) {
    uint8_t mask = 0x80;
    i2c->SDA_OUT();
    do {
        (data & mask)
            ? i2c->SDA_1()   // sda = 1
            : i2c->SDA_0();  // sda = 0
        i2c->delay();
        i2c->SCL_1();  // scl = 1
        i2c->delay();
        i2c->SCL_0();  // scl = 0
        i2c->delay();
    } while (mask >>= 1);
}

// return 0:success,1:fail
uint8_t soft_i2c_recv_byte(soft_i2c* i2c) {
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

// return 0:no,1:yes
uint8_t soft_i2c_check(soft_i2c* i2c, uint8_t dev) {
    uint8_t ack;
    soft_i2c_start(i2c);
    soft_i2c_send_byte(i2c, dev);
    ack = soft_i2c_recv_ack(i2c);
    soft_i2c_stop(i2c);
    return ack == 0;
}

uint8_t soft_i2c_scan(soft_i2c* i2c) {
    uint8_t addr_7bits, cnt = 0;
    for (addr_7bits = 0; addr_7bits < 0x80; ++addr_7bits) {
        if (soft_i2c_check(i2c, addr_7bits << 1)) {
            printf("0x%2x(%d)\r\n", addr_7bits, addr_7bits);
            ++cnt;
        }
    }
    printf("scan finished\r\n");
    return cnt;
}

/////////////////////////////////////////////////////

soft_i2c_state soft_i2c_read_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len) {
    dev &= 0xFE;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | WRITE);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), I2C_ERR;

    soft_i2c_send_byte(i2c, reg);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), I2C_ERR;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | READ);
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

soft_i2c_state soft_i2c_write_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data, uint16_t len) {
    dev &= 0xFE;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | WRITE);
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

/////////////////////////////////////////////////////

soft_i2c_state soft_i2c_read_1byte(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* data) {
    return soft_i2c_read_mem(i2c, dev, reg, &data, 1);
}

soft_i2c_state soft_i2c_read_2byte(soft_i2c* i2c, uint8_t dev, uint8_t reg_lsb, uint8_t reg_msb, uint16_t* data) {
    uint8_t lsb, msb;
    if (soft_i2c_read_mem(i2c, dev, reg_lsb, &lsb, 1) == I2C_ERR) return I2C_ERR;
    if (soft_i2c_read_mem(i2c, dev, reg_msb, &msb, 1) == I2C_ERR) return I2C_ERR;
    *data = (msb << 8) | lsb;
    return I2C_OK;
}

soft_i2c_state soft_i2c_write_1byte(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t data) {
    return soft_i2c_write_mem(i2c, dev, reg, &data, 1);
}

soft_i2c_state soft_i2c_read_bit(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t bit /* bit index: 0~7 */, uint8_t* data) {
    uint8_t buff;

    if (soft_i2c_read_mem(i2c, dev, reg, &buff, 1) == I2C_ERR) return I2C_ERR;

    *data = (buff >> (bit & 0x07)) & 0x01;

    return I2C_ERR;
}

soft_i2c_state soft_i2c_write_bit(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t bit, uint8_t data) {
    uint8_t buff;

    if (soft_i2c_read_mem(i2c, dev, reg, &buff, 1)) return I2C_ERR;

    bit &= 0x07;

    if (data) {
        buff |= (1 << bit);
    } else {
        buff &= ~(1 << bit);
    }

    return soft_i2c_write_mem(i2c, dev, reg, &buff, 1);
}

soft_i2c_state soft_i2c_read_bits(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len, uint8_t* data) {
    uint8_t buff;

    if (soft_i2c_read_mem(i2c, dev, reg, &buff, 1)) return I2C_ERR;

    *data = buff & (0xFF >> (8 - len) << start);

    return I2C_OK;
}

soft_i2c_state soft_i2c_write_bits(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t len, uint8_t data) {
    uint8_t buff, mask;

    if (soft_i2c_read_mem(i2c, dev, reg, &buff, 1)) return I2C_ERR;

    mask = 0xFF >> (8 - len) << start;

    buff &= ~mask;
    buff |= mask & data;

    return soft_i2c_write_mem(i2c, dev, reg, &buff, 1);
}

soft_i2c_state soft_i2c_read_bits_ex(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end, uint8_t* data) {
    return soft_i2c_read_bits(i2c, dev, reg, start, end - start, data);
}

soft_i2c_state soft_i2c_write_bits_ex(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t start, uint8_t end, uint8_t data) {
}

soft_i2c_state soft_i2c_read_mask(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t mask, uint8_t* data) {
    uint8_t buff;

    if (soft_i2c_read_mem(i2c, dev, reg, &buff, 1)) return I2C_ERR;

    *data = buff & mask;

    return I2C_OK;
}

soft_i2c_state soft_i2c_write_mask(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t mask, uint8_t data) {
    uint8_t buff;

    if (soft_i2c_read_mem(i2c, dev, reg, &buff, 1)) return I2C_ERR;

    buff &= ~mask;  // clear area where are set 1

    buff |= (mask & data);  // set value

    return soft_i2c_write_mem(i2c, dev, reg, &buff, 1);
}
