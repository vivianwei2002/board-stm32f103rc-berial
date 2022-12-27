#include "xmc5883l.h"

#define PI         3.1415926
#define RAD_TO_DEG 57.2958

float mgPerDigit;
float Gauss_LSB_XY = 1090.0;

bool XMC5883L_Init()
{
#if defined CONFIG_HMC5883L
    if ((XMC5883L_ReadByte(HMC5883L_REG_IDENT_A) != 0x48) ||
        (XMC5883L_ReadByte(HMC5883L_REG_IDENT_B) != 0x34) ||
        (XMC5883L_ReadByte(HMC5883L_REG_IDENT_C) != 0x33))
        return false;
    printf("ok\r\n");
    XMC5883L_SetRange(HMC5883L_RANGE_1_3GA);
    XMC5883L_SetMeasurementMode(HMC5883L_CONTINOUS);
    XMC5883L_SetDataRate(HMC5883L_DATARATE_15HZ);
    XMC5883L_SetSamples(HMC5883L_SAMPLES_1);
    mgPerDigit = 0.92f;
#elif defined CONFIG_QMC5883L
    XMC5883L_WriteByte(QMC5883L_REG_IDENT_B, 0X01);
    XMC5883L_WriteByte(QMC5883L_REG_IDENT_C, 0X40);
    XMC5883L_WriteByte(QMC5883L_REG_IDENT_D, 0X01);
    XMC5883L_WriteByte(QMC5883L_REG_CONFIG_1, 0X1D);
    if ((XMC5883L_ReadByte(QMC5883L_REG_IDENT_B) != 0x01) ||
        (XMC5883L_ReadByte(QMC5883L_REG_IDENT_C) != 0x40) ||
        (XMC5883L_ReadByte(QMC5883L_REG_IDENT_D) != 0x01))
        return false;
    XMC5883L_SetRange(QMC5883L_RANGE_8GA);
    XMC5883L_SetMeasurementMode(QMC5883L_CONTINOUS);
    XMC5883L_SetDataRate(QMC5883L_DATARATE_50HZ);
    XMC5883L_SetSamples(QMC5883L_SAMPLES_1);
    mgPerDigit = 4.35f;
#elif defined CONFIG_VMC5883L
    XMC5883L_WriteByte(VCM5883L_CTR_REG1, 0X00);
    XMC5883L_WriteByte(VCM5883L_CTR_REG2, 0X4D);
#endif
    return true;
}

vector3_int16* XMC5883L_ReadRaw(vector3_int16* v)
{
#if defined CONFIG_HMC5883L
    v->x = XMC5883L_ReadTwoByte(HMC5883L_REG_OUT_X_M);
    v->y = XMC5883L_ReadTwoByte(HMC5883L_REG_OUT_Y_M);
    v->z = XMC5883L_ReadTwoByte(HMC5883L_REG_OUT_Z_M);
#elif defined CONFIG_QMC5883L
    v->x       = XMC5883L_ReadTwoByte(QMC5883L_REG_OUT_X_L);
    v->y       = XMC5883L_ReadTwoByte(QMC5883L_REG_OUT_Y_L);
    v->z       = XMC5883L_ReadTwoByte(QMC5883L_REG_OUT_Z_L);
#elif defined CONFIG_VMC5883L
    v->x = -XMC5883L_ReadTwoByte(VCM5883L_REG_OUT_X_L);
    v->y = -XMC5883L_ReadTwoByte(VCM5883L_REG_OUT_Y_L);
    v->z = -XMC5883L_ReadTwoByte(VCM5883L_REG_OUT_Z_L);
#endif
    return v;
}

vector3_float* XMC5883L_CalcAngle(vector3_int16* data, vector3_float* angle)
{
    angle->z = (atan2(data->y, data->x) * RAD_TO_DEG + 180);  // xy
    angle->y = (atan2(data->z, data->x) * RAD_TO_DEG + 180);  // xz
    angle->x = (atan2(data->z, data->y) * RAD_TO_DEG + 180);  // yz
    return angle;
}

vector3_int16* XMC5883L_Calibrate(vector3_int16* data, vector3_int16* min, vector3_int16* max)
{
    if (data->x < min->x)
        data->x = min->x;
    else if (data->x > max->x)
        data->x = max->x;
    if (data->y < min->y)
        data->y = min->y;
    else if (data->y > max->y)
        data->y = max->y;
    if (data->z < min->z)
        data->z = min->z;
    else if (data->z > max->z)
        data->z = max->z;
    return data;
}

