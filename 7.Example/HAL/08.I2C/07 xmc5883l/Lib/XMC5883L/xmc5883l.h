#ifndef __XMC5883L_H_
#define __XMC5883L_H_

#include <math.h>
#include <stdbool.h>

#include "i2cdev.h"
#include "i2c.h"

#define XMC5883L_I2C hi2c2

// 注：VMC5883L 未进行实际测试

#define CONFIG_HMC5883L 1
// #define CONFIG_QMC5883L 1
// #define CONFIG_VMC5883L 1

// device i2c address
#define HMC5883L_ADDRESS 0x1E
#define QMC5883L_ADDRESS 0x0D
#define VCM5883L_ADDRESS 0x0C

#if defined CONFIG_HMC5883L
#define XMC5883L_DEV (HMC5883L_ADDRESS << 1)
#elif defined CONFIG_QMC5883L
#define XMC5883L_DEV (QMC5883L_ADDRESS << 1)
#elif defined CONFIG_VMC5883L
#define XMC5883L_DEV (VCM5883L_ADDRESS << 1)
#endif

// registers

#if defined CONFIG_HMC5883L
#define HMC5883L_REG_CONFIG_A 0x00
#define HMC5883L_REG_CONFIG_B 0x01
#define HMC5883L_REG_MODE     0x02
#define HMC5883L_REG_OUT_X_M  0x03
#define HMC5883L_REG_OUT_X_L  0x04
#define HMC5883L_REG_OUT_Z_M  0x05
#define HMC5883L_REG_OUT_Z_L  0x06
#define HMC5883L_REG_OUT_Y_M  0x07
#define HMC5883L_REG_OUT_Y_L  0x08
#define HMC5883L_REG_STATUS   0x09
#define HMC5883L_REG_IDENT_A  0x0A
#define HMC5883L_REG_IDENT_B  0x0B
#define HMC5883L_REG_IDENT_C  0x0C
#elif defined CONFIG_QMC5883L
#define QMC5883L_REG_OUT_X_M  0x01
#define QMC5883L_REG_OUT_X_L  0x00
#define QMC5883L_REG_OUT_Z_M  0x05
#define QMC5883L_REG_OUT_Z_L  0x04
#define QMC5883L_REG_OUT_Y_M  0x03
#define QMC5883L_REG_OUT_Y_L  0x02
#define QMC5883L_REG_STATUS   0x06
#define QMC5883L_REG_CONFIG_1 0x09
#define QMC5883L_REG_CONFIG_2 0x0A
#define QMC5883L_REG_IDENT_B  0x0B
#define QMC5883L_REG_IDENT_C  0x20
#define QMC5883L_REG_IDENT_D  0x21
#elif defined CONFIG_VMC5883L
#define VCM5883L_REG_OUT_X_L 0x00
#define VCM5883L_REG_OUT_X_H 0x01
#define VCM5883L_REG_OUT_Y_L 0x02
#define VCM5883L_REG_OUT_Y_H 0x03
#define VCM5883L_REG_OUT_Z_L 0x04
#define VCM5883L_REG_OUT_Z_H 0x05
#define VCM5883L_CTR_REG1    0x0B
#define VCM5883L_CTR_REG2    0x0A
#endif

// configs

typedef enum {
#if defined CONFIG_HMC5883L
    HMC5883L_SAMPLES_8 = 0b11,
    HMC5883L_SAMPLES_4 = 0b10,
    HMC5883L_SAMPLES_2 = 0b01,
    HMC5883L_SAMPLES_1 = 0b00,
#elif defined CONFIG_QMC5883L || defined CONFIG_VMC5883
    QMC5883L_SAMPLES_8      = 0b11,
    QMC5883L_SAMPLES_4      = 0b10,
    QMC5883L_SAMPLES_2      = 0b01,
    QMC5883L_SAMPLES_1      = 0b00
#endif
} XMC5883L_samples_t;

typedef enum {
#if defined CONFIG_HMC5883L
    HMC5883L_DATARATE_75HZ    = 0b110,
    HMC5883L_DATARATE_30HZ    = 0b101,
    HMC5883L_DATARATE_15HZ    = 0b100,
    HMC5883L_DATARATE_7_5HZ   = 0b011,
    HMC5883L_DATARATE_3HZ     = 0b010,
    HMC5883L_DATARATE_1_5HZ   = 0b001,
    HMC5883L_DATARATE_0_75_HZ = 0b000,
#elif defined CONFIG_QMC5883L
    QMC5883L_DATARATE_10HZ  = 0b00,
    QMC5883L_DATARATE_50HZ  = 0b01,
    QMC5883L_DATARATE_100HZ = 0b10,
    QMC5883L_DATARATE_200HZ = 0b11,
#endif
} XMC5883L_dataRate_t;

