#include "stm32f1xx_hal.h"

void HAL_Delay_us(uint32_t us);

#define delay_ms(ms) HAL_Delay(ms)
#define delay_us(us) HAL_Delay_us(us)