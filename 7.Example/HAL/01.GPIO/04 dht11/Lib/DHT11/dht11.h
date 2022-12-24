#ifndef __DHT11_H__
#define __DHT11_H__

#include <stdbool.h>
#include <stdio.h>
#include "gpio.h"

uint8_t DHT11_ReadData(float* temp, float* humi);

float ConvertCtoF(float);
float ConvertFtoC(float);
float ComputeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit);

#endif