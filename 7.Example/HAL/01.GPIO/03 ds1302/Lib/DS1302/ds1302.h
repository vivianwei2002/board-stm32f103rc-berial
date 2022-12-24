#ifndef _DS1302_H
#define _DS1302_H

#include "gpio.h"

#define RST_W(state) HAL_GPIO_WritePin(DS1302_RST_GPIO_Port, DS1302_RST_Pin, state)
#define CLK_W(state) HAL_GPIO_WritePin(DS1302_CLK_GPIO_Port, DS1302_CLK_Pin, state)
#define DAT_W(state) HAL_GPIO_WritePin(DS1302_DAT_GPIO_Port, DS1302_DAT_Pin, state)

#define DAT_R() HAL_GPIO_ReadPin(DS1302_DAT_GPIO_Port, DS1302_DAT_Pin)

uint8_t BCD2HEX(uint8_t bcd);
uint8_t DEC2BCD(uint8_t dec);

void DS1302_Init();

uint8_t DS1302_GetYear();
uint8_t DS1302_GetMonth();
uint8_t DS1302_GetDate();
uint8_t DS1302_GetHour();
uint8_t DS1302_GetMinite();
uint8_t DS1302_GetSecond();

void DS1302_SetTime(uint8_t time[]);
void DS1302_GetTime(uint8_t time[]);

/* Ram addresses range from 0 to 30 */
void DS1302_WriteRam(uint8_t addr, uint8_t val);

/* Reads ram address 'addr' */
uint8_t DS1302_ReadRam(uint8_t addr);

/* Clears the entire ram writing 0 */
void DS1302_ClearRam(void);

/* Reads time in burst mode, includes control byte */
void DS1302_GetTime_Brust(uint8_t time[]);

/* Writes time in burst mode, includes control byte */
void DS1302_SetTime_Brust(uint8_t time[]);

/* Reads ram in burst mode 'len' bytes into 'buf' */
void DS1302_ReadRam_Brust(uint8_t len, uint8_t* buf);

/* Writes ram in burst mode 'len' bytes from 'buf' */
void DS1302_WriteRam_Brust(uint8_t len, uint8_t* buf);

#endif
