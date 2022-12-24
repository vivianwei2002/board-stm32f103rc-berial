#ifndef _SSD1306_I2C_H_
#define _SSD1306_I2C_H_

#include "i2c.h"

// 通过调整0R电阻来改变地址
#define SSD1306_ADDR_LOW 0X78   // (0x68<<1)+0
#define SSD1306_ADDR_HIGH 0X7A  // (0x68<<1)+1

#define SSD1306_DEV SSD1306_ADDR_LOW  // <--- 器件地址
#define SSD1306_I2C hi2c2             // <--- 硬件I2C

void SSD1306_WriteCmd(uint8_t cmd);
void SSD1306_WriteData(uint8_t data);
void SSD1306_Init(void);
void SSD1306_SetPos(uint8_t x, uint8_t y);
void SSD1306_Fill(uint8_t data);
void SSD1306_Clear(void);
void SSD1306_On(void);
void SSD1306_Off(void);
void SSD1306_ShowStr(uint8_t x, uint8_t y, const uint8_t ch[], uint8_t size);
void SSD1306_ShowCN(uint8_t x, uint8_t y, uint8_t N);
void SSD1306_DrawIMG(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bmp[]);
void SSD1306_ShowIMG(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t img[]);

// 绘制太空人
void Showpaceman();
// 绘制温度图标
void ShowTempLogo(uint8_t x, uint8_t y);
// 绘制湿度图标
void ShowHumidityLogo(uint8_t x, uint8_t y);
// 绘制摄氏度
void ShowDegreeLogo(uint8_t x, uint8_t y);

#endif
