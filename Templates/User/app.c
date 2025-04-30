/******************************************************************************
*@file  : app.c
*@brief : UART Demos 
******************************************************************************/
#include "app.h"

HAL_StatusTypeDef GetSN(uint8_t sn[16])
{
    int i;
    uint16_t checksum,checksum1,checksum_xor;
    if(HAL_EFUSE_ReadBytes(EFUSE1,0x58,sn,16,100) != HAL_OK)
    {
        printfS("get sn fail\r\n");
        return HAL_ERROR;
    }
    checksum = HAL_EFUSE_GetCrc16(sn,13,0xffff);
    checksum1 = sn[13] | (sn[14]<<8);
    checksum_xor = sn[15] ^ sn[13];
    printfS("sn: ");
    for(i=0;i<16;i++)
    {
        printfS("%02X",sn[i]);
    }
    if((checksum==checksum1) && (checksum_xor==0xff))
    {
        printfS(", check crc ok\r\n");
        return HAL_OK;
    }
    else
    {
        printfS(", check crc fail\r\n");
        return HAL_ERROR;        
    }   
}

/******************************************************************************
*@brief : printf logging message every 1000ms on @BSP_UARTx
*@param : none
*@return: none
******************************************************************************/
int APP_Test(void)
{
    uint8_t icModel[13];
    uint16_t icCode;    
    uint32_t i;
    uint8_t sn[16];
    
    HAL_EFUSE_GetICCode(&icCode);
    HAL_EFUSE_GetICModel(icModel);
    icModel[12]=0;
    printfS("IC code:%04X,model:%s\r\n",icCode,icModel) ;     
    GetSN(sn);
    
    i = 0; 
	while(i++ < 10000)
    {
        printfS("hello, %d\r\n", (int)HAL_GetTick());
        HAL_Delay(1000);
    }
    
    return 0;   
}
