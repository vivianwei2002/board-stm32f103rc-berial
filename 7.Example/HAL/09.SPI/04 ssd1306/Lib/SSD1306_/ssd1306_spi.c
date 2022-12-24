
#include "ssd1306_spi.h"
#include "font.h"

// OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127

#if OLED_MODE == 1
//向SSD1106写入一个字节。
// dat:要写入的数据/命令
// type:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WriteByte(uint8_t dat, uint8_t type) {
    DATAOUT(dat);
    if (type)
        OLED_DC_SET;
    else
        OLED_DC_CLR;
    OLED_CS_CLR;
    OLED_WR_CLR;
    OLED_WR_SET;
    OLED_CS_SET;
    OLED_DC_SET;
}
#else
//向SSD1106写入一个字节。
// dat:要写入的数据/命令
// type:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WriteByte(uint8_t dat, uint8_t type) {
    uint8_t i;
    type ? OLED_DC_SET : OLED_DC_CLR;
    OLED_CS_CLR;
    for (i = 0; i < 8; i++) {
        OLED_SCL_CLR;
        if (dat & 0x80)
            OLED_SDA_SET;
        else
            OLED_SDA_CLR;
        OLED_SCL_SET;
        dat <<= 1;
    }
    OLED_CS_SET;
    OLED_DC_SET;
}
#endif
void OLED_SetCursor(uint8_t x, uint8_t y) {
    OLED_WriteByte(0xb0 + y, OLED_CMD);
    OLED_WriteByte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WriteByte((x & 0x0f) | 0x01, OLED_CMD);
}
//开启OLED显示
void OLED_DisplayOn(void) {
    OLED_WriteByte(0X8D, OLED_CMD);  // SET DCDC命令
    OLED_WriteByte(0X14, OLED_CMD);  // DCDC ON
    OLED_WriteByte(0XAF, OLED_CMD);  // DISPLAY ON
}
//关闭OLED显示
void OLED_DisplayOff(void) {
    OLED_WriteByte(0X8D, OLED_CMD);  // SET DCDC命令
    OLED_WriteByte(0X10, OLED_CMD);  // DCDC OFF
    OLED_WriteByte(0XAE, OLED_CMD);  // DISPLAY OFF
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void) {
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        OLED_WriteByte(0xb0 + i, OLED_CMD);  //设置页地址（0~7）
        OLED_WriteByte(0x00, OLED_CMD);      //设置显示位置—列低地址
        OLED_WriteByte(0x10, OLED_CMD);      //设置显示位置—列高地址
        for (n = 0; n < 128; n++) OLED_WriteByte(0, OLED_DATA);
    }  //更新显示
}

