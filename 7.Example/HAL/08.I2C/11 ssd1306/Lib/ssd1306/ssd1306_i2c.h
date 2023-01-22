#ifndef _SSD1306_I2C_H_
#define _SSD1306_I2C_H_

#include "i2c.h"

// 通过调整0R电阻来改变地址
#define SSD1306_ADDR_LOW  0X78  // (0x68<<1)+0
#define SSD1306_ADDR_HIGH 0X7A  // (0x68<<1)+1

#define SSD1306_DEV SSD1306_ADDR_LOW  // <--- 器件地址
#define SSD1306_I2C hi2c2             // <--- 硬件I2C

#define FONT_ENABLE 0
#if FONT_ENABLE == 1
#define FONT_ENABLE_CN_16X16   1
#define FONT_ENABLE_ASCII_6X8  1
#define FONT_ENABLE_ASCII_8X16 1
#endif

////////////////// functions

void ssd1306_write_cmd(uint8_t cmd);
void ssd1306_write_data(uint8_t data);
void ssd1306_init(void);
void ssd1306_set_cursor(uint8_t x, uint8_t y);
void ssd1306_fill(uint8_t data);
void ssd1306_clear(void);
void ssd1306_display_on(void);
void ssd1306_display_off(void);
#if FONT_ENABLE
#if FONT_ENABLE_ASCII_6X8 || FONT_ENABLE_ASCII_8X16
void ssd1306_show_str(uint8_t x, uint8_t y, const uint8_t ch[], uint8_t size);
#endif
#if FONT_ENABLE_CN_16X16
void ssd1306_show_cn(uint8_t x, uint8_t y, uint8_t N);
#endif
#endif
void ssd1306_show_img(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t img[]);
void ssd1306_show_anim(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t frames, uint8_t fps, const uint8_t imgs[]);
void ssd1306_fill_img(const uint8_t img[1024]);
void ssd1306_fill_anim(uint8_t frames, uint8_t fps, const uint8_t imgs[][1024]);

#endif
