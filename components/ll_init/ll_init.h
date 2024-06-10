#pragma once

#include <stdint.h>

void init_clock(void);
void init_sdram(void);
void init_qspi(void);
uint8_t BSP_QSPI_Init(void);
uint8_t BSP_TS_Init(uint16_t ts_SizeX, uint16_t ts_SizeY);
