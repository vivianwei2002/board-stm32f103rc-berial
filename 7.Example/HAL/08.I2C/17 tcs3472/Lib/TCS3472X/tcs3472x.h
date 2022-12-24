
#ifndef _TCS3472X_H_
#define _TCS3472X_H_

#include "i2c.h"

#define TCS34721_I2C_ADDRESS 0x39
#define TCS34723_I2C_ADDRESS 0x39
#define TCS34725_I2C_ADDRESS 0x29
#define TCS34727_I2C_ADDRESS 0x29

#define TCS3472X_I2C hi2c2                        // <----
#define TCS3472X_DEV (TCS34725_I2C_ADDRESS << 1)  // <----

#define USE_GAMMA_TABLE

#ifdef USE_GAMMA_TABLE  // 伽马表
#include <math.h>
extern uint8_t gammatable[256];
void           InitGammaTable(uint8_t commonAnode);
#endif

#define TCS3472_REG_ID_34721_34725 0x44
#define TCS3472_REG_ID_34723_34727 0x4D

#define TCS3472X_COMMAND_BIT 0x80

#define TCS3472X_ENABLE 0x00
#define TCS3472X_ENABLE_AIEN 0x10  // RGBC Interrupt Enable
#define TCS3472X_ENABLE_WEN 0x08   // Wait enable - Writing 1 activates the wait timer
#define TCS3472X_ENABLE_AEN 0x02   // RGBC Enable - Writing 1 actives the ADC, 0 disables it
#define TCS3472X_ENABLE_PON 0x01   // Power on - Writing 1 activates the internal oscillator, 0 disables it
#define TCS3472X_ATIME 0x01        // Integration time
#define TCS3472X_WTIME 0x03        // Wait time (if TCS3472X_ENABLE_WEN is asserted)
#define TCS3472X_WTIME_2_4MS 0xFF  // WLONG0 = 2.4ms   WLONG1 = 0.029s
#define TCS3472X_WTIME_204MS 0xAB  // WLONG0 = 204ms   WLONG1 = 2.45s
#define TCS3472X_WTIME_614MS 0x00  // WLONG0 = 614ms   WLONG1 = 7.4s
#define TCS3472X_AILTL 0x04        // Clear channel lower interrupt threshold
#define TCS3472X_AILTH 0x05
#define TCS3472X_AIHTL 0x06  // Clear channel upper interrupt threshold
#define TCS3472X_AIHTH 0x07
#define TCS3472X_PERS 0x0C           // Persistence register - basic SW filtering mechanism for interrupts
#define TCS3472X_PERS_NONE 0x00      // Every RGBC cycle generates an interrupt
#define TCS3472X_PERS_1_CYCLE 0x01   // 1 clean channel value outside threshold range generates an interrupt
#define TCS3472X_PERS_2_CYCLE 0x02   // 2 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_3_CYCLE 0x03   // 3 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_5_CYCLE 0x04   // 5 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_10_CYCLE 0x05  // 10 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_15_CYCLE 0x06  // 15 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_20_CYCLE 0x07  // 20 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_25_CYCLE 0x08  // 25 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_30_CYCLE 0x09  // 30 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_35_CYCLE 0x0A  // 35 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_40_CYCLE 0x0B  // 40 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_45_CYCLE 0x0C  // 45 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_50_CYCLE 0x0D  // 50 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_55_CYCLE 0x0E  // 55 clean channel values outside threshold range generates an interrupt
#define TCS3472X_PERS_60_CYCLE 0x0F  // 60 clean channel values outside threshold range generates an interrupt
#define TCS3472X_CONFIG 0x0D
#define TCS3472X_CONFIG_WLONG 0x02  // Choose between short and long (12x) wait times via TCS3472X_WTIME
#define TCS3472X_CONTROL 0x0F       // Set the gain level for the sensor
#define TCS3472X_ID 0x12            // 0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727
#define TCS3472X_STATUS 0x13
#define TCS3472X_STATUS_AINT 0x10    // RGBC Clean channel interrupt
#define TCS3472X_STATUS_AVALID 0x01  // Indicates that the RGBC channels have completed an integration cycle
#define TCS3472X_CDATAL 0x14         // Clear channel data
#define TCS3472X_CDATAH 0x15
#define TCS3472X_RDATAL 0x16  // Red channel data
#define TCS3472X_RDATAH 0x17
#define TCS3472X_GDATAL 0x18  // Green channel data
#define TCS3472X_GDATAH 0x19
#define TCS3472X_BDATAL 0x1A  // Blue channel data
#define TCS3472X_BDATAH 0x1B

