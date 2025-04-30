#ifndef __DEMO_H__
#define __DEMO_H__

#include  "hal.h"
#include "IC_APS6404M.h"
#ifdef HAL_DMA_MODULE_ENABLED
#include "HAL_DMA.h"
#endif

/*
f1_cs#      pc0

*/


/* paltform dependent */
#ifndef SPI_PSRAM_CS_PORT
#define SPI_PSRAM_CS_PORT						GPIOM
#define SPI_PSRAM_CS_PIN						GPIO_PIN_1
#define SPI_PSRAM_CS_FUNC                       GPIO_FUNCTION_15
#endif

void psram_demo_test(void);

#endif



