/*
  ******************************************************************************
  * @file    IC_AT24.c
  * @author  Xiao Han
  * @version V1.0.0
  * @date    2020
  * @brief   AT24 driver Source file.
  ******************************************************************************
*/
#include "IC_AT24.h"
#include "HAL_I2C.h"

static uint8_t IC_AT24_Writepage(I2C_HandleTypeDef *hi2c, uint8_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    return HAL_I2C_Mem_Write(hi2c, DevAddress, MemAddress, IC_AT24_MEMADD_SIZE, pData, Size,Timeout);
}

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
void IC_AT24_WriteBytes(I2C_HandleTypeDef *hi2c, uint8_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint32_t len;

    len    = (AT24_PAGE_SIZE - MemAddress % AT24_PAGE_SIZE);

    if(len)
    {
        if(len > Size)      len = Size; 
        IC_AT24_Writepage(hi2c, DevAddress, MemAddress, pData, len,Timeout);
        Size -= len;
        MemAddress += len;
        pData += len;
    }

    while(Size > AT24_PAGE_SIZE)
    {
        IC_AT24_Writepage(hi2c, DevAddress, MemAddress, pData, AT24_PAGE_SIZE,Timeout);
        Size -= AT24_PAGE_SIZE;
        MemAddress +=    AT24_PAGE_SIZE;
        pData += AT24_PAGE_SIZE;
    }
    if(Size)
    {
        IC_AT24_Writepage(hi2c, DevAddress, MemAddress, pData, Size,Timeout);
    }
}

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
void IC_AT24_ReadBytes(I2C_HandleTypeDef *hi2c, uint8_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    HAL_I2C_Mem_Read(hi2c, DevAddress, MemAddress, IC_AT24_MEMADD_SIZE, pData, Size,Timeout);
}
