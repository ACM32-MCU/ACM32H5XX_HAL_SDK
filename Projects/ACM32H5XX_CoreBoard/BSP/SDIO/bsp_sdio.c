/*!
    \file    BSP_SDIO.c
    \brief    SD card driver
    
    \version 2025-03, demo for ACM32H5XX
*/


#include "bsp_sdio.h"

static SDMMC_HandleTypeDef sdmmc;

SDIO_RESULT SD_Init(void)
{
	uint32_t	Ret;
	uint32_t	div;
		
	sdmmc.Instance = SDMMC;
	sdmmc.Init.Ch = SDMMC_CH1;
	sdmmc.Init.CardType = SD_CARD;
	sdmmc.Init.TransBW = SDMMC_TRANS_BW_4;
	sdmmc.Init.TransMode = SDMMC_MODE_SDR;
	sdmmc.Init.BusClk = SDMMC_BUS_CLK_FREQ;
	sdmmc.Init.SDVConvEn = true;
	sdmmc.Init.IDmaEn = 0;
	sdmmc.Init.SDSigVoltage = SD_CARD_IO_VCC_3V3;
	memset(&sdmmc.SdEmmcRegInfo, 0, sizeof(SDMMC_REG_INFO));

	if(HAL_OK!=(Ret = HAL_SDMMC_Init(&sdmmc)))
	{
		printfS("HAL_SDMMC_Init() failed : 0x%x\r\n", Ret);
		return SDRES_PARERR;
	}
	if(HAL_OK!=(Ret = HAL_SD_Enum(&sdmmc, FCLK*1000000)))
	{
		printfS("HAL_EMMC_Enum() failed : 0x%x\r\n", Ret);
		return SDRES_PARERR;
	}

	if(HAL_OK!=(Ret = HAL_SDMMC_Cmd9_GetCsd(&sdmmc)))
	{
		printfS("HAL_SDMMC_Cmd9_GetCsd() failed : 0x%x\r\n", Ret);
		return SDRES_PARERR;
	}

	if(HAL_OK!=(Ret = HAL_SDMMC_Cmd7_Sel(&sdmmc)))
	{
		printfS("HAL_SDMMC_Cmd7_Sel() failed : 0x%x\r\n", Ret);
		return SDRES_PARERR;
	}

	if(HAL_OK!=(Ret = HAL_SDMMC_Cmd16_SetBlkLen(&sdmmc, SDMMC_BLOCK_SIZE)))
	{
		printfS("HAL_SDMMC_Cmd16_SetBlkLen() failed : 0x%x\r\n", Ret);
		return SDRES_PARERR;
	}

	if(HAL_OK!=(Ret = HAL_SD_Acmd6_set_bus_width(&sdmmc, SDMMC_TRANS_BW_4)))
	{
		printfS("HAL_SDMMC_Cmd6_SwitchBitW() failed : 0x%x\r\n", Ret);
		return SDRES_PARERR;
	}
	div = (FCLK*1000000)/sdmmc.Init.BusClk;
	if((FCLK*1000000)%sdmmc.Init.BusClk)
	{
		div++;
	}

//	printfS("SDMMC CLK= %d Mhz\n", ((FCLK*1000000)/div)/1000000);

	if(HAL_OK!=(Ret = HAL_SDMMC_SetClk(&sdmmc, div, SDMMC_CLK_SRC_DIV0)))
	{
//		printfS("SDMMC CLK= %d Mhz\n", ((FCLK*1000000)/div)/1000000);
		return SDRES_PARERR;
	}
	
	return SDRES_OK;	
}

SDIO_RESULT SD_ReadBlock(uint32_t *readbuff, uint32_t ReadAddr)
{
	uint32_t	Ret;

	if(HAL_OK!=(Ret = HAL_SDMMC_Cmd17_RdSingle(&sdmmc, ReadAddr, readbuff)))
	{
		printfS("HAL_SDMMC_Cmd17_RdSingle() failed : 0x%x\r\n", Ret);
		return SDRES_PARERR;
	}
	
	return SDRES_OK;
}

SDIO_RESULT SD_ReadMultiBlocks(uint32_t *readbuff, uint32_t ReadAddr, uint32_t NumberOfBlocks)
{
	uint32_t	Ret;

	if(HAL_OK!=(Ret = HAL_SDMMC_Cmd18_RdMul(&sdmmc, ReadAddr, NumberOfBlocks, readbuff)))
	{
		printfS("HAL_SDMMC_Cmd18_RdSingle() failed : 0x%x\r\n", Ret);
		return SDRES_PARERR;
	}
	
	return SDRES_OK;
}

SDIO_RESULT SD_WriteBlock(uint32_t *writebuff, uint32_t WriteAddr)
{
	uint32_t	Ret;

	if(HAL_OK!=(Ret = HAL_SDMMC_Cmd24_WrSingle(&sdmmc, WriteAddr, writebuff)))
	{
		printfS("HAL_SDMMC_Cmd24_WrSingle() failed : 0x%x\r\n", Ret);
		return SDRES_PARERR;
	}
	
	return SDRES_OK;
}

SDIO_RESULT SD_WriteMultiBlocks(uint32_t *writebuff, uint32_t WriteAddr,  uint32_t NumberOfBlocks)
{
	uint32_t	Ret;

	if(HAL_OK!=(Ret = HAL_SDMMC_Cmd25_WrMul(&sdmmc, WriteAddr, NumberOfBlocks, writebuff)))
	{
		printfS("HAL_SDMMC_Cmd25_WrMul() failed : 0x%x\r\n", Ret);
		return SDRES_PARERR;
	}
   	return SDRES_OK;
}













