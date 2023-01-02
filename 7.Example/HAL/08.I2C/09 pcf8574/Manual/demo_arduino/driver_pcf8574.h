
#ifndef DRIVER_PCF8574_H
#define DRIVER_PCF8574_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    PCF8574_ADDRESS_A000 = 0,  // A2A1A0 000
    PCF8574_ADDRESS_A001 = 1,  // A2A1A0 001
    PCF8574_ADDRESS_A010 = 2,  // A2A1A0 010
    PCF8574_ADDRESS_A011 = 3,  // A2A1A0 011
    PCF8574_ADDRESS_A100 = 4,  // A2A1A0 100
    PCF8574_ADDRESS_A101 = 5,  // A2A1A0 101
    PCF8574_ADDRESS_A110 = 6,  // A2A1A0 110
    PCF8574_ADDRESS_A111 = 7,  // A2A1A0 111
} pcf8574_address_t;
typedef enum {
    PCF8574_PIN_0 = 0x00,  // pin 0
    PCF8574_PIN_1 = 0x01,  // pin 1
    PCF8574_PIN_2 = 0x02,  // pin 2
    PCF8574_PIN_3 = 0x03,  // pin 3
    PCF8574_PIN_4 = 0x04,  // pin 4
    PCF8574_PIN_5 = 0x05,  // pin 5
    PCF8574_PIN_6 = 0x06,  // pin 6
    PCF8574_PIN_7 = 0x07,  // pin 7
} pcf8574_pin_t;
typedef enum {
    PCF8574_PIN_LEVEL_LOW  = 0x00,  // low level
    PCF8574_PIN_LEVEL_HIGH = 0x01,  // high level
} pcf8574_pin_level_t;
typedef struct pcf8574_handle_s {
    uint8_t iic_addr;  // iic device address

    uint8_t inited;  // inited flag
} pcf8574_handle_t;

uint8_t pcf8574_info(pcf8574_info_t* info);
uint8_t pcf8574_set_addr_pin(pcf8574_handle_t* handle, pcf8574_address_t addr_pin);
uint8_t pcf8574_get_addr_pin(pcf8574_handle_t* handle, pcf8574_address_t* addr_pin);
uint8_t pcf8574_init(pcf8574_handle_t* handle);
uint8_t pcf8574_deinit(pcf8574_handle_t* handle);
uint8_t pcf8574_read(pcf8574_handle_t* handle, pcf8574_pin_t pin, pcf8574_pin_level_t* level);
uint8_t pcf8574_write(pcf8574_handle_t* handle, pcf8574_pin_t pin, pcf8574_pin_level_t level);
uint8_t pcf8574_set_reg(pcf8574_handle_t* handle, uint8_t* buf, uint16_t len);
uint8_t pcf8574_get_reg(pcf8574_handle_t* handle, uint8_t* buf, uint16_t len);
#ifdef __cplusplus
}
#endif
#endif
