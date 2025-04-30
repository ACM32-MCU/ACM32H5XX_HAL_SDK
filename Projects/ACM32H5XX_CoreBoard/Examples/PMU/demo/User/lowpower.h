
#ifndef __LOWPOWER_H__
#define __LOWPOWER_H__

#include "hal.h"



uint8_t API_Read_SPI_Flash_ID(uint8_t * p_out);
uint8_t API_Read_SPI_Flash_Unique_ID(uint8_t * p_out);



void Enter_Standby_RunInSram(uint32_t mode);
void Enter_Stop_RunInSram(uint32_t mode);











#endif


