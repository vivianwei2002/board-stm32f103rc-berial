
#include <math.h>
#include <stdlib.h>

#include "tcs3472x.h"

#ifdef USE_GAMMA_TABLE

uint8_t gammatable[256];

void InitGammaTable(uint8_t commonAnode /*1*/) {
    for (int i = 0; i < 256; ++i) {
        float x       = pow(i / 255.f, 2.5) * 255;
        gammatable[i] = commonAnode ? (255 - x) : x;
    }
}

#endif

float powf(const float x, const float y) {
    return (float)(pow((double)x, (double)y));
}

void TCS3472X_WriteReg(uint8_t reg, uint8_t value) {
    HAL_I2C_Mem_Write(&TCS3472X_I2C, TCS3472X_DEV, reg | TCS3472X_COMMAND_BIT, I2C_MEMADD_SIZE_8BIT, &value, 1, 0XFF);
}

uint8_t TCS3472X_ReadReg(uint8_t reg) {
    uint8_t buffer;
    HAL_I2C_Mem_Read(&TCS3472X_I2C, TCS3472X_DEV, reg | TCS3472X_COMMAND_BIT, I2C_MEMADD_SIZE_8BIT, &buffer, 1, 0XFF);
    return buffer;
}

uint16_t TCS3472X_ReadRegWord(uint8_t reg) {
    uint8_t buffer[2];
    HAL_I2C_Mem_Read(&TCS3472X_I2C, TCS3472X_DEV, reg | TCS3472X_COMMAND_BIT, I2C_MEMADD_SIZE_8BIT, buffer, 2, 0XFF);
    return (buffer[1] << 8) | buffer[0];
}

void TCS3472X_Enable(void) {
    TCS3472X_WriteReg(TCS3472X_ENABLE, TCS3472X_ENABLE_PON);
    TCS3472X_WriteReg(TCS3472X_ENABLE, TCS3472X_ENABLE_PON | TCS3472X_ENABLE_AEN);
    HAL_Delay(3);
}

void TCS3472X_Disable(void) {
    /* Turn the device off to save power */
    TCS3472X_WriteReg(TCS3472X_ENABLE, TCS3472X_ReadReg(TCS3472X_ENABLE) & ~(TCS3472X_ENABLE_PON | TCS3472X_ENABLE_AEN));
}

uint8_t TCS3472X_Init(void) {
    uint8_t id = TCS3472X_ReadReg(TCS3472X_ID);  // get device id
    printf("tcs3472x id: %x\r\n", id);
    if ((id == TCS3472_REG_ID_34721_34725) | (id == TCS3472_REG_ID_34723_34727)) {
        /* Set default integration time and gain */
        TCS3472X_SetIntegrationTime(TCS3472X_INTEGRATIONTIME_50MS);
        TCS3472X_SetGain(TCS3472X_GAIN_1X);
        /* Note: by default, the device is in power down mode on bootup */
        TCS3472X_Enable();
        return 1;
    }
    return 0;
}

void TCS3472X_SetIntegrationTime(uint8_t IntegrationTime) {
    /* Update the timing register */
    TCS3472X_WriteReg(TCS3472X_ATIME, IntegrationTime);
}

void TCS3472X_SetGain(uint8_t gain) {
    /* Update the timing register */
    TCS3472X_WriteReg(TCS3472X_CONTROL, gain);
}

uint8_t TCS3472X_GetRGBC(uint16_t* r, uint16_t* g, uint16_t* b, uint16_t* c /*清晰光感应值*/) {
    if (TCS3472X_ReadReg(TCS3472X_STATUS) & TCS3472X_STATUS_AVALID) {
        *c = TCS3472X_ReadRegWord(TCS3472X_CDATAL);
        *r = TCS3472X_ReadRegWord(TCS3472X_RDATAL);
        *g = TCS3472X_ReadRegWord(TCS3472X_GDATAL);
        *b = TCS3472X_ReadRegWord(TCS3472X_BDATAL);
        return 1;
    } else {
        *c = *r = *g = *b = 0;
        return 0;
    }

    /* Set a delay for the integration time */
    // switch (TCS3472X_IntegrationTime) {
    //     case TCS3472X_INTEGRATIONTIME_2_4MS: HAL_Delay(3); break;
    //     case TCS3472X_INTEGRATIONTIME_24MS: HAL_Delay(24); break;
    //     case TCS3472X_INTEGRATIONTIME_50MS: HAL_Delay(50); break;
    //     case TCS3472X_INTEGRATIONTIME_101MS: HAL_Delay(101); break;
    //     case TCS3472X_INTEGRATIONTIME_154MS: HAL_Delay(154); break;
    //     case TCS3472X_INTEGRATIONTIME_700MS: HAL_Delay(700); break;
    // }
}

