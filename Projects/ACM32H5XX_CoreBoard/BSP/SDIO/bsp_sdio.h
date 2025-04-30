/*!
    \file    BSP_SDIO.h
    \brief   the header file of SD card driver
    
    \version 2025-03, demo for ACM32H5XX
*/

#ifndef BSP_SDIO_H
#define BSP_SDIO_H

#include "acm32h5xx_coreboard.h"
#include "hal.h"


/* Results of Disk Functions */
typedef enum {
  SDRES_OK = 0,		/* 0: Successful */
	SDRES_ERROR,		/* 1: R/W Error */
	SDRES_WRPRT,		/* 2: Write Protected */
	SDRES_NOTRDY,		/* 3: Not Ready */
	SDRES_PARERR		/* 4: Invalid Parameter */
} SDIO_RESULT;


SDIO_RESULT SD_Init(void);
SDIO_RESULT SD_ReadBlock(uint32_t *readbuff, uint32_t ReadAddr);
SDIO_RESULT SD_ReadMultiBlocks(uint32_t *readbuff, uint32_t ReadAddr, uint32_t NumberOfBlocks);
SDIO_RESULT SD_WriteBlock(uint32_t *writebuff, uint32_t WriteAddr);
SDIO_RESULT SD_WriteMultiBlocks(uint32_t *writebuff, uint32_t WriteAddr,  uint32_t NumberOfBlocks);












#endif /* BSP_SDIO_H */