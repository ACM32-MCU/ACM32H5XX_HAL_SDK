/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 


void printf_buff_byte(uint8_t *data,uint32_t len); 


/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
	uint8_t buff[256];

    if (HAL_OK != HAL_EFUSE_Init(EFUSE2) )
    {
        printfS("RCH Not Ready\n");
        return;  
    }
    
    HAL_EFUSE_ByteWpEnable(EFUSE2);  // If eFuse has been writtern not 0, then new data is not allowed to write     
    
	HAL_EFUSE_ReadBytes(EFUSE2, 0, buff, 256, 10000);  
	printf_buff_byte(buff, 256);
    buff[0] = 0x55;  
	if (HAL_OK != HAL_EFUSE_WriteBytes(EFUSE2, EFUSE_USER_REGION_ADDR_START, buff, 1, 10000) )   
    {
        printfS("EFUSE Write Error\n");   
    }
    HAL_EFUSE_ReadBytes(EFUSE2, 0, buff, 256, 10000);  
    printf_buff_byte(buff, 256);   
}

void printf_buff_byte(uint8_t *data,uint32_t len)
{
	uint32_t i=0;
	for(i=0;i<len;i++)
	{
		printfS("%02x ",data[i]);
	}
	printfS("\r\n");
}


