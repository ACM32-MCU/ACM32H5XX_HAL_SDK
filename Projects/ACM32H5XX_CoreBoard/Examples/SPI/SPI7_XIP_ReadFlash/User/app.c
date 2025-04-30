/******************************************************************************
*@file  : app.c
*@brief : SPI Master Demos 
******************************************************************************/
#include  "app.h"

volatile uint8_t temp;

void printf_buff_byte(uint8_t* buff, uint32_t length)
{
	uint32_t i;

	for(i = 0; i < length; i++)
	{
		printf("%.2x ",buff[i]);	
	}
	printf("\n");
}

/************************For SPI NorFlash Test***************************/ 

#define BUFFER_LENGTH_NOR    (1024)

uint8_t TxBuffer_Nor[BUFFER_LENGTH_NOR];
uint8_t RxBuffer_Nor[BUFFER_LENGTH_NOR];

/******************************************************************************
*@brief : SPI Nor Flash Demo
*         
*@param : None
*@return: None
******************************************************************************/
void SPI_XIP_Read_Test(void)
{
    uint32_t i;
    
    uint16_t id;
    uint32_t ErrCount = 0;
    uint32_t TestMode = 0;
    
    uint32_t FLASH_ADDR = 0x08000000;
    uint8_t *pflash_addr_byte = (uint8_t*)FLASH_ADDR;
    uint16_t *pflash_addr_hfword = (uint16_t*)FLASH_ADDR;
    uint32_t *pflash_addr_word = (uint32_t*)FLASH_ADDR;
    
    uint32_t index = 100;
    
    printfS("SPI Nor Flash RW Demo(should connect to a NOR Flash. eg. W25Q32B)\r\n\r\n");

    SPI_NorFlash_Init(SPI7, SPI_BAUDRATE_PRESCALER_8);
    
    printfS("SPI7 Memory map address = 0x%08X\r\n", FLASH_ADDR);
    
    printfS("Please press the USR_PB button to continue......\r\n\r\n");
    
    //while(GPIO_PIN_SET == BSP_PB_GetState());
    
    HAL_Delay(500);
    
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        TxBuffer_Nor[i] = 1 + i;
    }

    __SWITCH_TO_FIFO_MODE();

    /* Read Read Manufacture ID and Device ID */
    id = IC_W25Qxx_Read_ID();

    printfS("Get Manufacture ID and Device ID : 0x%04X \r\n", id);
    
    printfS("Wait for Flash Erasing...\r\n");
    IC_W25Qxx_EraseChip();
    
    IC_W25Qxx_QuadConfig(true);
    
    printfS("Programing data...\r\n");
    IC_W25Qxx_PageProgram_Quad(TxBuffer_Nor, 0,   256);
    IC_W25Qxx_PageProgram_Quad(TxBuffer_Nor, 256, 256);
    IC_W25Qxx_PageProgram_Quad(TxBuffer_Nor, 512, 256);
    IC_W25Qxx_PageProgram_Quad(TxBuffer_Nor, 768, 256);

    printfS("Verifying data...\r\n");
    IC_W25Qxx_Read_Quad_Output(RxBuffer_Nor, 0, BUFFER_LENGTH_NOR);
    
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        if (TxBuffer_Nor[i] != RxBuffer_Nor[i]) 
        {
            ErrCount++;
        } 
    }

    if (ErrCount) 
        printfS("Flash data programed Fail!\r\n");
    else 
        printfS("Flash data programed Success!\r\n");
    
    
        
    __SWITCH_TO_XIP_MODE();
    
    //////////////////////////////////////////////////////
    /* flash XIP byte read */
    //printfS("flash XIP byte read test:\r\n");
    memset(RxBuffer_Nor, 0, sizeof(RxBuffer_Nor));
    index = 100;
    
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        temp = pflash_addr_byte[index++];
        RxBuffer_Nor[i] =  pflash_addr_byte[i];
    }
    
            
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        if (TxBuffer_Nor[i] != RxBuffer_Nor[i]) 
        {
            ErrCount++;
        } 
    }

    if (ErrCount) 
        printfS("SPI XIP 4X byte read Fail!!! \r\n");
    else 
        printfS("SPI XIP 4X byte read Success!!! \r\n");
    
    
    //////////////////////////////////////////////////////
    /* flash XIP half-word read */
    //printfS("flash XIP half-word read test:\r\n");
    memset(RxBuffer_Nor, 0, sizeof(RxBuffer_Nor));
    index = 100;
    
    for (i = 0; i < BUFFER_LENGTH_NOR / 2; i++)
    {
        temp = pflash_addr_hfword[index++];
        *(uint16_t*)&RxBuffer_Nor[i*2] =  pflash_addr_hfword[i];
    }
    
            
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        if (TxBuffer_Nor[i] != RxBuffer_Nor[i]) 
        {
            ErrCount++;
        } 
    }

    if (ErrCount) 
        printfS("SPI XIP 4X half-word read Fail!!! \r\n");
    else 
        printfS("SPI XIP 4X half-word read Success!!! \r\n");
    
    
    //////////////////////////////////////////////////////
    /* flash XIP word read */
    //printfS("flash XIP word read test:\r\n");
    memset(RxBuffer_Nor, 0, sizeof(RxBuffer_Nor));
    index = 100;
    
    for (i = 0; i < BUFFER_LENGTH_NOR / 4; i++)
    {
        temp = pflash_addr_word[index++];
        *(uint32_t*)&RxBuffer_Nor[i*4] =  pflash_addr_word[i];
    }
    
            
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        if (TxBuffer_Nor[i] != RxBuffer_Nor[i]) 
        {
            ErrCount++;
        } 
    }

    if (ErrCount) 
        printfS("SPI XIP 4X word read Fail!!! \r\n");
    else 
        printfS("SPI XIP 4X word read Success!!! \r\n");
    
}
