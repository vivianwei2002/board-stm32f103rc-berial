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

// return 0:success,1:fail
uint8_t soft_i2c_read_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* buff, uint16_t len) {
    dev &= 0xFE;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | WRITE);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), 1;

    soft_i2c_send_byte(i2c, reg);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), 2;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | READ);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), 3;

    while (--len) {
        *buff++ = soft_i2c_recv_byte(i2c);
        soft_i2c_send_ack(i2c);
    }

    *buff = soft_i2c_recv_byte(i2c);
    soft_i2c_send_nack(i2c);

    return soft_i2c_stop(i2c), 0;
}

// return 0:success,1:fail
uint8_t soft_i2c_write_mem(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t* buff, uint16_t len) {
    dev &= 0xFE;

    soft_i2c_start(i2c);

    soft_i2c_send_byte(i2c, dev | WRITE);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), 1;

    soft_i2c_send_byte(i2c, reg);
    if (soft_i2c_recv_ack(i2c))
        return soft_i2c_stop(i2c), 2;

    while (len--) {
        soft_i2c_send_byte(i2c, *buff++);
        if (soft_i2c_recv_ack(i2c))
            return soft_i2c_stop(i2c), 3;
    }

    return soft_i2c_stop(i2c), 0;
}

// reutrn value of register
uint8_t soft_i2c_read_1byte(soft_i2c* i2c, uint8_t dev, uint8_t reg) {
    uint8_t buff;
    if (soft_i2c_read_mem(i2c, dev, reg, &buff, 1)) return 0;
    return buff;
}

// reutrn value of register
uint16_t soft_i2c_read_2byte(soft_i2c* i2c, uint8_t dev, uint8_t reg_lsb, uint8_t reg_msb) {
    uint8_t lsb, msb;
    if (soft_i2c_read_mem(i2c, dev, reg_lsb, &lsb, 1)) return 0;
    if (soft_i2c_read_mem(i2c, dev, reg_msb, &msb, 1)) return 0;
    return (msb << 8) | lsb;
}

// return 0:success,1:fail
uint8_t soft_i2c_write_1byte(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t buff) {
    return soft_i2c_write_mem(i2c, dev, reg, &buff, 1);
}

// reutrn bit of register
uint8_t soft_i2c_read_bit(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t bit /* bit index: 0~7 */) {
    uint8_t buff;

    if (soft_i2c_read_mem(i2c, dev, reg, &buff, 1))
        return -1;

    return (buff >> (bit & 0x07)) & 0x01;
}

// return 0:success,1:fail
uint8_t soft_i2c_write_bit(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t bit, uint8_t value) {
    uint8_t buff;

    if (soft_i2c_read_mem(i2c, dev, reg, &buff, 1))
        return -1;

    bit &= 0x07;

    if (value) {
        buff |= (1 << bit);
    } else {
        buff &= ~(1 << bit);
    }

    return soft_i2c_write_mem(i2c, dev, reg, &buff, 1);
}

// reutrn bits with mask of register
uint8_t soft_i2c_read_bits(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t mask) {
    uint8_t buff;

    if (soft_i2c_read_mem(i2c, dev, reg, &buff, 1))
        return -1;

    return buff & mask;
}

// return 0:success,1:fail
uint8_t soft_i2c_write_bits(soft_i2c* i2c, uint8_t dev, uint8_t reg, uint8_t mask, uint8_t value) {
    uint8_t buff;

    if (soft_i2c_read_mem(i2c, dev, reg, &buff, 1))
        return -1;

    buff &= ~mask;  // clear area where are set 1

    buff |= (mask & value);  // set value

    return soft_i2c_write_mem(i2c, dev, reg, &buff, 1);
}