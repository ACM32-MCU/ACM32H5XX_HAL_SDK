/******************************************************************************
*@file  : app.c
*@brief : HRNG Demos 
******************************************************************************/
#include "app.h"


void APP_HRNG_Test(void)
{
	uint32_t i;
	uint32_t j;
    uint8_t data_buf[64];
    
    printfS(" HRNG Test Start!\r\n");

	for(i = 8; i <= 64; i+=8)
	{
		printfS("get random number: %d\n", i);
		if(HAL_HRNG_GetHrng(data_buf, i))
		{
			printfS("random number is error\n");
			return;
		}
		for(j = 0; j < i; j++)
		{
			printfS("%x", data_buf[j]);
		}
		printfS("\n");
	}	

    printfS("\n HRNG Test End!\n");

}





