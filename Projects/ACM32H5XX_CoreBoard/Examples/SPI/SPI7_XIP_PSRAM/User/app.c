#include  "app.h"

uint8_t test_buf_1[1024*8] PSRAM;

#define BUF_LEN  1024
uint8_t TxBuffer[BUF_LEN];
uint8_t RxBuffer[BUF_LEN];

uint32_t ptest_addr = (uint32_t)&test_buf_1[512];

void msg_result(uint8_t err)
{
    if(err)
        printfS("[FAILED]@%d\r\n", err);
    else
        printfS("[PASS]\r\n");
}

void hexdump_8(uint8_t *buf, uint32_t len)
{
    uint32_t i;
    for(i = 0; i < len; i++)
        printfS("%02X ", buf[i]);
    printfS("\r\n");
}

uint32_t byte_compare(uint8_t *buf1, uint8_t *buf2, uint32_t length)
{
    uint32_t i;
    uint32_t err = 0;
    
    for(i = 0; i < length; i++)
    {
        if(buf1[i] != buf2[i])
        {
            err = i + 1;
        }
    }
    return err;
}

#define __PSRAM_BYTE_COMPARE()    byte_compare(TxBuffer, RxBuffer, BUF_LEN)

void SPI_PSRAM_CS_Pin_select(psram_info_t* info)
{
    info->CS_Port = SPI_PSRAM_CS_PORT;
    info->CS_Pin = SPI_PSRAM_CS_PIN;
    info->CS_Pin_AF = SPI_PSRAM_CS_FUNC;
}

