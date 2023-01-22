#include "stm32f10x.h"
#include "math.h"
#include <stdlib.h>
#include "I2C.h"

uint8_t OLED_RAM[8][128];

void OLED_I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Light_A_Pixel(uint8_t x, uint8_t y)
{
    uint8_t page_idx = 7 - (y - 1) / 8;
    uint8_t page_y   = (y - 1) % 8;
    OLED_RAM[page_idx][x - 1] |= 0x80 >> page_y;
}

void OLED_display()
{
    uint8_t i, j;
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
        ;
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ;
    I2C_Send7bitAddress(I2C1, 0X78, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;
    I2C_SendData(I2C1, 0x40);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        ;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 128; j++) {
            I2C_SendData(I2C1, OLED_RAM[i][j]);
            OLED_RAM[i][j] = 0;
            while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
                ;
        }
    }
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void OLED_Clear(void)
{
    uint8_t i, j;
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
        ;
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ;
    I2C_Send7bitAddress(I2C1, 0X78, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;
    I2C_SendData(I2C1, 0x40);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        ;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 128; j++) {
            OLED_RAM[i][j] = 0;
            I2C_SendData(I2C1, OLED_RAM[i][j]);
            while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
                ;
        }
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    int8_t  dx  = x1 - x0;
    int8_t  dy  = y1 - y0;
    int8_t  DIR = 1;
    int16_t d;
    if (abs(dx) >= abs(dy)) {
        uint8_t x_start, x_end, yi;
        d = -dx;
        if (dx >= 0) {
            x_start = x0;
            x_end   = x1;
            yi      = y0;
            if (dy < 0)
                DIR = -1;
            else if (dy == 0)
                DIR = 0;
        } else {
            x_start = x1;
            x_end   = x0;
            yi      = y1;
            d       = -d;
            if (dy > 0)
                DIR = -1;
        }
        for (; x_start <= x_end; x_start++) {
            if (d > 0) {
                yi = yi + DIR;
                d  = d - 2 * abs(dx);
            }
            Light_A_Pixel((uint8_t)x_start, (uint8_t)yi);
            d = d + 2 * abs(dy);
        }
    } else {
        uint8_t y_start, y_end, xi;
        d = -dy;
        if (dy >= 0) {
            y_start = y0;
            y_end   = y1;
            xi      = x0;
            if (dx < 0)
                DIR = -1;
        } else {
            y_start = y1;
            y_end   = y0;
            xi      = x1;
            d       = -d;
            if (dx > 0)
                DIR = -1;
            else if (dx == 0)
                DIR = 0;
        }
        for (; y_start <= y_end; y_start++) {
            if (d > 0) {
                xi = xi + DIR;
                d  = d - 2 * abs(dy);
            }
            Light_A_Pixel((uint8_t)xi, (uint8_t)y_start);
            d = d + 2 * abs(dx);
        }
    }
}

void OLED_Init(void)
{
    uint32_t i, j;

    for (i = 0; i < 1000; i++)  // 上电延时
    {
        for (j = 0; j < 1000; j++)
            ;
    }

    IIC_Init();  // 端口初始化

    OLED_WriteCommand(0xAE);  // 关闭显示

    OLED_WriteCommand(0xD5);  // 设置显示时钟分频比/振荡器频率
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8);  // 设置多路复用率
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3);  // 设置显示偏移
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40);  // 设置显示开始行

    OLED_WriteCommand(0xA1);  // 设置左右方向，0xA1正常 0xA0左右反置

    OLED_WriteCommand(0xC8);  // 设置上下方向，0xC8正常 0xC0上下反置

    OLED_WriteCommand(0xDA);  // 设置COM引脚硬件配置
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81);  // 设置对比度控制
    OLED_WriteCommand(0xCF);

    OLED_WriteCommand(0xD9);  // 设置预充电周期
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB);  // 设置VCOMH取消选择级别
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4);  // 设置整个显示打开/关闭

    OLED_WriteCommand(0xA6);  // 设置正常/倒转显示

    OLED_WriteCommand(0x8D);  // 设置充电泵
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0x20);  // 设置横向地址模式
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x21);
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(127);

    OLED_WriteCommand(0x22);
    OLED_WriteCommand(0);
    OLED_WriteCommand(7);

    OLED_WriteCommand(0xAF);  // 开启显示

    OLED_Clear();  // OLED清屏
}
