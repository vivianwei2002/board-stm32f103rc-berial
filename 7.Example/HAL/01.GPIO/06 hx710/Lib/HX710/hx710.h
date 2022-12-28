
#ifndef __HX710_H__
#define __HX710_H__

#include "gpio.h"
#include "utils.h"

#define CONFIG_HX710 1
#define CONFIG_HX711 0

typedef enum {
    HX71X_GAIN_128 = 1,  // channel A
    HX71X_GAIN_64  = 3,  // channel A
#if CONFIG_HX711
    HX71X_GAIN_32 = 2,  // channel B
#endif
} HX71x_gain_t;

static int32_t TareWeight = 0;  // 皮重(unused)

int32_t HX710_Read(HX71x_gain_t gain);

void ConvertToPressure(int32_t value);

#endif
