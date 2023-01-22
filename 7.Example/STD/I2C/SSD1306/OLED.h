#ifndef __OLED_H

#define __OLED_H

void OLED_Init(void);
void Light_A_Pixel(uint8_t x, uint8_t y);
void DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void OLED_display(void);
void OLED_Clear(void);
#endif
