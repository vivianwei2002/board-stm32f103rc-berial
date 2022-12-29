#ifndef __UTILS_H__
#define __UTILS_H__

#define CONFIG_DELAY 1
#define CONFIG_DEBUG 1

// <! delay

#if CONFIG_DELAY

#include "stm32f1xx_hal.h"

#define delay_ms(ms) HAL_Delay(ms)
#define delay_us(us) HAL_Delay_us(us)

static void HAL_Delay_us(uint32_t us)
{
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000000);
    HAL_Delay(us - 1);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
}

#endif

// <! log

#if CONFIG_DEBUG

#include <stdio.h>

// print something (end with newline character)
#define println(format, ...) printf(format "\r\n", ##__VA_ARGS__)

// print variable (end with newline character)
#if 1
#define printv(format, var) printf("[ line: %d | function: %s ] %s = " format "\r\n", __LINE__, __FUNCTION__, #var, var)
#else
#define printv(format, var) printf("[ line: %d | file: %s | function: %s ] %s = " format "\r\n", __LINE__, __FILE__, __FUNCTION__, #var, var)
#endif

#endif

#endif