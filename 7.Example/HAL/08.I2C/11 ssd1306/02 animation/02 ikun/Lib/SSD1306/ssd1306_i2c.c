
#include "ssd1306_i2c.h"
#include "font.h"

// 写命令
void SSD1306_WriteCmd(uint8_t cmd)
{
    HAL_I2C_Mem_Write(&SSD1306_I2C, SSD1306_DEV, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 0xFF);
}

// 写数据
void SSD1306_WriteData(uint8_t data)
{
    HAL_I2C_Mem_Write(&SSD1306_I2C, SSD1306_DEV, 0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xFF);
}

// 初始化
void SSD1306_Init(void)
{
    HAL_Delay(500);  // 这里的延时很重要

    SSD1306_WriteCmd(0xAE);  // display off
    SSD1306_WriteCmd(0x20);  // Set Memory Addressing Mode
    SSD1306_WriteCmd(0x10);  // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    SSD1306_WriteCmd(0xb0);  // Set Page Start Address for Page Addressing Mode,0-7
    SSD1306_WriteCmd(0xc8);  // Set COM Output Scan Direction
    SSD1306_WriteCmd(0x00);  // set low column address
    SSD1306_WriteCmd(0x10);  // -set high column address
    SSD1306_WriteCmd(0x40);  // set start line address
    SSD1306_WriteCmd(0x81);  // set contrast control register
    SSD1306_WriteCmd(0xff);  // 亮度调节 0x00~0xff
    SSD1306_WriteCmd(0xa1);  // set segment re-map 0 to 127
    SSD1306_WriteCmd(0xa6);  // set normal display
    SSD1306_WriteCmd(0xa8);  // set multiplex ratio(1 to 64)
    SSD1306_WriteCmd(0x3F);
    SSD1306_WriteCmd(0xa4);  // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    SSD1306_WriteCmd(0xd3);  // -set display offset
    SSD1306_WriteCmd(0x00);  // -not offset
    SSD1306_WriteCmd(0xd5);  // set display clock divide ratio/oscillator frequency
    SSD1306_WriteCmd(0xf0);  // set divide ratio
    SSD1306_WriteCmd(0xd9);  // set pre-charge period
    SSD1306_WriteCmd(0x22);  //
    SSD1306_WriteCmd(0xda);  // set com pins hardware configuration
    SSD1306_WriteCmd(0x12);
    SSD1306_WriteCmd(0xdb);  // set vcomh
    SSD1306_WriteCmd(0x20);  // 0x20,0.77xVcc
    SSD1306_WriteCmd(0x8d);  // set DC-DC enable
    SSD1306_WriteCmd(0x14);
    SSD1306_WriteCmd(0xaf);  // turn on SSD1306 panel
}

// 设置起始点坐标
void SSD1306_SetPos(uint8_t x, uint8_t y)
{
    SSD1306_WriteCmd(0xb0 + y);
    SSD1306_WriteCmd(((x & 0xf0) >> 4) | 0x10);
    SSD1306_WriteCmd((x & 0x0f) | 0x01);
}

// 全屏填充(全亮:0xFF,全灭:0x00)
void SSD1306_Fill(uint8_t data)
{
    uint8_t m, n;
    for (m = 0; m < 8; m++) {
        SSD1306_WriteCmd(0xb0 + m);  // page0 - page1
        SSD1306_WriteCmd(0x00);      // low column start address
        SSD1306_WriteCmd(0x10);      // high column start address
        for (n = 0; n < 128; n++) {
            SSD1306_WriteData(data);
        }
    }
}

// 清屏
void SSD1306_Clear(void)
{
    SSD1306_Fill(0x00);
}

// 唤醒
void SSD1306_On(void)
{
    SSD1306_WriteCmd(0X8D);  // 设置电荷泵
    SSD1306_WriteCmd(0X14);  // 开启电荷泵
    SSD1306_WriteCmd(0XAF);  // SSD1306唤醒
}

// 休眠(功耗<10uA)
void SSD1306_Off(void)
{
    SSD1306_WriteCmd(0X8D);  // 设置电荷泵
    SSD1306_WriteCmd(0X10);  // 关闭电荷泵
    SSD1306_WriteCmd(0XAE);  // SSD1306休眠
}

// 显示 ascii 字符 ( size = 1: 6*8, 2: 8*16)
void SSD1306_ShowStr(uint8_t x, uint8_t y, const uint8_t str[], uint8_t size)
{
    uint8_t c = 0, i = 0, j = 0;
    switch (size) {
        case 1: {
            while (str[j] != '\0') {
                c = str[j] - 32;
                if (x > 126) {
                    x = 0;
                    ++y;
                }
                SSD1306_SetPos(x, y);
                for (i = 0; i < 6; i++)
                    SSD1306_WriteData(F6x8[c][i]);
                x += 6;
                ++j;
            }
        } break;
        case 2: {
            while (str[j] != '\0') {
                c = str[j] - 32;
                if (x > 120) {
                    x = 0;
                    ++y;
                }
                SSD1306_SetPos(x, y);
                for (i = 0; i < 8; i++)
                    SSD1306_WriteData(F8X16[c * 16 + i]);
                SSD1306_SetPos(x, y + 1);
                for (i = 0; i < 8; i++)
                    SSD1306_WriteData(F8X16[c * 16 + i + 8]);
                x += 8;
                ++j;
            }
        } break;
    }
}

// 显示汉字（需先取模，16*16点阵，N 为 汉字在 font.h 中的索引）
void SSD1306_ShowCN(uint8_t x, uint8_t y, uint8_t N)
{
    uint8_t  wm  = 0;
    uint32_t idx = 32 * N;
    SSD1306_SetPos(x, y);
    for (wm = 0; wm < 16; ++wm) {
        SSD1306_WriteData(F16x16[idx++]);
    }
    SSD1306_SetPos(x, y + 1);
    for (wm = 0; wm < 16; ++wm) {
        SSD1306_WriteData(F16x16[idx++]);
    }
}

// 显示BMP位图 (x:0~127, y:0~7)
void SSD1306_DrawIMG(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t img[])
{
    uint32_t j = 0;
    uint8_t  x, y;
    y = y1 / 8;
    if (y1 % 8 == 0) ++y;
    for (y = y0; y < y1; ++y) {
        SSD1306_SetPos(x0, y);
        for (x = x0; x < x1; ++x, ++j)
            SSD1306_WriteData(img[j]);
    }
}

void SSD1306_ShowIMG(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t img[])
{
    SSD1306_DrawIMG(x, y, x + w, y + h, img);
}

void SSD1306_ShowAnim(uint8_t frames, uint8_t fps, const uint8_t anim[][1024])
{
    for (uint8_t i = 0; i < frames; ++i) {
        SSD1306_SetPos(0, 0);
        HAL_I2C_Mem_Write(&SSD1306_I2C, SSD1306_DEV, 0x40, I2C_MEMADD_SIZE_8BIT, anim[i], 1024, 0xFF);
        // SSD1306_ShowIMG(0, 0, 127, 7, anim[i]);
        HAL_Delay(1000 / fps);
    }
}

void ShowCXK() { SSD1306_ShowIMG(0, 0, 127, 7, cxk); }
