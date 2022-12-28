#ifndef __AD9833_H__
#define __AD9833_H__

// https://blog.csdn.net/weixin_56608779/article/details/125654260

#include "spi.h"

#define AD9833_SPI hspi3  // <---

#define AD9833_CS_LOW()  HAL_GPIO_WritePin(AD9833_CS_GPIO_Port, AD9833_CS_Pin, GPIO_PIN_RESET)  // <--- FSYNC Pin
#define AD9833_CS_HIGH() HAL_GPIO_WritePin(AD9833_CS_GPIO_Port, AD9833_CS_Pin, GPIO_PIN_SET)    // <---

#define FMCLK 25000000UL  // <--- Master Clock On AD9833, 25MHz

////////////////////////////////

// 输出频率：0~12.5MHz（12.5MHz在时钟25MHz时达到）；
// 工作电压：2.3V~5.5V（最大不超过6V）；
// 输出电压：非方波:38mV~650mv,方波:0~VCC
// 通信方式：三线SPI（最大通信速率40MHz）；
// 输出波形：正弦、三角、方波；也可软件控制输出复杂波形；
// 睡眠模式（唤醒时间1ms）、脉冲直接输出、DAC关断等。

////////////////////////////////

/***********************************************************************
                            Control Register:
------------------------------------------------------------------------
D15,D14(MSB)	10 = FREQ1 write, 01 = FREQ0 write,
                11 = PHASE write, 00 = control write
D13	If D15,D14 = 00, 0 = individual LSB and MSB FREQ write, 频率寄存器的写入方式（单独写入/连续写入）
                     1 = both LSB and MSB FREQ writes consecutively
    If D15,D14 = 11, 0 = PHASE0 write, 1 = PHASE1 write
D12	0 = writing LSB independently 频率寄存器高低位选择
    1 = writing MSB independently
D11	0 = output FREQ0 [Frequency Select] 输出频率选择
    1 = output FREQ1
D10	0 = output PHASE0 [Phase Select] 输出相位选择
    1 = output PHASE1
D9	Reserved. Must be 0. [Reserved]
D8	0 = RESET disabled [Reset]
    1 = RESET enabled
D7	0 = internal clock is enabled MCLK控制
    1 = internal clock is disabled
D6	0 = onboard DAC is active for sine and triangle wave output, DAC输出控制
    1 = put DAC to sleep for square wave output
D5	0 = output depends on D1 输出连接方式选择
    1 = output is a square wave
D4	Reserved. Must be 0.
D3	0 = square wave of half frequency output  方波输出大小选择
    1 = square wave output
D2	Reserved. Must be 0. [Reserved]
D1	If D5 = 1, D1 = 0.
    Otherwise 0 = sine output, 1 = triangle output 输出模式选择
D0	Reserved. Must be 0. [Reserved]
***********************************************************************/

typedef enum {
    CTRL_B28     = 13,
    CTRL_HLB     = 12,
    CTRL_FSELECT = 11,
    CTRL_PSELECT = 10,
    CTRL_RESET   = 8,
    CTRL_SLEEP1  = 7,
    CTRL_SLEEP12 = 6,
    CTRL_OPBITEN = 5,
    CTRL_DIV2    = 3,
    CTRL_MODE    = 1,
} CtrlBit;  // Bit For Control Register

typedef enum {  // 可单独执行的命令：

    // Resets internal registers to 0
    CMD_RESET = (1 << CTRL_RESET),  // 0x0100

    // Disable DAC and MCLK
    CMD_SLEEP = 0x2000 | (1 << CTRL_SLEEP1) | (1 << CTRL_SLEEP12),  // 0x20C0

} CtrlCmd;

typedef enum {
    // Wirte Frequency
    FREQ0_SELECT = 0x4000,  // 0100 0000 0000 0000
    FREQ1_SELECT = 0x8000,  // 1000 0000 0000 0000
} FreqSelect;

typedef enum {
    // Wirte Phase
    PHASE0_SELECT = 0xC000,  // 1100 0000 0000 0000
    PHASE1_SELECT = 0xE000,  // 1110 0000 0000 0000
} PhaseSelect;

typedef enum {
    // Selcet Output Frequency
    FREQ0_OUTPUT = 0x2000 | (0 << CTRL_FSELECT),
    FREQ1_OUTPUT = 0x2000 | (1 << CTRL_FSELECT),
} FreqOutput;

typedef enum {
    // Selcet Output Phase
    PHASE0_OUTPUT = 0x2000 | (0 << CTRL_PSELECT),
    PHASE1_OUTPUT = 0x2000 | (1 << CTRL_PSELECT),
} PhaseOutput;

typedef enum {
    // Set Output Waveform
    WAVEFORM_SINE        = 0x2000 | (0 << CTRL_OPBITEN) | (0 << CTRL_MODE),
    WAVEFORM_TRIANGLE    = 0x2000 | (0 << CTRL_OPBITEN) | (1 << CTRL_MODE),
    WAVEFORM_HALF_SQUARE = 0x2000 | (1 << CTRL_OPBITEN) | (0 << CTRL_DIV2),  // 频率减半
    WAVEFORM_SQUARE      = 0x2000 | (1 << CTRL_OPBITEN) | (1 << CTRL_DIV2),
} Waveform;

///////////////////////////////////////////////////

void AD9833_Write(uint16_t data);

void AD9833_Init(void);
void AD9833_Reset(void);
void AD9833_SetWaveform(Waveform wave);
void AD9833_SetFrequency(FreqSelect cmd, float freq /*[0,12.5M@25M]*/);
void AD9833_SetPhase(PhaseSelect cmd, float phase /*[0,360]*/);
void AD9833_SetAmplitude(uint8_t value /*[0,255]*/);

void AD9833_SetOutput(FreqOutput freq, PhaseOutput phase);

#endif  // __AD9833_H__
