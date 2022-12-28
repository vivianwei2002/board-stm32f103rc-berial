
#include "hx710.h"

#define Delay(us) HAL_Delay_us(us)

#define DT() HAL_GPIO_ReadPin(HX710_DT_GPIO_Port, HX710_DT_Pin)

#define SCK_L() HAL_GPIO_WritePin(HX710_SCK_GPIO_Port, HX710_SCK_Pin, GPIO_PIN_RESET), Delay(20)
#define SCK_H() HAL_GPIO_WritePin(HX710_SCK_GPIO_Port, HX710_SCK_Pin, GPIO_PIN_SET), Delay(20)

////////////////////////////////////////////

int32_t HX710_Read(HX71x_gain_t gain)
{
    uint8_t  i    = 0;
    uint32_t data = 0;

    SCK_L();

    // Wait for the chip to become ready. (DT=0)

    while (DT()) {}

    // Pulse the clock pin 24 times to read the data. (read 3 bytes, msb first)

    for (i = 0; i < 24; ++i) {
        SCK_H();
        SCK_L();
        data <<= 1;
        data |= DT();
    }

    data ^= 0x800000;  // 最高位取反

    // Set the channel and the gain factor for the next reading using the clock pin.

    for (i = 0; i < gain; ++i) {
        SCK_H();
        SCK_L();
    }

    // printv("%d", data);

    return data;
}

void ConvertToPressure(int32_t value)
{
    // 海拔越高，气压越低

    float pascal = value * 2.98023e-7 * 200 + 500;
    float atm    = pascal * 9.86923E-6;
    float mmHg   = pascal * 0.00750062;
    float psi    = pascal * 0.000145038;

    printf("%.4f\r\n", pascal);

    return;
    printv("%.4f", pascal);
    printv("%.4f", atm);
    printv("%.4f", mmHg);
    printv("%.4f", psi);
    println("---------------------------");
}