// 在指定位置显示一个字符,包括部分字符
// mode:0,反白显示;1,正常显示
// size:选择字体 16/12
// (x:0~127,y:0~63)
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr) {
    uint8_t c = 0, i = 0;
    c = chr - ' ';  //得到偏移后的值
    if (x > Max_Column - 1) {
        x = 0;
        y = y + 2;
    }
    if (SIZE == 16) {
        OLED_SetCursor(x, y);
        for (i = 0; i < 8; i++)
            OLED_WriteByte(F8X16[c * 16 + i], OLED_DATA);
        OLED_SetCursor(x, y + 1);
        for (i = 0; i < 8; i++)
            OLED_WriteByte(F8X16[c * 16 + i + 8], OLED_DATA);
    } else {
        OLED_SetCursor(x, y + 1);
        for (i = 0; i < 6; i++)
            OLED_WriteByte(F6x8[c][i], OLED_DATA);
    }
}
// m^n函数
uint32_t oled_pow(uint8_t m, uint8_t n) {
    uint32_t result = 1;
    while (n--) result *= m;
    return result;
}
//显示2个数字
// x,y :起点坐标
// len :数字的位数
// size:字体大小
// mode:模式	0,填充模式;1,叠加模式
// num:数值(0~4294967295);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size) {
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                OLED_ShowChar(x + (size / 2) * t, y, ' ');
                continue;
            } else
                enshow = 1;
        }
        OLED_ShowChar(x + (size / 2) * t, y, temp + '0');
    }
}
//显示一个字符号串
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t* chr) {
    uint8_t j = 0;
    while (chr[j] != '\0') {
        OLED_ShowChar(x, y, chr[j]);
        x += 8;
        if (x > 120) {
            x = 0;
            y += 2;
        }
        j++;
    }
}
//显示汉字
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t no) {
    uint8_t t, adder = 0;
    OLED_SetCursor(x, y);
    for (t = 0; t < 16; t++) {
        OLED_WriteByte(font_zh[2 * no][t], OLED_DATA);
        adder += 1;
    }
    OLED_SetCursor(x, y + 1);
    for (t = 0; t < 16; t++) {
        OLED_WriteByte(font_zh[2 * no + 1][t], OLED_DATA);
        adder += 1;
    }
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]) {
    unsigned int j = 0;
    uint8_t      x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++) {
        OLED_SetCursor(x0, y);
        for (x = x0; x < x1; x++) {
            OLED_WriteByte(BMP[j++], OLED_DATA);
        }
    }
}

//初始化SSD1306
void OLED_Init(void) {
    OLED_RST_SET;
    HAL_Delay(100);
    OLED_RST_CLR;
    HAL_Delay(100);
    OLED_RST_SET;

    OLED_WriteByte(0xAE, OLED_CMD);  // turn off oled panel
    OLED_WriteByte(0x00, OLED_CMD);  // -set low column address
    OLED_WriteByte(0x10, OLED_CMD);  // -set high column address
    OLED_WriteByte(0x40, OLED_CMD);  // set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WriteByte(0x81, OLED_CMD);  // set contrast control register
    OLED_WriteByte(0xCF, OLED_CMD);  // Set SEG Output Current Brightness
    OLED_WriteByte(0xA1, OLED_CMD);  // Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WriteByte(0xC8, OLED_CMD);  // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WriteByte(0xA6, OLED_CMD);  // set normal display
    OLED_WriteByte(0xA8, OLED_CMD);  // set multiplex ratio(1 to 64)
    OLED_WriteByte(0x3f, OLED_CMD);  // 1/64 duty
    OLED_WriteByte(0xD3, OLED_CMD);  // set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WriteByte(0x00, OLED_CMD);  // not offset
    OLED_WriteByte(0xd5, OLED_CMD);  // set display clock divide ratio/oscillator frequency
    OLED_WriteByte(0x80, OLED_CMD);  // set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WriteByte(0xD9, OLED_CMD);  // set pre-charge period
    OLED_WriteByte(0xF1, OLED_CMD);  // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WriteByte(0xDA, OLED_CMD);  // set com pins hardware configuration
    OLED_WriteByte(0x12, OLED_CMD);
    OLED_WriteByte(0xDB, OLED_CMD);  // set vcomh
    OLED_WriteByte(0x40, OLED_CMD);  // Set VCOM Deselect Level
    OLED_WriteByte(0x20, OLED_CMD);  // Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WriteByte(0x02, OLED_CMD);  //
    OLED_WriteByte(0x8D, OLED_CMD);  // set Charge Pump enable/disable
    OLED_WriteByte(0x14, OLED_CMD);  // set(0x10) disable
    OLED_WriteByte(0xA4, OLED_CMD);  // Disable Entire Display On (0xa4/0xa5)
    OLED_WriteByte(0xA6, OLED_CMD);  // Disable Inverse Display On (0xa6/a7)
    OLED_WriteByte(0xAF, OLED_CMD);  // turn on oled panel

    OLED_WriteByte(0xAF, OLED_CMD); /*display ON*/
    OLED_Clear();
    OLED_SetCursor(0, 0);
}
