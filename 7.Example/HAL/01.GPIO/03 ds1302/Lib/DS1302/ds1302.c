#include "ds1302.h"

// !!!!!!! 电源接5V, 不能接3.3V, 否则秒会卡在85，其他为会卡在 165
// 外接的 32.768K 晶振要用内部电容为 6PF 的。

#define DS1302_SECOND 0x80  // 秒 0-59
// [bit7]CH: 0 时钟工作; 1 时钟停振，进入低功耗态;

#define DS1302_MINUTE 0x82  // 分 0-59
#define DS1302_HOUR 0x84    // 时 1-12/1-24
#define DS1302_DATE 0x86    // 日 1-31
#define DS1302_MONTH 0x88   // 月 1-12
#define DS1302_DAY 0x8A     // 星期 1-7
#define DS1302_YEAR 0x8C    // 年 0-99

#define DS1302_CONTROL 0x8E  // [bit7] WP: 0不保护 1保护

#define DS1302_TRACKLE_CHARGER 0x90  // 涓流充电
// [bit7-4]TCS: 1010 慢充电
// [bit3-2]DS:  二极管个数选择。    01: 1个二极管; 10: 2个二极管; 11或00，禁止充电。
// [bit1-0]RS:  二极管串联电阻选择。00，不充电；01，2KΩ电阻；10，4KΩ电阻；11，8KΩ电阻。

#define DS1302_CLOCK_BURST 0xBE
#define DS1302_RAM_BURST 0xFE
// burst mode 就是必须1次读写全部

#define DS1302_RAMSIZE 0x31
#define DS1302_RAMSTART 0xC0
#define DS1302_RAMEND 0xFC

// uint8_t HEX2BCD(uint8_t hex) { return hex % 10 + hex / 10 * 16; }
// uint8_t BCD2HEX(uint8_t bcd) { return bcd % 16 + bcd / 16 * 10; }
uint8_t HEX2BCD(uint8_t hex) { return ((hex / 10) << 4) + (hex % 10); }
uint8_t BCD2HEX(uint8_t bcd) { return ((bcd >> 4) * 10) + (bcd & 0x0f); }

void DAT_OUT() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin              = DS1302_DAT_Pin;
    GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull             = GPIO_NOPULL;
    GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DS1302_DAT_GPIO_Port, &GPIO_InitStruct);
}

void DAT_IN() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin              = DS1302_DAT_Pin;
    GPIO_InitStruct.Mode             = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull             = GPIO_PULLUP;
    GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DS1302_DAT_GPIO_Port, &GPIO_InitStruct);
}

static void DS1302_Write(uint8_t addr, uint8_t data) {
    uint8_t i;

    DAT_OUT();

    RST_W(0);
    CLK_W(0);
    RST_W(1);
    addr &= 0xFE;

    // write address
    for (i = 0; i < 8; i++) {
        DAT_W(addr & 0x01);
        addr >>= 1;
        CLK_W(1);
        CLK_W(0);
    }

    // write data
    for (i = 0; i < 8; i++) {
        DAT_W(data & 0x01);
        data >>= 1;
        CLK_W(1);
        CLK_W(0);
    }

    RST_W(0);
    DAT_W(0);
}

static uint8_t DS1302_Read(uint8_t addr) {
    uint8_t data = 0, i;

    DAT_OUT();
    CLK_W(0);
    RST_W(0);

    RST_W(1);

    addr |= 0x01;
    for (i = 0; i < 8; i++) {
        DAT_W(addr & 0x01);
        addr >>= 1;
        CLK_W(1);
        CLK_W(0);
    }

    DAT_IN();

    for (i = 0; i < 8; i++) {
        data >>= 1;
        if (DAT_R())
            data |= 0x80;
        CLK_W(1);
        CLK_W(0);
    }

    RST_W(0);
    DAT_OUT();
    return data;
}

void DS1302_Init() {
    RST_W(0);
    CLK_W(0);
    DAT_W(0);
}

uint8_t DS1302_GetYear() { return BCD2HEX(DS1302_Read(DS1302_YEAR)); }
uint8_t DS1302_GetMonth() { return BCD2HEX(DS1302_Read(DS1302_MONTH)); }
uint8_t DS1302_GetDate() { return BCD2HEX(DS1302_Read(DS1302_DATE)); }
uint8_t DS1302_GetHour() { return BCD2HEX(DS1302_Read(DS1302_HOUR)); }
uint8_t DS1302_GetMinite() { return BCD2HEX(DS1302_Read(DS1302_MINUTE)); }
uint8_t DS1302_GetSecond() { return BCD2HEX(DS1302_Read(DS1302_SECOND)); }

void DS1302_SetTime(uint8_t time[]) {
    DS1302_Write(DS1302_CONTROL, 0x00);  // Disable write protection
    DS1302_Write(DS1302_SECOND, 0x80);   // [bit7] the clock halt flag: 1 stop clock
    DS1302_Write(DS1302_YEAR, HEX2BCD(time[0]));
    DS1302_Write(DS1302_MONTH, HEX2BCD(time[1]));
    DS1302_Write(DS1302_DATE, HEX2BCD(time[2]));
    DS1302_Write(DS1302_HOUR, HEX2BCD(time[3]));  // [bit7] 0:24小时制 1:12小时制,
    DS1302_Write(DS1302_MINUTE, HEX2BCD(time[4]));
    DS1302_Write(DS1302_SECOND, HEX2BCD(time[5]));
    DS1302_Write(DS1302_CONTROL, 0x80);  // Enable write protection
}

