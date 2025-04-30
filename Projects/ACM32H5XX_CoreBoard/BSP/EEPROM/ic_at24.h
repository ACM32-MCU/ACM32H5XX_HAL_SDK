/*
  ******************************************************************************
  * @file    IC_AT24.h
  * @author  Xiao Han
  * @version V1.0.0
  * @date    2020
  * @brief   AT24 driver header file.
  ******************************************************************************
*/
#ifndef __IC_AT24_H__
#define __IC_AT24_H__

#include "hal.h"

//#define AT24C02
#define AT24LC256

#ifdef AT24C02
    #define IC_AT24_MEMADD_SIZE     I2C_MEMADD_SIZE_8BIT
    #define AT24_PAGE_SIZE          8
#elif defined AT24LC256    
    #define IC_AT24_MEMADD_SIZE     I2C_MEMADD_SIZE_16BIT
    #define AT24_PAGE_SIZE          64
#endif


/************************************************************************
 * function   : IC_AT24_WriteBytes
 * Description: IC_AT24 write bytes
 * input      : hi2c : pointer to a I2C_HandleTypeDef structure that contains
 *                     the configuration information for I2C module
 *              DevAddress : Target device address
 *              MemAddress : MemAddress Internal memory address
 *              MemAddSize : MemAddSize Size of internal memory address
 *              pData      : Pointer to data buffer
 *              Size       : Amount of data to be sent
 * return: 
 ************************************************************************/
void IC_AT24_WriteBytes(I2C_HandleTypeDef *hi2c, uint8_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/************************************************************************
* function   : IC_AT24_ReadBytes
* Description: IC_AT24 read bytes
 * input      : hi2c : pointer to a I2C_HandleTypeDef structure that contains
 *                     the configuration information for I2C module
 *              DevAddress : Target device address
 *              MemAddress : MemAddress Internal memory address
 *              MemAddSize : MemAddSize Size of internal memory address
 *              pData      : Pointer to data buffer
 *              Size       : Amount of data to be sent
* return: 
************************************************************************/
void IC_AT24_ReadBytes(I2C_HandleTypeDef *hi2c, uint8_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);


#endif