void psram_demo_test(void)
{
    uint16_t id;
    uint8_t eid[6];
    uint32_t err = 0;
    uint32_t i,j;
    uint32_t *paddr = (uint32_t*)&test_buf_1[0];
    uint8_t *paddr_byte;
    uint16_t *paddr_halfword;
    
#ifdef HAL_DMA_MODULE_ENABLED    
    IC_APS6404M_Init(SPI7, DMA1_Channel0, DMA1_Channel1);
#else
    IC_APS6404M_Init(SPI8);
#endif
    
    printfS("PSRAM SPI clock:%dHz\r\n\r\n", HAL_RCC_GetPCLK3Freq()/get_psram_info()->hspi.Instance->BAUD);

    id = IC_APS6404M_Read_ID(&eid[0]);
    printfS("id = %02X ", (id >> 8) & 0xFF);
    printfS("%02X ", (id) & 0xFF);
    hexdump_8(eid, 6);
    
#ifdef APS1604M
    printfS("MR = %02X ", IC_APS6404M_Read_ModeRegister());
    IC_APS6404M_Write_ModeRegister(0x70);
    printfS("MR = %02X ", IC_APS6404M_Read_ModeRegister());
#endif

    printfS("case01:%-40s", "PSRAM FIFO read/write 1x mode");
    err = 0;
    for(i = 0; i < BUF_LEN; i++)
        TxBuffer[i] = 1 + i;
        
    IC_APS6404M_Write(ptest_addr, TxBuffer, BUF_LEN);
    
    IC_APS6404M_FastRead(ptest_addr, RxBuffer, BUF_LEN);
    
    err = __PSRAM_BYTE_COMPARE();  
    msg_result(err);
     
    /////////////////////////////////////////////////////////
    /*
        xip read write
    */
    printfS("case02:%-40s", "PSRAM XIP word write/byte read");
    err = 0;
    memset(RxBuffer, 0, BUF_LEN);
    paddr = (uint32_t*)ptest_addr;
    paddr_byte = (uint8_t*)ptest_addr;
    for(i = 0; i < BUF_LEN; i++)
        TxBuffer[i] = 2 + i;
    
    for(i = 0; i < BUF_LEN; i += 4)
    {
        *paddr++ = *(uint32_t*)&TxBuffer[i];
    }
    
    for(i = 0; i < BUF_LEN; i++)
    {
        RxBuffer[i] = paddr_byte[i];
    }
    
    err = __PSRAM_BYTE_COMPARE();
    msg_result(err);
    
    //////////////////////////////////////////////////////////////////////////////
    printfS("case03:%-40s", "PSRAM XIP byte read/write ");
    err = 0;
    memset(RxBuffer, 0, BUF_LEN);
    paddr_byte = (uint8_t*)ptest_addr;
    for(i = 0; i < BUF_LEN; i++)
        TxBuffer[i] = 0x10 + i;
    
    for(i = 0; i < BUF_LEN; i++)
    {
        *paddr_byte++ = TxBuffer[i];
    }
    
    paddr_byte = (uint8_t*)ptest_addr;
    
    for(i = 0; i < BUF_LEN; i++)
    {
        RxBuffer[i] = paddr_byte[i];
    }
    
    err = __PSRAM_BYTE_COMPARE();
    msg_result(err);
    
    
    ////////////////////////////////////////////////////////////
    printfS("case04:%-40s", "PSRAM XIP half-word read/write");
    err = 0;
    memset(RxBuffer, 0, BUF_LEN);
    paddr_halfword = (uint16_t*)ptest_addr;
    for(i = 0; i < BUF_LEN; i++)
        TxBuffer[i] = 0x11 + i;
    
    for(i = 0; i < BUF_LEN; i += 2)
    {
        *paddr_halfword++ = *(uint16_t*)&TxBuffer[i];
    }
    
    paddr_halfword = (uint16_t*)ptest_addr;
    for(i = 0, j = 0; i < BUF_LEN; i += 2, j++)
    {
        *(uint16_t*)&RxBuffer[i] = paddr_halfword[j];
    }
    
    err = __PSRAM_BYTE_COMPARE();
    msg_result(err);
        
    ////////////////////////////////////////////////////////////
    printfS("case05:%-40s", "PSRAM XIP word read/write");
    err = 0;
    memset(RxBuffer, 0, BUF_LEN);
    paddr = (uint32_t*)ptest_addr;
    for(i = 0; i < BUF_LEN; i++)
        TxBuffer[i] = 0x12 + i;
    
    for(i = 0; i < BUF_LEN; i += 4)
    {
        *paddr++ = *(uint32_t*)&TxBuffer[i];
    }
    
    paddr = (uint32_t*)ptest_addr;
    for(i = 0, j = 0; i < BUF_LEN; i += 4, j++)
    {
        *(uint32_t*)&RxBuffer[i] = paddr[j];
    }
    
    err = __PSRAM_BYTE_COMPARE();
    msg_result(err);

    
    ////////////////////////////////////////////////////////
    printfS("case06:%-40s", "PSRAM FIFO read/write 4x mode");
    err = 0;
    memset(RxBuffer, 0, BUF_LEN);
    
    for(i = 0; i < BUF_LEN; i++)
        TxBuffer[i] = 3 + i;

    IC_APS6404M_QPI_Write((uint32_t)ptest_addr, TxBuffer, BUF_LEN);

    IC_APS6404M_QPI_Read((uint32_t)ptest_addr, RxBuffer, BUF_LEN);
    
    err = __PSRAM_BYTE_COMPARE();
    msg_result(err);
    
    
    
#ifdef HAL_DMA_MODULE_ENABLED  
    /////////////////////////////////////////////////////////////////////////////////////////////
    printfS("case07:%-40s", "PSRAM FIFO DMA read/write 1x mode");
    err = 0;
    memset(RxBuffer, 0, BUF_LEN);
    
    for(i = 0; i < BUF_LEN; i++)
            TxBuffer[i] = 4 + i;
    
    IC_APS6404M_DMA_Write((uint32_t)ptest_addr, TxBuffer, BUF_LEN);
    
    IC_APS6404M_DMA_FastRead((uint32_t)ptest_addr, RxBuffer, BUF_LEN);
    
    err = __PSRAM_BYTE_COMPARE();
    msg_result(err);
#endif
    
    ////////////////////////////////////////////////////////
    printfS("case08:%-40s", "PSRAM FIFO read/write 4x mode");
    err = 0;
    memset(RxBuffer, 0, BUF_LEN);

    for(i = 0; i < BUF_LEN; i++)
        TxBuffer[i] = 5 + i;

    IC_APS6404M_QPI_Write((uint32_t)ptest_addr, TxBuffer, BUF_LEN);

    IC_APS6404M_QPI_Read((uint32_t)ptest_addr, RxBuffer, BUF_LEN);    
    
    err = __PSRAM_BYTE_COMPARE();
    msg_result(err);
    
#ifdef HAL_DMA_MODULE_ENABLED
    /////////////////////////////////////////////////////////////////////////////////////////////
    printfS("case09:%-40s", "PSRAM FIFO DMA read/write 1x mode");
    err = 0;
    memset(RxBuffer, 0, BUF_LEN);  
    for(i = 0; i < BUF_LEN; i++)
            TxBuffer[i] = 6 + i;
    
    IC_APS6404M_DMA_Write((uint32_t)ptest_addr, TxBuffer, BUF_LEN);
    
    IC_APS6404M_DMA_FastRead((uint32_t)ptest_addr, RxBuffer, BUF_LEN);
    
    err = __PSRAM_BYTE_COMPARE();
    msg_result(err);
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////    
    printfS("case10:%-40s", "PSRAM FIFO DMA read/write 4x mode");
    err = 0;
    memset(RxBuffer, 0, BUF_LEN);
    for(i = 0; i < BUF_LEN; i++)
            TxBuffer[i] = 7 + i;
    
    IC_APS6404M_QPI_DMA_Write((uint32_t)ptest_addr, TxBuffer, BUF_LEN);
    
    IC_APS6404M_QPI_DMA_Read((uint32_t)ptest_addr, RxBuffer, BUF_LEN);
    
    err = __PSRAM_BYTE_COMPARE();
    msg_result(err);
#endif    
    
    ////////////////////////////////////////////////////////
    printfS("case11:%-40s", "PSRAM FIFO read/write 4x mode");
    err = 0;
    memset(RxBuffer, 0, BUF_LEN);
        
    for(i = 0; i < BUF_LEN; i++)
        TxBuffer[i] = 8 + i;

    IC_APS6404M_QPI_Write((uint32_t)ptest_addr, TxBuffer, BUF_LEN);

    IC_APS6404M_QPI_Read((uint32_t)ptest_addr, RxBuffer, BUF_LEN);
    
    err = __PSRAM_BYTE_COMPARE();
    msg_result(err);
    
#ifdef HAL_DMA_MODULE_ENABLED    
    ////////////////////////////////////////////////////////////////////////////////////////////    
    printfS("case12:%-40s", "PSRAM FIFO DMA read/write 4x mode");
    err = 0;
    memset(RxBuffer, 0, BUF_LEN);
    for(i = 0; i < BUF_LEN; i++)
            TxBuffer[i] = 9 + i;
    
    IC_APS6404M_QPI_DMA_Write((uint32_t)ptest_addr, TxBuffer, BUF_LEN);
    
    IC_APS6404M_QPI_DMA_Read((uint32_t)ptest_addr, RxBuffer, BUF_LEN);
    
    err = __PSRAM_BYTE_COMPARE();
    msg_result(err);
    
    //////////////////////////////////////////////////////////////////////////////////////////// 
    printfS("case13:%-40s", "PSRAM FIFO DMA read/write 1x mode");
    err = 0;
    memset(RxBuffer, 0, BUF_LEN);    
    for(i = 0; i < BUF_LEN; i++)
            TxBuffer[i] = 10 + i;
    
    IC_APS6404M_DMA_Write((uint32_t)ptest_addr, TxBuffer, BUF_LEN);
    
    IC_APS6404M_DMA_FastRead((uint32_t)ptest_addr, RxBuffer, BUF_LEN);
    
    err = __PSRAM_BYTE_COMPARE();
    msg_result(err);
#endif    
    /////////////////////////////////////////////////////////
    /*
        xip read write
    */
    printfS("case14:%-40s", "PSRAM XIP word write/read alternately");
    err = 0;
    paddr = (uint32_t*)ptest_addr;
    paddr_byte = (uint8_t*)ptest_addr;
    memset(RxBuffer, 0, BUF_LEN);
    for(i = 0; i < BUF_LEN; i++)
        TxBuffer[i] = 11 + i;
    
    for(i = 0; i < BUF_LEN; i += 4)
    {
        *paddr = *(uint32_t*)&TxBuffer[i];
        *(uint32_t*)&RxBuffer[i] = *paddr;
        paddr++;
        if(*(uint32_t*)&TxBuffer[i] != *(uint32_t*)&RxBuffer[i])
        {
            err = i + 1;
            break;
        }
    }
    msg_result(err);
    
}


