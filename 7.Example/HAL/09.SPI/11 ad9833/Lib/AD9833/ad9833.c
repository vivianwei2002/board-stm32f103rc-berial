#include "ad9833.h"

#define POW_2_28 0x10000000UL  // 2^28
#define CHANGE_PER_DEG 11.37   // 4096/360
#define CLAMP(min, value, max) ((value < min) ? (min) : ((value > max) ? max : value))

void AD9833_Write(uint16_t data) {
    AD9833_CS_LOW();
    HAL_SPI_Transmit(&AD9833_SPI, (uint8_t*)&data, 1, HAL_MAX_DELAY);
    AD9833_CS_HIGH();
}

void AD9833_Init(void) {
    AD9833_Reset();
    AD9833_SetFrequency(FREQ0_SELECT, 0);
    AD9833_SetFrequency(FREQ1_SELECT, 0);
    AD9833_SetPhase(PHASE0_SELECT, 0);
    AD9833_SetPhase(PHASE1_SELECT, 0);
    AD9833_Write(0x00);  // clear reset bit
    AD9833_Write(CMD_SLEEP);
}

void AD9833_Reset(void) {
    AD9833_Write(CMD_RESET);
    HAL_Delay(1);  // 1ms
}

void AD9833_SetWaveform(Waveform wave) {
    AD9833_Write(wave);
}

void AD9833_SetFrequency(FreqSelect cmd, float freq /*hz*/) {
    uint32_t v = (uint64_t)freq * POW_2_28 / FMCLK;
    AD9833_Write(cmd | (v & 0x3FFF));          // lower 14 bits
    AD9833_Write(cmd | ((v >> 14) & 0x3FFF));  // upper 14 bits
}

void AD9833_SetPhase(PhaseSelect cmd, float phase) {
    phase      = CLAMP(0, phase, 360);
    uint16_t v = phase * CHANGE_PER_DEG;  // [0,4095]
    AD9833_Write(cmd | v);
}

float AD9833_GetResolution(void) { return (float)FMCLK / POW_2_28; }

void AD9833_SetOutput(FreqOutput freq, PhaseOutput phase) {
    AD9833_Write(freq);
    AD9833_Write(phase);
}
