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
void SPI_Nand_Test(void)
{
    uint32_t i;
    
    uint16_t id;
    uint32_t ErrCount = 0;   
    
    printfS("SPI Nand Flash RW Demo\r\n\r\n");

    SPI_NandFlash_Init(SPI7, SPI_BAUDRATE_PRESCALER_2);
    
    printfS("Please press the USR_PB button to continue......\r\n\r\n");
    
    //while(GPIO_PIN_SET == BSP_PB_GetState());
    
    HAL_Delay(500);
    
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        TxBuffer_Nor[i] = 1 + i;
    }

//    __SWITCH_TO_FIFO_MODE();

    /* Read Read Manufacture ID and Device ID */
    id = ic_th58cvg3_readid();
    
    printfS("block lock info = %x\r\n", ic_th58cvg3_get_feature(0xA0));//
    
    if(ic_th58cvg3_block_unlock())
        printfS("block unlock success\r\n");
    else
        printfS("block unlock failed\r\n");
    
    if(ic_th58cvg3_erase_block(0))
         printfS("block 0 erase success\r\n");
    else
        printfS("block 0 erase failed\r\n");
    
    ic_th58cvg3_read(0x0, 0, RxBuffer_Nor, sizeof(RxBuffer_Nor));
    
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        if (0xFF != RxBuffer_Nor[i]) 
        {
            ErrCount++;
        } 
    }
    
    if (ErrCount) 
        printfS("block erase Fail!\r\n");
    else 
        printfS("block erase Success!\r\n");
    
    
    ErrCount = 0;
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        TxBuffer_Nor[i] = 1 + i;
    }
    
    if(ic_th58cvg3_program(0, 0, TxBuffer_Nor, sizeof(TxBuffer_Nor)))
        printfS("program status success\r\n");
    else
        printfS("program status failed\r\n");
    
    
    
    ic_th58cvg3_read(0, 0, RxBuffer_Nor, sizeof(RxBuffer_Nor));
    
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        if (TxBuffer_Nor[i] != RxBuffer_Nor[i]) 
        {
            ErrCount++;
        } 
    }
    
    if (ErrCount) 
        printfS("Flash data 1x programed Fail!\r\n");
    else 
        printfS("Flash data 1x programed Success!\r\n");
    
    ///////////////////////////////////////////////////////////////////////////////
    printfS("--------------------------\r\n");
    ErrCount = 0;
    if(ic_th58cvg3_erase_block(0x40))
         printfS("block 1 erase success\r\n");
    else
        printfS("block 1 erase failed\r\n");
    
    ic_th58cvg3_read(0x40, 0, RxBuffer_Nor, sizeof(RxBuffer_Nor));
    
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        if (0xFF != RxBuffer_Nor[i]) 
        {
            ErrCount++;
        } 
    }
    
    if (ErrCount) 
        printfS("block erase Fail!\r\n");
    else 
        printfS("block erase Success!\r\n");
    
    
    ////
    ErrCount = 0;
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        TxBuffer_Nor[i] = 0xAA + i;
    }
    
    ic_th58cvg3_enable_4x_mode();
    
    //block 1, page 0
    if(ic_th58cvg3_program_4x(0x40, 0, TxBuffer_Nor, sizeof(TxBuffer_Nor)))
        printfS("program status success\r\n");
    else
        printfS("program status failed\r\n");
    
    ic_th58cvg3_read(0x40, 0, RxBuffer_Nor, sizeof(RxBuffer_Nor));
    
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        if (TxBuffer_Nor[i] != RxBuffer_Nor[i]) 
        {
            ErrCount++;
        } 
    }
    
    if (ErrCount) 
        printfS("Flash data programed_4x Fail!\r\n");
    else 
        printfS("Flash data programed_4x Success!\r\n");
    
    
    memset(RxBuffer_Nor, 0, BUFFER_LENGTH_NOR);    
    ErrCount = 0;
    ic_th58cvg3_read_4x(0x40, 0, RxBuffer_Nor, sizeof(RxBuffer_Nor));
    
    for (i = 0; i < BUFFER_LENGTH_NOR; i++)
    {
        if (TxBuffer_Nor[i] != RxBuffer_Nor[i]) 
        {
            ErrCount++;
        } 
    }
    
    if (ErrCount) 
        printfS("Flash data read_4x Fail!\r\n");
    else 
        printfS("Flash data read_4x Success!\r\n");
}