void XMC5883L_SetRange(XMC5883L_range_t range)
{
#if defined CONFIG_HMC5883L
    switch (range) {
        case HMC5883L_RANGE_0_88GA: Gauss_LSB_XY = 1370.0; break;
        case HMC5883L_RANGE_1_3GA: Gauss_LSB_XY = 1090.0; break;
        case HMC5883L_RANGE_1_9GA: Gauss_LSB_XY = 820.0; break;
        case HMC5883L_RANGE_2_5GA: Gauss_LSB_XY = 660.0; break;
        case HMC5883L_RANGE_4GA: Gauss_LSB_XY = 440.0; break;
        case HMC5883L_RANGE_4_7GA: Gauss_LSB_XY = 390.0; break;
        case HMC5883L_RANGE_5_6GA: Gauss_LSB_XY = 330.0; break;
        case HMC5883L_RANGE_8_1GA: Gauss_LSB_XY = 230.0; break;
        default: break;
    }
    XMC5883L_WriteByte(HMC5883L_REG_CONFIG_B, range << 5);
#elif defined CONFIG_QMC5883L
    switch (range) {
        case QMC5883L_RANGE_2GA: mgPerDigit = 1.22f; break;
        case QMC5883L_RANGE_8GA: mgPerDigit = 4.35f; break;
        default: break;
    }
    XMC5883L_WriteByte(QMC5883L_REG_CONFIG_2, range << 4);
#elif defined CONFIG_VMC5883L
    // default 8G
#endif
}

XMC5883L_range_t XMC5883L_GetRange(void)
{
#if defined CONFIG_HMC5883L  // bit5
    return XMC5883L_ReadBit(HMC5883L_REG_CONFIG_B, 5);
#elif defined CONFIG_QMC5883L  // bit4
    return XMC5883L_ReadBit(QMC5883L_REG_CONFIG_2, 4);
#elif defined CONFIG_VMC5883L
    return QMC5883L_RANGE_8GA;
#endif
}

void XMC5883L_SetMeasurementMode(XMC5883L_mode_t mode)
{
#if defined CONFIG_HMC5883L  // 0b00000011
    XMC5883L_WriteBits(HMC5883L_REG_MODE, 0, 2, mode);
#elif defined CONFIG_QMC5883L  // 0b00000011
    XMC5883L_WriteBits(QMC5883L_REG_CONFIG_1, 0, 2, mode);
#elif defined CONFIG_VMC5883L  // 0b00000001
    XMC5883L_WriteBit(QMC5883L_REG_CONFIG_1, 0, mode);
#endif
}

XMC5883L_mode_t XMC5883L_GetMeasurementMode(void)
{
#if defined CONFIG_HMC5883L
    return XMC5883L_ReadBits(HMC5883L_REG_MODE, 0, 2);
#elif defined CONFIG_QMC5883L
    return XMC5883L_ReadBits(QMC5883L_REG_CONFIG_1, 0, 2);
#elif defined CONFIG_VMC5883L
    return XMC5883L_ReadBit(VCM5883L_CTR_REG2, 0);
#endif
}

void XMC5883L_SetDataRate(XMC5883L_dataRate_t dataRate)
{
#if defined CONFIG_HMC5883L  // 0b00011100
    XMC5883L_WriteBits(HMC5883L_REG_CONFIG_A, 2, 3, dataRate);
#elif defined CONFIG_QMC5883L  // 0b00001100
    XMC5883L_WriteBits(QMC5883L_REG_CONFIG_1, 2, 2, dataRate);
#elif defined CONFIG_VMC5883L  // 0b00001100
    XMC5883L_WriteBits(VCM5883L_CTR_REG2, 2, 2, dataRate);
#endif
}

XMC5883L_dataRate_t XMC5883L_GetDataRate(void)
{
#if defined CONFIG_HMC5883L
    return XMC5883L_ReadBits(HMC5883L_REG_CONFIG_A, 2, 3);
#elif defined CONFIG_QMC5883L
    return XMC5883L_ReadBits(QMC5883L_REG_CONFIG_1, 2, 2);
#elif defined CONFIG_VMC5883L
    return XMC5883L_ReadBits(VCM5883L_CTR_REG2, 2, 2);
#endif
}

void XMC5883L_SetSamples(XMC5883L_samples_t samples)
{
#if defined CONFIG_HMC5883L  // 0b01100000
    XMC5883L_WriteBits(HMC5883L_REG_CONFIG_A, 5, 2, samples);
#elif defined CONFIG_QMC5883L || defined CONFIG_VMC5883L  // 0b11000000
    XMC5883L_WriteBits(QMC5883L_REG_CONFIG_1, 6, 2, samples);
#endif
}

XMC5883L_samples_t XMC5883L_GetSamples(void)
{
#if defined CONFIG_HMC5883L
    return XMC5883L_ReadBits(HMC5883L_REG_CONFIG_A, 5, 2);
#elif defined CONFIG_QMC5883L || defined CONFIG_VMC5883L
    return XMC5883L_ReadBits(QMC5883L_REG_CONFIG_1, 6, 2);
#endif
}

float_t XMC5883L_GetHeadingDegrees(vector3_int16* raw, float_t declination)
{
    // double XuT,YuT,ZuT;
    // ReadRaw();
    // XuT = raw->x / Gauss_LSB_XY * 100;
    // YuT = raw->y / Gauss_LSB_XY * 100;
    // ZuT = raw->z / Gauss_LSB_XY * 100;
    float_t heading = atan2(raw->y, raw->x);
    heading += declination;
    if (heading < 0)
        heading += 2 * PI;
    else if (heading > 2 * PI)
        heading -= 2 * PI;
    return heading * 180 / PI;
}
