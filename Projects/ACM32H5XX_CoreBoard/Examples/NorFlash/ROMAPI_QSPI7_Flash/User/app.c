/******************************************************************************
*@file  : app.c
*@brief : UART Demos 
******************************************************************************/
#include "app.h"



#define BUFFER_LENGTH    (128*1024)
uint8_t TEST_txBuffer[BUFFER_LENGTH] __attribute__ ((aligned (4)));
uint8_t TEST_rxBuffer[BUFFER_LENGTH] __attribute__ ((aligned (4)));

#define NORFLASH_TEST_ADDR (0x00020000)

uint8_t userkey[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};  

void printf_buff_byte(volatile uint8_t* buff, uint32_t length)
{
	uint32_t i;

	for(i=0;i<length;i++)
	{
		printf("%.2x ",buff[i]);	
	}
	printf("\n"); 
}

void APP_EncryptProgram_Test(void)
{
    uint8_t ret;
    int i;
    uint32_t starttick,delay;
    uint32_t testAddr,testLen;
    
    testAddr = 0x08000000 + NORFLASH_TEST_ADDR;
    testLen = BUFFER_LENGTH & (~(NORFLASH_PAGE_SIZE - 1));
    printfS("\r\nAPP_EncryptProgram_Test,\r\naddr:0x%x, size : %d KB\r\n", NORFLASH_TEST_ADDR, testLen/1024);
    if(testAddr & (NORFLASH_PAGE_SIZE - 1))
    {
        printfS("illegal addr\r\n");  
        return;
    }

    //擦除FLASH
    
    starttick = HAL_DWT_GetClkTick();    
    ret = HAL_NORFLASH_Erase(NORFLASH_TEST_ADDR, testLen);     
    printfS("erase time:%d ms\r\n",HAL_DWT_GetMsDelay(starttick)); 
    if(ret != HAL_OK)
       printfS("erase operation fail\r\n");   
    
    //检查是否擦除成功
    
    HAL_NORFLASH_Read(NORFLASH_TEST_ADDR, TEST_rxBuffer, testLen);       
    for(i = 0; i < testLen; i++)
    {
        if(0xff != TEST_rxBuffer[i])
            break;
    } 
    if(i != testLen)
        printfS("erase check fail,%d\r\n",i); 
    else
        printfS("erase ok\r\n");  


    for (i = 0; i < BUFFER_LENGTH; i++)
    {
        TEST_txBuffer[i] = 1 + i;  
    }
    
    //编程FLASH
    
    HAL_NORFLASH_SetEncryptionKey(userkey); 
    starttick = HAL_DWT_GetClkTick();
    for(i = 0; i < testLen / NORFLASH_PAGE_SIZE ; i++)
    {
        ret = HAL_NORFLASH_EncryptProgram(testAddr + NORFLASH_PAGE_SIZE * i, (uint32_t *)(TEST_txBuffer + NORFLASH_PAGE_SIZE * i), NORFLASH_PAGE_SIZE, (uint32_t *)TEST_rxBuffer); 
        if(ret != HAL_OK)
            printfS("encryption program operation fail,%dpage\r\n",i);
    }
    
    printfS("encryption program time:%d ms\r\n",HAL_DWT_GetMsDelay(starttick));
    
    if(ret != HAL_OK)
        printfS("encryption program operation fail\r\n");
     
    //检查是否编程成功 
    starttick = HAL_DWT_GetClkTick();
    
    
    HAL_NORFLASH_Read(testAddr, TEST_rxBuffer, testLen); 
    printfS("FIFO read(first 256 data):\n");
    printf_buff_byte((uint8_t *)TEST_rxBuffer, NORFLASH_PAGE_SIZE); 
    
     
    System_InvalidateDAccelerate_by_Addr( (uint32_t *)testAddr, 0x1000);  // Invalidate one sector  
    
    memcpy(TEST_rxBuffer, (uint8_t *)testAddr, testLen); 
    
    printfS("XIP read(first 256 data):\n");
    printf_buff_byte((uint8_t *)TEST_rxBuffer, NORFLASH_PAGE_SIZE); 
    
     
    delay = HAL_DWT_GetMsDelay(starttick);
    //printfS("XIP bytes read time:%d ms\r\n", delay);
     
    for(i = 0; i < testLen; i++)
    {
        if(TEST_txBuffer[i] != TEST_rxBuffer[i])
            break;
    } 
    if(i != testLen)
        printfS("encryption program check fail\r\n"); 
    else
        printfS("encryption program ok\r\n");       
}

