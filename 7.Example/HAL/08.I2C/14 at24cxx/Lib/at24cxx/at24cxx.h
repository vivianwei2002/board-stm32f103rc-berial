#ifndef _AT24CXX_H_
#define _AT24CXX_H_

#include "i2cbus/i2cbus.h"
#include <stdbool.h>

// at24cxx address (A2A1A0)
enum {
    AT24CXX_ADDRESS_A000 = 0x50,
    AT24CXX_ADDRESS_A001,
    AT24CXX_ADDRESS_A010,
    AT24CXX_ADDRESS_A011,
    AT24CXX_ADDRESS_A100,
    AT24CXX_ADDRESS_A101,
    AT24CXX_ADDRESS_A110,
    AT24CXX_ADDRESS_A111,
};

#define AT24C256                                 // <---
#define AT24CXX_DEV (AT24CXX_ADDRESS_A000 << 1)  // <---
#define AT24CXX_I2C hi2c2                        // <---

// kB
#if defined AT24C01
#define AT24CXX_MAX_SIZE 128
#elif defined AT24C02
#define AT24CXX_MAX_SIZE 256
#elif defined AT24C04
#define AT24CXX_MAX_SIZE 512
#elif defined AT24C08
#define AT24CXX_MAX_SIZE 1024
#elif defined AT24C16
#define AT24CXX_MAX_SIZE 2048
#elif defined AT24C32
#define AT24CXX_MAX_SIZE 4096
#elif defined AT24C64
#define AT24CXX_MAX_SIZE 8192
#elif defined AT24C128
#define AT24CXX_MAX_SIZE 16384
#elif defined AT24C256
#define AT24CXX_MAX_SIZE 32768
#endif

#define AT24CXX_MIN_ADDR 0
#define AT24CXX_MAX_ADDR (AT24CXX_MAX_SIZE - 1)

#define AT24CXX_PAGE_SIZE  8  // kB
#define AT24CXX_PAGE_COUNT (AT24CXX_MAX_SIZE / AT24CXX_PAGE_SIZE)

////////////////////////////////////////////

bool at24cxx_read(uint16_t addr, uint8_t* buff, uint16_t len);
bool at24cxx_write(uint16_t addr, uint8_t* buff, uint16_t len);

bool at24cxx_print(uint16_t start, uint16_t len, uint8_t wrap_num);
#define at24cxx_print_all() at24cxx_print(0, AT24CXX_MAX_ADDR, 32)

#define at24cxx_write_variable(addr, variable) at24cxx_write(addr, (uint8_t*)&variable, sizeof(variable))
#define at24cxx_read_variable(addr, variable)  at24cxx_read(addr, (uint8_t*)&variable, sizeof(variable))

#endif