typedef enum {
#if defined CONFIG_HMC5883L
    HMC5883L_RANGE_8_1GA  = 0b111,
    HMC5883L_RANGE_5_6GA  = 0b110,
    HMC5883L_RANGE_4_7GA  = 0b101,
    HMC5883L_RANGE_4GA    = 0b100,
    HMC5883L_RANGE_2_5GA  = 0b011,
    HMC5883L_RANGE_1_9GA  = 0b010,
    HMC5883L_RANGE_1_3GA  = 0b001,
    HMC5883L_RANGE_0_88GA = 0b000,
#elif defined CONFIG_QMC5883L
    QMC5883L_RANGE_2GA      = 0b00,
    QMC5883L_RANGE_8GA      = 0b01,
#elif defined CONFIG_VMC5883L
    VCM5883L_RANGE_8GA = 0b01,
#endif
} XMC5883L_range_t;

typedef enum {
#if defined CONFIG_HMC5883L
    HMC5883L_IDLE      = 0b10,
    HMC5883_SINGLE     = 0b01,
    HMC5883L_CONTINOUS = 0b00,
#elif defined CONFIG_QMC5883L
    QMC5883L_SINGLE         = 0b00,
    QMC5883L_CONTINOUS      = 0b01,
#elif defined CONFIG_VMC5883L
    VCM5883L_SINGLE    = 0b00,
    VCM5883L_CONTINOUS = 0b01,
#endif
} XMC5883L_mode_t;

#define Vector3(_type_, _name_) \
    typedef struct {            \
        _type_ x, y, z;         \
    } _name_

Vector3(int16_t, vector3_int16);
Vector3(float_t, vector3_float);

// read / write function

#define XMC5883L_ReadByte(reg) i2c_read_1byte_fast(&XMC5883L_I2C, XMC5883L_DEV, reg)

#if defined CONFIG_HMC5883L
#define XMC5883L_ReadTwoByte(reg) i2c_read_2byte_lsb_msb_fast(&XMC5883L_I2C, XMC5883L_DEV, reg)
#elif defined CONFIG_QMC5883L
#define XMC5883L_ReadTwoByte(reg) i2c_read_2byte_msb_lsb_fast(&XMC5883L_I2C, XMC5883L_DEV, reg)
#elif defined CONFIG_VMC5883L
#define XMC5883L_ReadTwoByte(reg) i2c_read_2byte_msb_lsb_fast(&XMC5883L_I2C, XMC5883L_DEV, reg)
#endif

#define XMC5883L_WriteByte(reg, data)             i2c_write_1byte(&XMC5883L_I2C, XMC5883L_DEV, reg, data)
#define XMC5883L_ReadBit(reg, bit)                i2c_read_bit_fast(&XMC5883L_I2C, XMC5883L_DEV, reg, bit)
#define XMC5883L_WriteBit(reg, bit, data)         i2c_write_bit(&XMC5883L_I2C, XMC5883L_DEV, reg, bit, data)
#define XMC5883L_ReadBits(reg, start, len)        i2c_read_bits_fast(&XMC5883L_I2C, XMC5883L_DEV, reg, start, len)
#define XMC5883L_WriteBits(reg, start, len, data) i2c_write_bits(&XMC5883L_I2C, XMC5883L_DEV, reg, start, len, data)

// main functions

bool XMC5883L_Init(void);

vector3_int16* XMC5883L_ReadRaw(vector3_int16* raw);                                            // return raw
vector3_int16* XMC5883L_Calibrate(vector3_int16* raw, vector3_int16* min, vector3_int16* max);  // return raw
vector3_float* XMC5883L_CalcAngle(vector3_int16* raw, vector3_float* angle);                    // return angle

void                XMC5883L_SetRange(XMC5883L_range_t range);
XMC5883L_range_t    XMC5883L_GetRange(void);
void                XMC5883L_SetMeasurementMode(XMC5883L_mode_t mode);
XMC5883L_mode_t     XMC5883L_GetMeasurementMode(void);
void                XMC5883L_SetDataRate(XMC5883L_dataRate_t rate);
XMC5883L_dataRate_t XMC5883L_GetDataRate(void);
void                XMC5883L_SetSamples(XMC5883L_samples_t samples);
XMC5883L_samples_t  XMC5883L_GetSamples(void);

float_t XMC5883L_GetHeadingDegrees(vector3_int16* raw, float_t declination /* 磁偏角 */);

#endif