uint32_t manuDeviceID;
void APP_Test(void)
{
    uint8_t ret;
    
    uint8_t identifyID[3];    
    uint8_t uniqueID[16];
    int i;
    uint32_t starttick,delay;
    
    printfS("\r\nnorflash test********************\r\n");
        
    ret = HAL_NORFLASH_ReadID((uint8_t*)&manuDeviceID);  //0x9F
    if(ret!=HAL_OK)
    {
       printfS("read mid/did operation fail\r\n"); 
    }
    else
    {
        printfS("chip mid/did: 0x%06X\r\n", manuDeviceID);        
        
    }    
    
    
    ret = HAL_NORFLASH_ReadUniqueID(uniqueID,16);
    if(ret!=HAL_OK)
    {
       printfS("read uid operation fail\r\n"); 
    }
    else
    {
        printfS("chip uid:");
        for(i = 0; i < 16; i++)
        {
            printfS("%02x ",uniqueID[i]);
            
        }
        printfS("\r\n");
        
    }
    
#if 0
    APP_EncryptProgram_Test();
    printfS("\r\nnorflash test,addr:0x%x, size : %d KB\r\n",NORFLASH_TEST_ADDR, BUFFER_LENGTH/1024);
#endif

    
    for(i = 0; i < BUFFER_LENGTH; i++)
    {
        TEST_txBuffer[i] = i + 102;
    }
    
    //擦除FLASH
    starttick = HAL_DWT_GetClkTick();    
    ret = HAL_NORFLASH_Erase(0x00020000, 128*1024);     
    printfS("erase time:%d ms\r\n",HAL_DWT_GetMsDelay(starttick)); 
    if(ret != HAL_OK)
       printfS("erase operation fail\r\n");   
    
    //检查是否擦除成功
    //HAL_NORFLASH_XipPlainRead(0x8000000+NORFLASH_TEST_ADDR,TEST_rxBuffer, BUFFER_LENGTH);
    HAL_NORFLASH_Read(NORFLASH_TEST_ADDR, TEST_rxBuffer, BUFFER_LENGTH);       
    for(i = 0; i < BUFFER_LENGTH; i++)
    {
        if(0xff != TEST_rxBuffer[i])
            break;
    } 
    if(i != BUFFER_LENGTH)
        printfS("erase check fail,%d\r\n",i); 
    else
        printfS("erase ok\r\n");     
    
    //编程FLASH
    
    starttick = HAL_DWT_GetClkTick(); 
    ret = HAL_NORFLASH_Program(NORFLASH_TEST_ADDR, TEST_txBuffer, BUFFER_LENGTH);    
    printfS("program time:%d ms\r\n", HAL_DWT_GetMsDelay(starttick));
    
     if(ret != HAL_OK)
        printfS("program operation fail\r\n");
     
    //检查是否编程成功 
    starttick = HAL_DWT_GetClkTick();      
    HAL_NORFLASH_Read(NORFLASH_TEST_ADDR, TEST_rxBuffer, BUFFER_LENGTH);   
    delay = HAL_DWT_GetMsDelay(starttick);
    printfS("read time:%d ms\r\n", delay);
     
    for(i = 0;i < BUFFER_LENGTH; i++)
    {
        if(TEST_txBuffer[i] != TEST_rxBuffer[i])
            break;
    } 
    if(i != BUFFER_LENGTH)
        printfS("program check fail\r\n"); 
    else
        printfS("program ok\r\n"); 
    

    for(i = 0; i < BUFFER_LENGTH; i++)
    {
        TEST_txBuffer[i] = i + 200;
    } 
    
    //修改FLASH数据
    
    starttick = HAL_DWT_GetClkTick(); 
    ret = HAL_NORFLASH_ModifyData(NORFLASH_TEST_ADDR, TEST_txBuffer, BUFFER_LENGTH);    
    printfS("ModifyData time:%d ms\r\n", HAL_DWT_GetMsDelay(starttick));
    
    if(ret != HAL_OK)
        printfS("ModifyData operation fail\r\n");
    
    starttick = HAL_DWT_GetClkTick();       
    HAL_NORFLASH_Read(NORFLASH_TEST_ADDR, TEST_rxBuffer, BUFFER_LENGTH);
    delay = HAL_DWT_GetMsDelay(starttick);
    printfS("read time:%d ms\r\n", delay);
    
    for(i = 0; i < BUFFER_LENGTH; i++)
    {
        if(TEST_txBuffer[i] != TEST_rxBuffer[i])
            break; 
    } 
    if(i != BUFFER_LENGTH)
        printfS("ModifyData check fail\r\n"); 
    else
        printfS("ModifyData ok\r\n");
      
}