void DS1302_GetTime(uint8_t time[]) {
    time[0] = BCD2HEX(DS1302_Read(DS1302_YEAR));
    time[1] = BCD2HEX(DS1302_Read(DS1302_MONTH));
    time[2] = BCD2HEX(DS1302_Read(DS1302_DATE));
    time[3] = BCD2HEX(DS1302_Read(DS1302_HOUR));
    time[4] = BCD2HEX(DS1302_Read(DS1302_MINUTE));
    time[5] = BCD2HEX(DS1302_Read(DS1302_SECOND) & 0x7F);  // clear hsb
}

void DS1302_StopTime() {  // 停止计时
    DS1302_Write(DS1302_CONTROL, 0x00);
    DS1302_Write(DS1302_SECOND, DS1302_Read(DS1302_SECOND) | 0x80);
    DS1302_Write(DS1302_CONTROL, 0x80);
}

void DS1302_WriteRam(uint8_t addr, uint8_t val) {
    DS1302_Write(DS1302_CONTROL, 0x00);
    if (addr >= DS1302_RAMSIZE) return;
    DS1302_Write(DS1302_RAMSTART + (2 * addr), val);
    DS1302_Write(DS1302_CONTROL, 0x80);
}

uint8_t DS1302_ReadRam(uint8_t addr) {
    if (addr >= DS1302_RAMSIZE) return 0;
    return DS1302_Read(DS1302_RAMSTART + (2 * addr));
}

void DS1302_ClearRam(void) {
    for (uint8_t i = 0; i < DS1302_RAMSIZE; i++)
        DS1302_WriteRam(i, 0x00);
}

void DS1302_Write_Brust(uint8_t addr, uint8_t* pbuf, uint8_t len) {
    uint8_t i, j;
    uint8_t data;

    DS1302_Write(DS1302_CONTROL, 0x00);

    DAT_OUT();
    CLK_W(0);
    RST_W(1);

    for (i = 0; i < 8; i++) {
        DAT_W(addr & 0x01);
        addr >>= 1;
        CLK_W(1);
        CLK_W(0);
    }

    for (j = 0; j < len; j++) {
        data = pbuf[j];
        for (i = 0; i < 8; i++) {
            DAT_W(data & 0x01);
            data >>= 1;
            CLK_W(1);
            CLK_W(0);
        }
    }

    RST_W(0);
    DAT_W(0);

    DS1302_Write(DS1302_CONTROL, 0x80);
}

void DS1302_Read_Brust(uint8_t addr, uint8_t* pbuf, uint8_t len) {
    uint8_t i, j;
    uint8_t data;

    DS1302_Write(DS1302_CONTROL, 0x00);  // disable write protection

    DAT_OUT();
    CLK_W(0);
    RST_W(1);

    addr |= 0x01;
    for (i = 0; i < 8; i++) {
        DAT_W(addr & 0x01);
        addr >>= 1;
        CLK_W(1);
        CLK_W(0);
    }

    DAT_IN();

    for (j = 0; j < len; j++) {
        for (i = 0; i < 8; i++) {
            data >>= 1;
            if (DAT_R())
                data |= 0x80;
            CLK_W(1);
            CLK_W(0);
        }
        pbuf[j] = data;
    }
    RST_W(0);
    DAT_OUT();
}

void DS1302_GetTime_Brust(uint8_t time[]) {
    uint8_t temp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    DS1302_Read_Brust(DS1302_CLOCK_BURST, temp, 8);
    time[0] = BCD2HEX(temp[6]);  // Year
    time[1] = BCD2HEX(temp[4]);  // Month
    time[2] = BCD2HEX(temp[3]);  // Date
    time[3] = BCD2HEX(temp[2]);  // Hour
    time[4] = BCD2HEX(temp[1]);  // Min
    time[5] = BCD2HEX(temp[0]);  // Sec
    time[6] = BCD2HEX(temp[5]);  // Day
    time[7] = temp[7];           // Control, bit7 is WP bit
}

void DS1302_SetTime_Brust(uint8_t time[]) {
    uint8_t temp[8];

    temp[0] = HEX2BCD(time[5]);  // Sec
    temp[1] = HEX2BCD(time[4]);  // Min
    temp[2] = HEX2BCD(time[3]);  // Hour
    temp[3] = HEX2BCD(time[2]);  // Date
    temp[4] = HEX2BCD(time[1]);  // Month
    temp[5] = HEX2BCD(time[6]);  // Day
    temp[6] = HEX2BCD(time[0]);  // Year
    temp[7] = 0;                 // Control

    DS1302_Write_Brust(DS1302_CLOCK_BURST, temp, 8);
}

void DS1302_ReadRam_Brust(uint8_t len, uint8_t* buf) {
    uint8_t i;

    if (len > DS1302_RAMSIZE)
        len = DS1302_RAMSIZE;

    for (i = 0; i < len; i++)
        buf[i] = 0;

    DS1302_Read_Brust(DS1302_RAM_BURST, buf, len);
}

void DS1302_WriteRam_Brust(uint8_t len, uint8_t* buf) {
    if (len > DS1302_RAMSIZE)
        len = DS1302_RAMSIZE;
    DS1302_Write_Brust(DS1302_RAM_BURST, buf, len);
}