/**
 * The RGBC timing register controls the internal integration time
 * of the RGBC clear and IR channel ADCs in 2.4-ms increments.
 * Max RGBC Count = (256 − ATIME) × 1024 up to a maximum of 65535.
 */

// Integration Time 积分时间
#define TCS3472X_INTEGRATIONTIME_2_4MS 0xFF  //  2.4ms - 1 cycle    - Max Count: 1024
#define TCS3472X_INTEGRATIONTIME_24MS 0xF6   //  24ms  - 10 cycles  - Max Count: 10240
#define TCS3472X_INTEGRATIONTIME_50MS 0xEB   //  50ms  - 20 cycles  - Max Count: 20480
#define TCS3472X_INTEGRATIONTIME_101MS 0xD5  //  101ms - 42 cycles  - Max Count: 43008
#define TCS3472X_INTEGRATIONTIME_154MS 0xC0  //  154ms - 64 cycles  - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_240MS 0x9C  //  240ms - 100 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_700MS 0x00  //  700ms - 256 cycles - Max Count: 65535

// Gain 增益
#define TCS3472X_GAIN_1X 0x00   //  No gain
#define TCS3472X_GAIN_4X 0x01   //  4x gain
#define TCS3472X_GAIN_16X 0x02  //  16x gain
#define TCS3472X_GAIN_60X 0x03  //  60x gain

/*!
 *  @brief Initializes I2C and configures the sensor (call this function beforedoing anything else).
 *  @return  0  success.
 */
uint8_t TCS3472X_Init(void);
/*!
 *  @brief Sets the integration time for the TC3472X.
 *  @param it  integration time.
 */
void TCS3472X_SetIntegrationTime(uint8_t it);
/*!
 *  @brief Adjusts the gain on the TCS3472X (adjusts the sensitivity to light)
 *  @param gain  gain time.
 */
void TCS3472X_SetGain(uint8_t gain);
/*!
 *  @brief Reads the raw red, green, blue and clear channel values
 *  @param r  red.
 *  @param g  green.
 *  @param b  blue.
 */
uint8_t TCS3472X_GetRGBC(uint16_t* r, uint16_t* g, uint16_t* b, uint16_t* c);
/*!
 *  @brief Converts the raw R/G/B values to color temperature in degrees
 *  @param r  red.
 *  @param g  green.
 *  @param b  blue.
 *  @return  c.
 */
uint16_t TCS3472X_CalculateColorTemperature(uint16_t r, uint16_t g, uint16_t b);
/*!
 *  @brief Converts the raw R/G/B values to lux
 *  @param r  red.
 *  @param g  green.
 *  @param b  blue.
 *  @return  lux.
 */
uint16_t TCS3472X_CalculateLux(uint16_t r, uint16_t g, uint16_t b);
/*!
 *  @brief Writes a register and an 8 bit value over I2C
 *  @param reg register address .
 *  @param value  data.
 */
void TCS3472X_WriteReg(uint8_t reg, uint8_t value);
/*!
 *  @brief Reads an 8 bit value over I2C
 *  @param reg register address .
 *  @return I2C  data.
 */
uint8_t TCS3472X_ReadReg(uint8_t reg);
/*!
 *  @brief Reads an 8 bit value over I2C
 *  @param reg register address .
 *  @return I2C  data.
 */
uint16_t TCS3472X_ReadRegWord(uint8_t reg);
/*!
 *  @brief Interrupts enabled
 */
void TCS3472X_Lock(void);
/*!
 *  @brief Interrupts disabled
 */
void TCS3472X_Unlock(void);
/*!
 *  @brief clear Interrupts
 */
void TCS3472X_Clear(void);
/*!
 *  @brief set Int Limits
 *  @param l low .
 *  @param h high .
 */
void TCS3472X_SetIntLimits(uint16_t low, uint16_t high);
/*!
 *  @brief Enables the device
 */
void TCS3472X_Enable(void);
/*!
 *  Disables the device (putting it in lower power sleep mode)
 */
void TCS3472X_Disable(void);

#endif