uint16_t TCS3472X_CalculateColorTemperature(uint16_t r, uint16_t g, uint16_t b) {
    float X, Y, Z; /* RGB to XYZ correlation      */
    float xc, yc;  /* Chromaticity co-ordinates   */
    float n;       /* McCamy's formula            */
    float cct;

    /* 1. Map RGB values to their XYZ counterparts.    */
    /* Based on 6500K fluorescent, 3000K fluorescent   */
    /* and 60W incandescent values for a wide range.   */
    /* Note: Y = Illuminance or lux                    */
    X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
    Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
    Z = (-0.68202F * r) + (0.77073F * g) + (0.56332F * b);

    /* 2. Calculate the chromaticity co-ordinates      */
    xc = (X) / (X + Y + Z);
    yc = (Y) / (X + Y + Z);

    /* 3. Use McCamy's formula to determine the CCT    */
    n = (xc - 0.3320F) / (0.1858F - yc);

    /* Calculate the final CCT */
    cct = (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

    /* Return the results in degrees Kelvin */
    return (uint16_t)cct;
}

uint16_t TCS3472X_CalculateLux(uint16_t r, uint16_t g, uint16_t b) {
    float illuminance;

    /* This only uses RGB ... how can we integrate clear or calculate lux */
    /* based exclusively on clear since this might be more reliable?      */
    illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

    return (uint16_t)illuminance;
}

void TCS3472X_Lock() {  // 使能中断
    TCS3472X_WriteReg(TCS3472X_ENABLE, TCS3472X_ReadReg(TCS3472X_ENABLE) | TCS3472X_ENABLE_AIEN);
}

void TCS3472X_Unlock() {  // 禁用中断
    TCS3472X_WriteReg(TCS3472X_ENABLE, TCS3472X_ReadReg(TCS3472X_ENABLE) & ~TCS3472X_ENABLE_AIEN);
}

void TCS3472X_Clear(void) {  // 清除中断标志(每次TCS3472X产生中断后,需调用该函数进行中断标志位清除)
    uint8_t v = TCS3472X_COMMAND_BIT | 0x66;
    HAL_I2C_Master_Transmit(&TCS3472X_I2C, TCS3472X_DEV, &v, 1, 0xFF);
}

void TCS3472X_SetIntLimits(uint16_t low, uint16_t high) {
    TCS3472X_WriteReg(0x04, low & 0xFF);
    TCS3472X_WriteReg(0x05, low >> 8);
    TCS3472X_WriteReg(0x06, high & 0xFF);
    TCS3472X_WriteReg(0x07, high >> 8);
}

// RGB 转 HSL

typedef struct {
    uint16_t c;  //[0-65536]
    uint16_t r;
    uint16_t g;
    uint16_t b;
} COLOR_RGBC;  // RGBC

typedef struct {
    uint16_t h;  //[0,360]
    uint8_t  s;  //[0,100]
    uint8_t  l;  //[0,100]
} COLOR_HSL;     // HSL

#define max3v(v1, v2, v3) ((v1) < (v2) ? ((v2) < (v3) ? (v3) : (v2)) : ((v1) < (v3) ? (v3) : (v1)))
#define min3v(v1, v2, v3) ((v1) > (v2) ? ((v2) > (v3) ? (v3) : (v2)) : ((v1) > (v3) ? (v3) : (v1)))

COLOR_RGBC rgb;
COLOR_HSL  hsl;

void RGBtoHSL(COLOR_RGBC* Rgb, COLOR_HSL* Hsl) {
    uint8_t maxVal, minVal, difVal;
    uint8_t r = Rgb->r * 100 / Rgb->c;  //[0-100]
    uint8_t g = Rgb->g * 100 / Rgb->c;
    uint8_t b = Rgb->b * 100 / Rgb->c;

    maxVal = max3v(r, g, b);
    minVal = min3v(r, g, b);
    difVal = maxVal - minVal;

    //计算亮度
    Hsl->l = (maxVal + minVal) / 2;  //[0-100]

    if (maxVal == minVal)  //若r=g=b,灰度
    {
        Hsl->h = 0;
        Hsl->s = 0;
    } else {
        //计算色调
        if (maxVal == r) {
            if (g >= b)
                Hsl->h = 60 * (g - b) / difVal;
            else
                Hsl->h = 60 * (g - b) / difVal + 360;
        } else {
            if (maxVal == g)
                Hsl->h = 60 * (b - r) / difVal + 120;
            else if (maxVal == b)
                Hsl->h = 60 * (r - g) / difVal + 240;
        }

        //计算饱和度
        if (Hsl->l <= 50)
            Hsl->s = difVal * 100 / (maxVal + minVal);  //[0-100]
        else
            Hsl->s = difVal * 100 / (200 - (maxVal + minVal));
    }
}