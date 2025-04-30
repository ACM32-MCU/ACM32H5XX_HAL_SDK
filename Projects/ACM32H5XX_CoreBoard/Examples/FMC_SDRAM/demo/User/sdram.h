

#ifndef __SDRAM_H
#define __SDRAM_H

#include "hal.h"







void SDRAM_Init(void);
void SDRAM_32168Write_Read_Test(void);
void SDRAM_Attribute_Test(void);
void SDRAM_Memcpy_Test(void);
void SDRAM_DMA_Test(void);
void SDRAM_Width_8_16_32_Write_Read_Test(uint32_t width);



#endif 



