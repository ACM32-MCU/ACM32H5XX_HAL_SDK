
#include "tkey_case.h" 
#include "app.h" 
#include "dac7311.h"

#define TEST_MODE		"TKEY"

extern TKEY_HandleTypeDef TKEY_Handle;
extern volatile uint32_t g32TkeyIrqFlag;
extern uint16_t g16Rawdata[16];

#define FLIP_PIN         (GPIO_PIN_4) //PC4 JP12-05
#define FLIP_PORT        (GPIOC)     
void Flip_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Drive     = GPIO_DRIVE_LEVEL3;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;
    
    GPIO_InitStruct.Pin       = FLIP_PIN;
    HAL_GPIO_Init(FLIP_PORT, &GPIO_InitStruct);
}

    

//从寄存器中读取值
void Read_Values_From_Registers(void)
{
    uint8_t i;
    printfS("[0x00]TKEY->SR(RO)         = 0x%08x\r\n", TKEY->SR);
    printfS("[0x04]TKEY->IER(RW)        = 0x%08x\r\n", TKEY->IER);
    printfS("[0x08]TKEY->CR(RW)         = 0x%08x\r\n", TKEY->CR);
    printfS("[0x0C]TKEY->CFGR1(RW)      = 0x%08x\r\n", TKEY->CFGR1);
    printfS("[0x10]TKEY->CFGR2(RW)      = 0x%08x\r\n", TKEY->CFGR2);
    printfS("[0x14]TKEY->INTVLR(RW)     = 0x%08x\r\n", TKEY->INTVLR);
    printfS("[0x18]TKEY->DIVR(RW)       = 0x%08x\r\n", TKEY->DIVR);
    printfS("[0x1C]TKEY->SCCR(RW)       = 0x%08x\r\n", TKEY->SCCR);
    printfS("[0x20]TKEY->TSETR(RW)      = 0x%08x\r\n", TKEY->TSETR);
    printfS("[0x24]TKEY->CXENR(RW)      = 0x%08x\r\n", TKEY->CXENR);
    printfS("[0x28]TKEY->DR(RO)         = 0x%08x\r\n", TKEY->DR);
    for(i = 0; i < 16; i++)
    {
        printfS("[0x%02x]TKEY->TH[%d](RW)   = 0x%08x\r\n", (0x2C + i * 4), i, TKEY->TH[i]);
    }
    
    for(i = 0; i < 16; i++)
    {
        printfS("[0x%02x]TKEY->CH[%d](RW)   = 0x%08x\r\n", (0x6C + i * 4), i, TKEY->CH[i]);
    }
    printfS("[0xAC]TKEY->CFLTR(RW)      = 0x%08x\r\n", TKEY->CFLTR);
    printfS("[0xB0]TKEY->NSETR(RW)      = 0x%08x\r\n", TKEY->NSETR);
    printfS("[0xB4]TKEY->TWAITR(RW)     = 0x%08x\r\n", TKEY->TWAITR);
    
}

//往寄存器中写入值
void Write_Values_To_Registers(uint32_t write_data)
{
    uint8_t i;
    TKEY->SR = write_data;
    TKEY->IER = write_data;
    TKEY->CR = write_data;
    TKEY->CFGR1 = write_data;
    TKEY->CFGR2 = write_data;
    TKEY->INTVLR = write_data;
    TKEY->DIVR = write_data;
    TKEY->SCCR = write_data;
    TKEY->TSETR = write_data;
    TKEY->CXENR = write_data;
    TKEY->DR = write_data;
    for(i = 0; i < 16; i++)
    {
        TKEY->TH[i] = write_data;
    }
    
    for(i = 0; i < 16; i++)
    {
        TKEY->CH[i] = write_data;
    }
    TKEY->CFLTR = write_data;
    TKEY->NSETR = write_data;
    TKEY->TWAITR = write_data;

}


//寄存器默认值
void Registers_Default_Value_Compare(uint32_t regAddr, uint32_t defaultVal)
{
	uint32_t rdata;
    
    rdata = (*(volatile uint32_t *)(regAddr)); //读出数据
    
    if(rdata == defaultVal)
    {
        printfS("Pass : [0x%08x] = 0x%08x\n", regAddr, defaultVal);
    }
    else
    {
        printfS("Error: [0x%08x] = 0x%08x ? 0x%08x\n", regAddr, rdata, defaultVal);
    }
    
}

//寄存器读/写
void Registers_Write_Compare(uint32_t regAddr, uint32_t wdata, uint32_t compareVal)
{
	uint32_t rdata;
    
    (*(volatile uint32_t *)(regAddr)) = wdata; //写入数据
    
    rdata = (*(volatile uint32_t *)(regAddr)); //读出数据
    
    if(rdata == compareVal)
    {
        printfS("Pass : [0x%08x] = 0x%08x\n", regAddr, rdata);
    }
    else
    {
        printfS("Error: [0x%08x] = 0x%08x ? 0x%08x\n", regAddr, rdata, compareVal);
    }
}

//寄存器默认值测试
void Register_Default_Value_Test(TKEY_TypeDef *TKEYx)
{
    uint8_t index = 0;
    uint32_t i = 0;
    uint32_t RegisterMap[][2] = 
    {
        { 0x00, 0x00000000 },
        { 0x04, 0x00000000 },
        { 0x08, 0x00000000 },
        { 0x0C, 0x0000000f },
        { 0x10, 0x00000000 },
        { 0x14, 0x00000000 },
        { 0x18, 0x00000000 },
        { 0x1C, 0x00000000 },
        { 0x20, 0x00000000 },
        { 0x24, 0x00000000 },
        { 0x28, 0x00000000 },
        { 0x2C, 0x00000000 },
        { 0x30, 0x00000000 },
        { 0x34, 0x00000000 },
        { 0x38, 0x00000000 },
        { 0x3C, 0x00000000 },
        { 0x40, 0x00000000 },
        { 0x44, 0x00000000 },
        { 0x48, 0x00000000 },
        { 0x4C, 0x00000000 },
        { 0x50, 0x00000000 },
        { 0x54, 0x00000000 },
        { 0x58, 0x00000000 },
        { 0x5C, 0x00000000 },
        { 0x60, 0x00000000 },
        { 0x64, 0x00000000 },
        { 0x68, 0x00000000 },
        { 0x6C, 0x00000000 },
        { 0x70, 0x00000000 },
        { 0x74, 0x00000000 },
        { 0x78, 0x00000000 },
        { 0x7C, 0x00000000 },
        { 0x80, 0x00000000 },
        { 0x84, 0x00000000 },
        { 0x88, 0x00000000 },
        { 0x8C, 0x00000000 },
        { 0x90, 0x00000000 },
        { 0x94, 0x00000000 },
        { 0x98, 0x00000000 },
        { 0x9C, 0x00000000 },
        { 0xA0, 0x00000000 },
        { 0xA4, 0x00000000 },
        { 0xA8, 0x00000000 },
        { 0xAC, 0x00000003 },
        { 0xB0, 0x00000000 },
        { 0xB4, 0x00000000 },
        { 0xC4, 0x00000000 },
        { 0xC8, 0x00000000 },
        { 0xCC, 0x00000000 },
        { 0xD0, 0x00000000 },
        { 0xD4, 0x00000000 },
        { 0xD8, 0x00000000 },
      
        { 0xFFFFFFFF, 0x00000000 },  //结束标志
    };
    
    printfS("*************************************************\r\n");
    for (i = 0; RegisterMap[i][0] != 0xffffffff; i++)
    {
        Registers_Default_Value_Compare((uint32_t)TKEYx + RegisterMap[i][0], RegisterMap[i][1]);
    }
    printfS("*************************************************\r\n");
    
}

//寄存器读/写属性测试
void Register_Read_Write_Attribute_Test(TKEY_TypeDef *TKEYx)
{
    uint32_t i, j;
    uint8_t index = 0;
    uint32_t RegisterMap[][9] = 
    {
        { 0x00, 0xFFFFFFFF, 0x00000000, 0xAAAAAAAA, 0x00000000, 0x55555555, 0x00000000, 0x00000000, 0x00000000 },
        { 0x04, 0xFFFFFFFF, 0x00000F07, 0xAAAAAAAA, 0x00000A02, 0x55555555, 0x00000505, 0x00000000, 0x00000000 },
        { 0x08, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
//        { 0x0C, 0xFFFFFFFF, 0x00003E0F, 0xAAAAAAAA, 0x00002A0A, 0x55555555, 0x00001405, 0x00000000, 0x00000000 },
        { 0x0C, 0xFFFFFFFF, 0x00003FFF, 0xAAAAAAAA, 0x00002AAA, 0x55555555, 0x00001555, 0x00000000, 0x00000000 },
        { 0x10, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x14, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x18, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x1C, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x20, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x24, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x28, 0xFFFFFFFF, 0x00000000, 0xAAAAAAAA, 0x00000000, 0x55555555, 0x00000000, 0x00000000, 0x00000000 },
        { 0x2C, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x30, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x34, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x38, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x3C, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x40, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x44, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x48, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x4C, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x50, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x54, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x58, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x5C, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x60, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x64, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x68, 0xFFFFFFFF, 0x000000FF, 0xAAAAAAAA, 0x000000AA, 0x55555555, 0x00000055, 0x00000000, 0x00000000 },
        { 0x6C, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x70, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x74, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x78, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x7C, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x80, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x84, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x88, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x8C, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x90, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x94, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x98, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0x9C, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0xA0, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0xA4, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0xA8, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0xAC, 0xFFFFFFFF, 0x0000001F, 0xAAAAAAAA, 0x0000000A, 0x55555555, 0x00000015, 0x00000000, 0x00000000 },
        { 0xB0, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0xB4, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0xC4, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0xC8, 0xFFFFFFFF, 0x0000FFFF, 0xAAAAAAAA, 0x0000AAAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0xCC, 0xFFFFFFFF, 0x00007FFF, 0xAAAAAAAA, 0x00002AAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0xD0, 0xFFFFFFFF, 0x00007FFF, 0xAAAAAAAA, 0x00002AAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0xD4, 0xFFFFFFFF, 0x00007FFF, 0xAAAAAAAA, 0x00002AAA, 0x55555555, 0x00005555, 0x00000000, 0x00000000 },
        { 0xD8, 0xFFFFFFFF, 0x00000007, 0xAAAAAAAA, 0x00000002, 0x55555555, 0x00000005, 0x00000000, 0x00000000 },
        { 0xFFFFFFFF }, //结束标志
    };

    printfS("*************************************************\r\n");
    __HAL_RCC_TKEY_RESET();
    printfS("/****** Write 0xffffffff ******/\r\n");
    for (i = 0; RegisterMap[i][0] != 0xffffffff; i++)
    {
        Registers_Write_Compare((uint32_t)TKEYx + RegisterMap[i][0], RegisterMap[i][1], RegisterMap[i][2]);
    }
    __HAL_RCC_TKEY_RESET();
    printfS("/****** Write 0xaaaaaaaa ******/\r\n");
    for (i = 0; RegisterMap[i][0] != 0xffffffff; i++)
    {
        Registers_Write_Compare((uint32_t)TKEYx + RegisterMap[i][0], RegisterMap[i][3], RegisterMap[i][4]);
    }
    __HAL_RCC_TKEY_RESET();
    printfS("/****** Write 0x55555555 ******/\r\n");
    for (i = 0; RegisterMap[i][0] != 0xffffffff; i++)
    {
        Registers_Write_Compare((uint32_t)TKEYx + RegisterMap[i][0], RegisterMap[i][5], RegisterMap[i][6]);
    }
    __HAL_RCC_TKEY_RESET();
    printfS("/****** Write 0x00000000 ******/\r\n");
    for (i = 0; RegisterMap[i][0] != 0xffffffff; i++)
    {
        Registers_Write_Compare((uint32_t)TKEYx + RegisterMap[i][0], RegisterMap[i][7], RegisterMap[i][8]);
    }
    printfS("*************************************************\r\n");
}


//测试案例0--测试寄存器默认值
void CMD_Case_000_Test(void)
{
    printfS("%s case_000--time : %u\r\n", TEST_MODE, HAL_GetTick());
    printfS("Register default value test\r\n");
  
    __HAL_RCC_TKEY_CLK_ENABLE();
    Read_Values_From_Registers();
    Register_Default_Value_Test(TKEY);
}
//测试案例1--测试寄存器的读写功能
void CMD_Case_001_Test(void)
{
    printfS("%s case_001--time : %u\r\n", TEST_MODE, HAL_GetTick());
    printfS("Register read and write test\r\n");
    
    __HAL_RCC_TKEY_CLK_ENABLE();
     printfS("/****** default value ******/\r\n");
     Read_Values_From_Registers();
     printfS("/****** Write 0xFFFFFFFF ******/\r\n");
     Write_Values_To_Registers(0xFFFFFFFF);
     Read_Values_From_Registers();
     __HAL_RCC_TKEY_RESET();
     printfS("/****** Write 0xAAAAAAAA ******/\r\n");
     Write_Values_To_Registers(0xAAAAAAAA);
     Read_Values_From_Registers();
     __HAL_RCC_TKEY_RESET();
     printfS("/****** Write 0x55555555 ******/\r\n");
     Write_Values_To_Registers(0x55555555);
     Read_Values_From_Registers();
     __HAL_RCC_TKEY_RESET();
     printfS("/****** Write 0x00000000 ******/\r\n");
     Write_Values_To_Registers(0x00000000);
     Read_Values_From_Registers();
    
    __HAL_RCC_TKEY_RESET();
    Register_Read_Write_Attribute_Test(TKEY);
 
}
//测试案例2--互感发送 
void CMD_Case_002_Test(void)
{
    printfS("%s case_002--time : %u\r\n", TEST_MODE, HAL_GetTick());
}
//测试案例3--EOC中断及中断标志位测试
void CMD_Case_003_Test(void)
{
   printfS("%s case_003--time : %u\r\n", TEST_MODE, HAL_GetTick());
    
    TKEY_CSD_Init();
  
    /* 开启扫描 */
    HAL_TKEY_StartScan();
    
    while(1)
    {   
        if(g32TkeyIrqFlag)
        {
            g32TkeyIrqFlag = 0;
            
            HAL_TKEY_ReadChannelData(&TKEY_Handle);
            
            printf("%d\n", TKEY_Handle.ChannelData[13].Tkey_Data->RawData);
        }
    }    
}
//测试案例4--
void CMD_Case_004_Test(void)
{
    printfS("%s case_004--time : %u\r\n", TEST_MODE, HAL_GetTick());
    
 
}
//测试案例5--
void CMD_Case_005_Test(void)
{
    printfS("%s case_005--time : %u\r\n", TEST_MODE, HAL_GetTick());
 
}
//测试案例6--
void CMD_Case_006_Test(void)
{
    printfS("Test case_T006--time : %u\r\n", HAL_GetTick());      
 
}

void CHNUM_test_CSD(void)
{
    TKEY_CSD_Init();  

    /* 使能通道扫描结束中断 */
    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);
    /* 开启扫描 */
    HAL_TKEY_StartScan();
    
    while((TKEY->SR&0x01) == 0); 
    TKEY->SR |= 0x01;
    printf("CHANNEL_NUM is %d\n", (TKEY->SR>>4)&0x0f);    
    printf("TKEY->SR is0x%x\n", TKEY->SR);    
}

//测试案例7--channel_num_test (CSD/CSA/AUTO)
void CMD_Case_007_Test(void)
{
    printfS("%s case_007--time : %u\r\n", TEST_MODE, HAL_GetTick());  
    CHNUM_test_CSD();  //CSD模式通道号测试， 需要在TKEY_CSD_Init()初始化函数中依次使能通道0-15，读出通道号确认是否正确

}
//测试案例8--通道BUSY扫描标志测试
void CMD_Case_008_Test(void)
{
    printfS("%s case_008--time : %u\r\n", TEST_MODE, HAL_GetTick());
    TKEY_CSD_Init();  

    /* 使能通道扫描结束中断 */
    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);
    /* 开启扫描 */
    HAL_TKEY_StartScan();
    
    while((TKEY->SR&0x08) == 0x00);
    printf("TKEY->SR is0x%x\n", TKEY->SR);
    while((TKEY->SR&0x08) == 0x08);
    printf("TKEY->SR is0x%x\n", TKEY->SR);    
  
}
//测试案例9--TIMEOUT标志位测试
void CMD_Case_009_Test(void)
{
    printfS("%s case_009--time : %u\r\n", TEST_MODE, HAL_GetTick());
    TKEY_CSD_Init();  

    /* 使能通道扫描结束中断 */
    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);
    /* 开启扫描 */
    HAL_TKEY_StartScan();
    printf("TKEY->SR is0x%x\n", TKEY->SR);
    while((TKEY->SR&0x04) == 0x0);
    printf("TKEY->SR is0x%x\n", TKEY->SR); 
    if((TKEY->SR&0x04) == 0x4)
    printf("CSD: timeout flag test pass\n");
    
    TKEY->SR|=0x04;
    printf("TKEY->SR is0x%x\n", TKEY->SR);
}
//测试案例10--
void CMD_Case_010_Test(void)
{
    printfS("%s case_010--time : %u\r\n", TEST_MODE, HAL_GetTick());
       
    
}

void TKEY_CSD_Init_011(void)
{
    TKEY_CSDConfigTypeDef   TKEY_CSDConfig;
    TKEY_CSAConfigTypeDef   TKEY_CSAConfig;     

    /* TKEY模式选择：CSA或CSD */
    TKEY_Handle.Init.Mode       = TKEY_MODE_CSD; 
    /* TKEY工作模式：常规或自动 */    
    TKEY_Handle.Init.WorkMode   = TKEY_WORK_MODE_NORMAL;
    /* 扫描模式：单次或连续 */     
    TKEY_Handle.Init.ScanMode   = TKEY_SCAN_MODE_SINGLE; 
    /* 屏蔽通道使能 */
    TKEY_Handle.Init.ShieldEn   = TKEY_SHIELD_DISABLE;
    /* 自动模式下按键触摸条件硬件滤波次数 */    
    TKEY_Handle.Init.FilterNum  = TKEY_FILTER_NONE; 
    /* 常规模式下采样次数 */    
    TKEY_Handle.Init.SampleNum  = TKEY_SAMPLE_1;
    /* 充电电压VLDO */    
    TKEY_Handle.Init.Vldo       = TKEY_LDO_2V4;
    /* 比较器电压VREF */      
    TKEY_Handle.Init.Vref       = TKEY_VREF_HALF_VLDO;
    /* 扫描时钟扩频使能 */    
    TKEY_Handle.Init.SpreadEn   = TKEY_SPREAD_DISABLE;
    /* TKEY通道使能 */    
    TKEY_Handle.Init.ChannelEn  = TKEY_CHANNEL_0_EN| TKEY_CHANNEL_9_EN;// | TKEY_CHANNEL_1_EN | TKEY_CHANNEL_2_EN | TKEY_CHANNEL_3_EN   | \
                                  TKEY_CHANNEL_4_EN | TKEY_CHANNEL_5_EN | TKEY_CHANNEL_6_EN | TKEY_CHANNEL_7_EN   | \
                                  TKEY_CHANNEL_8_EN | TKEY_CHANNEL_9_EN | TKEY_CHANNEL_10_EN | TKEY_CHANNEL_11_EN | \
                                  TKEY_CHANNEL_12_EN | TKEY_CHANNEL_13_EN | TKEY_CHANNEL_14_EN | TKEY_CHANNEL_15_EN;                                                                                           

    /* 扫描时钟随机抖动，扫描时钟扩频使能后才有效 */ 
    TKEY_Handle.Timing.RandomJitterNum      = TKEY_RADOM_JITTER_1; 
    /* 两组扫描之间的间隔时间：(ScanInterval+1) * RC32k */    
    TKEY_Handle.Timing.ScanInterval         = 20;
    /*  扫描时钟分频：TKCLK / (ScanClkPrescale + 1) */    
    TKEY_Handle.Timing.ScanClkPrescale      = 3;
    /* 扫描时钟SW1高电平(SW2低电平)：(ScanClkSW1HSW2L + 1)*TscanClk */ 
    TKEY_Handle.Timing.ScanClkSW1HSW2L      = 4;
    /* 扫描时钟SW1低电平(SW2高电平)：(ScanClkSW1LSW2H + 1)*TscanClk */    
    TKEY_Handle.Timing.ScanClkSW1LSW2H      = 4;
    /*CSA超时时间或CSD PWM的周期：(TimeoutOrPwmCycle + 1) * RC32k*/    
    TKEY_Handle.Timing.TimeoutOrPwmCycle    = 30; 
    
    HAL_TKEY_Init(&TKEY_Handle);
    
    /* CSD模式配置 */
    /* 放电模式选择：电阻和电流源同时放电 */ 
    TKEY_CSDConfig.DischargeMode            = TKEY_DISMS_DISCHAG_RES_CURRENT;
    /* 预充使能 */    
    TKEY_CSDConfig.PrechargeEn              = TKEY_PRECHARGE_ENABLE;
    /* 放电电流选择：0.4uA * DischargeCurrent */
    TKEY_CSDConfig.DischargeCurrent         = 2;
    /* 放电电阻选择：Vref / 512K * DischargeRes */
    TKEY_CSDConfig.DischargeRes             = 2;
    /* 采样比特流的时钟分频：TKCLK / (SampleBitClkDiv + 1) */     
    TKEY_CSDConfig.SampleBitClkDiv          = 0;
    
    HAL_TKEY_CSDConfig(&TKEY_CSDConfig);
}
//测试案例11----EOC标志位测试
void CMD_Case_011_Test(void)
{
    printfS("%s case_011--time : %u\r\n", TEST_MODE, HAL_GetTick());   
    TKEY_CSD_Init_011();  

    /* 使能通道扫描结束中断 */
//    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);
    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, DISABLE);
    /* 开启扫描 */
    HAL_TKEY_StartScan();
    
    while((TKEY->SR&0x01) == 0x00);
    printf("TKEY->SR is0x%x\n", TKEY->SR);
    if((TKEY->SR&0x01) == 0x1)
    printf("CSD: EOC flag test pass\n");    
    
    TKEY->SR|=0x01;
    printf("TKEY->SR is0x%x\n", TKEY->SR);    
       
}
//测试案例12--FSEL OUT
void CMD_Case_012_Test(void)
{
    printfS("%s case_012--time : %u\r\n", TEST_MODE, HAL_GetTick());
    

}
//测试案例13--
void CMD_Case_013_Test(void)
{
    printfS("%s case_013--time : %u\r\n", TEST_MODE, HAL_GetTick());

  
}
//测试案例14--
void CMD_Case_014_Test(void)
{
    printfS("%s case_014--time : %u\r\n", TEST_MODE, HAL_GetTick());
    
  
}
//测试案例15--比较器翻转完成中断使能  --仅对CSA有效
void CMD_Case_015_Test(void)
{
    printfS("%s case_015--time : %u\r\n", TEST_MODE, HAL_GetTick());

}

void TKEY_CSD_Init_016(void)
{
    TKEY_CSDConfigTypeDef   TKEY_CSDConfig;

    /* TKEY模式选择：CSA或CSD */
    TKEY_Handle.Init.Mode       = TKEY_MODE_CSD; 
    /* TKEY工作模式：常规或自动 */    
    TKEY_Handle.Init.WorkMode   = TKEY_WORK_MODE_NORMAL;
    /* 扫描模式：单次或连续 */     
    TKEY_Handle.Init.ScanMode   = TKEY_SCAN_MODE_CONT; 
    /* 屏蔽通道使能 */
    TKEY_Handle.Init.ShieldEn   = TKEY_SHIELD_DISABLE;
    /* 自动模式下按键触摸条件硬件滤波次数 */    
    TKEY_Handle.Init.FilterNum  = TKEY_FILTER_NONE; 
    /* 常规模式下采样次数 */    
    TKEY_Handle.Init.SampleNum  = TKEY_SAMPLE_1;
    /* 充电电压VLDO */    
    TKEY_Handle.Init.Vldo       = TKEY_LDO_2V4;
    /* 比较器电压VREF */      
    TKEY_Handle.Init.Vref       = TKEY_VREF_QUARTER_VLDO;
    /* 扫描时钟扩频使能 */    
    TKEY_Handle.Init.SpreadEn   = TKEY_SPREAD_DISABLE;
    /* TKEY通道使能 */    
    TKEY_Handle.Init.ChannelEn  = TKEY_CHANNEL_0_EN| TKEY_CHANNEL_9_EN;// | TKEY_CHANNEL_1_EN | TKEY_CHANNEL_2_EN | TKEY_CHANNEL_3_EN   | \
                                  TKEY_CHANNEL_4_EN | TKEY_CHANNEL_5_EN | TKEY_CHANNEL_6_EN | TKEY_CHANNEL_7_EN   | \
                                  TKEY_CHANNEL_8_EN | TKEY_CHANNEL_9_EN | TKEY_CHANNEL_10_EN | TKEY_CHANNEL_11_EN | \
                                  TKEY_CHANNEL_12_EN | TKEY_CHANNEL_13_EN | TKEY_CHANNEL_14_EN | TKEY_CHANNEL_15_EN;                                                                                           

    /* 扫描时钟随机抖动，扫描时钟扩频使能后才有效 */ 
    TKEY_Handle.Timing.RandomJitterNum      = TKEY_RADOM_JITTER_1; 
    /* 两组扫描之间的间隔时间：(ScanInterval+1) * RC32k */    
    TKEY_Handle.Timing.ScanInterval         = 20;
    /*  扫描时钟分频：TKCLK / (ScanClkPrescale + 1) */    
    TKEY_Handle.Timing.ScanClkPrescale      = 4;
    /* 扫描时钟SW1高电平(SW2低电平)：(ScanClkSW1HSW2L + 1)*TscanClk */ 
    TKEY_Handle.Timing.ScanClkSW1HSW2L      = 4;
    /* 扫描时钟SW1低电平(SW2高电平)：(ScanClkSW1LSW2H + 1)*TscanClk */    
    TKEY_Handle.Timing.ScanClkSW1LSW2H      = 4;
    /*CSA超时时间或CSD PWM的周期：(TimeoutOrPwmCycle + 1) * RC32k*/    
    TKEY_Handle.Timing.TimeoutOrPwmCycle    = 50; 
    
    HAL_TKEY_Init(&TKEY_Handle);
    
    /* CSD模式配置 */
    /* 放电模式选择：电阻和电流源同时放电 */ 
    TKEY_CSDConfig.DischargeMode            = TKEY_DISMS_DISCHAG_RES_CURRENT;
    /* 预充使能 */    
    TKEY_CSDConfig.PrechargeEn              = TKEY_PRECHARGE_ENABLE;
    /* 放电电流选择：0.4uA * DischargeCurrent */
    TKEY_CSDConfig.DischargeCurrent         = 2;
    /* 放电电阻选择：Vref / 512K * DischargeRes */
    TKEY_CSDConfig.DischargeRes             = 2;
    /* 采样比特流的时钟分频：TKCLK / (SampleBitClkDiv + 1) */     
    TKEY_CSDConfig.SampleBitClkDiv          = 0;
    
    HAL_TKEY_CSDConfig(&TKEY_CSDConfig);
}

//测试案例16--CSD timeout 中断
extern volatile uint32_t g32TkeyIrqFlag_timeout;

void CMD_Case_016_Test(void)
{
    printfS("%s case_016--time : %u\r\n", TEST_MODE, HAL_GetTick());  
    TKEY_CSD_Init_016();  

    /* 使能通道扫描超时中断 */
    HAL_TKEY_ITConfig(TKEY_IT_TIMEOUTIE, ENABLE);
    /* 使能通道扫描结束中断 */
//    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);
    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, DISABLE);
    /* 开启扫描 */
    HAL_TKEY_StartScan();
    
//    printf("TKEY->SR is0x%x\n", TKEY->SR);
//    while((TKEY->SR&0x04) == 0x0);
//    printf("TKEY->SR is0x%x\n", TKEY->SR); 
//    if((TKEY->SR&0x04) == 0x4)
//    printf("CSD: timeout flag test pass\n");
//    
//    TKEY->SR|=0x04;
//    printf("TKEY->SR is0x%x\n", TKEY->SR);
    
    
    while(g32TkeyIrqFlag_timeout ==0);
    g32TkeyIrqFlag_timeout = 0;
    printf("CSD: timeout IER test pass\n");   
}
//测试案例17
void CMD_Case_017_Test(void)
{
    printfS("%s case_017--time : %u\r\n", TEST_MODE, HAL_GetTick());
}
//测试案例18--通道扫描结束中断使能
void CMD_Case_018_Test(void)
{
    printfS("%s case_018--time : %u\r\n", TEST_MODE, HAL_GetTick());
    TKEY_CSD_Init();  

    /* 使能通道扫描结束中断 */
    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);
    /* 开启扫描 */
    HAL_TKEY_StartScan();
    
    printf("TKEY->SR is0x%x\n", TKEY->SR);

   
    while(1)
    {
        if(g32TkeyIrqFlag)
        {
            g32TkeyIrqFlag = 0;
            HAL_TKEY_ITConfig(TKEY_IT_EOCIE, DISABLE);

            printf("CSD: EOC IER test pass\n"); 
        }
    }
}
//测试案例19
void CMD_Case_019_Test(void)
{
    printfS("%s case_019--time : %u\r\n", TEST_MODE, HAL_GetTick());
}
//测试案例20
void CMD_Case_020_Test(void)
{
    printfS("%s case_020--time : %u\r\n", TEST_MODE, HAL_GetTick());  
}
//测试案例21
void CMD_Case_021_Test(void)
{
   
}
//测试案例22 扫描时钟扩频使能及扫描时钟抖动控制 
void CMD_Case_022_Test(void)
{
    printfS("%s case_018--time : %u\r\n", TEST_MODE, HAL_GetTick());
    TKEY_CSD_Init();  //初始化改成TKEY_SPREAD_ENABLE， TKEY_RADOM_JITTER_1_2_3_4

    /* 使能通道扫描结束中断 */
    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);
    /* 开启扫描 */
    HAL_TKEY_StartScan();
    
    printf("TKEY->SR is0x%x\n", TKEY->SR);

   
    while(1)  //示波器查看CLK2波形
    {
    }   
}
//测试案例23
void CMD_Case_023_Test(void)
{
    
}
//测试案例24
void CMD_Case_024_Test(void)
{
   
}
//测试案例25
void CMD_Case_025_Test(void)
{
    
}
//测试案例26
void CMD_Case_026_Test(void)
{
   
}
//测试案例27
void CMD_Case_027_Test(void)
{
    
}
//测试案例28
void CMD_Case_028_Test(void)
{
   
}
//测试案例29
void CMD_Case_029_Test(void)
{
    
}
//测试案例30
void CMD_Case_030_Test(void)
{
    
}
//测试案例31
void CMD_Case_031_Test(void)
{
    
}
//测试案例32
void CMD_Case_032_Test(void)
{
    printfS("%s case_018--time : %u\r\n", TEST_MODE, HAL_GetTick());
    TKEY_CSD_Init();  //初始化改成TKEY_SPREAD_ENABLE， TKEY_RADOM_JITTER_1_2_3_4

    /* 使能通道扫描结束中断 */
    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);
    /* 开启扫描 */
    HAL_TKEY_StartScan();
    
    printf("TKEY->SR is0x%x\n", TKEY->SR);

   
    while(1)  //示波器查看CLK2波形
    {
    }      
}
//测试案例33
void CMD_Case_033_Test(void)
{
//    TKEY_CSD_Init();  //初始化改成TKEY_SPREAD_ENABLE， TKEY_RADOM_JITTER_1_2_3_4

//    Flip_GPIO_Init();
//    printf("TKEY->CR = 0x%x\n", TKEY->CR);
////    TKEY->CFGR1 |= (1<<4); //MODSEL - 快速模式
////    printf("TKEY->CR = 0x%x\n", TKEY->CR); 
//    
//    /* 开启扫描 */
//    HAL_TKEY_StartScan();

//    while(1)
//    {        
//        HAL_GPIO_WritePin(FLIP_PORT, FLIP_PIN, GPIO_PIN_RESET);
//        if(g32TkeyIrqFlag)
//        {
//            g32TkeyIrqFlag = 0;
//            HAL_GPIO_WritePin(FLIP_PORT, FLIP_PIN, GPIO_PIN_SET);

//            HAL_TKEY_ReadChannelCount(g16Rawdata);
//            printfS("%d\r\n", g16Rawdata[0]);        
//        }         
//    }     
}
//测试案例34
void CMD_Case_034_Test(void)
{
    
}
//测试案例35
void CMD_Case_035_Test(void)
{
    
}
//测试案例36
void CMD_Case_036_Test(void)
{
   
}
//测试案例37
void CMD_Case_037_Test(void)
{
    
}
//测试案例38
void CMD_Case_038_Test(void)
{
    
}
//测试案例39
void CMD_Case_039_Test(void)
{
    
}
//测试案例40
void CMD_Case_040_Test(void)
{
    
}
//测试案例41
void CMD_Case_041_Test(void)
{
    
}
//测试案例42
void CMD_Case_042_Test(void)
{
    
}
//测试案例43
void CMD_Case_043_Test(void)
{
  
}
//测试案例44
void CMD_Case_044_Test(void)
{
    
}
//测试案例45
void CMD_Case_045_Test(void)
{
   
}

void Case_046_Normal_mode_test(void)
{
//    float temp1 = 0.0, temp2 = 0.0;
//    uint8_t i;
//    
//    printfS("CAT TKEY CSD tset\r\n\r\n");
//    
//    g32TkeyIrqFlag = 0;
//    
//    memset(g16Rawdata, 0, 16);
//    
//    TKEY_CSD_Init();

//    DAC7311_Init();
//    
//    switch(TKEY_Handle.Init.Vldo)
//    {
//        case TKEY_LDO_1V6:
//        {
//            printfS("VLDO Output 1.6V\r\n");
//            AO1_Set_Voltage(1.6);
//            temp1 = 1.6;            
//        }break ;
//        case TKEY_LDO_2V0:
//        {
//            printfS("VLDO Output 2.0V\r\n");
//            AO1_Set_Voltage(2.0); 
//            temp1 = 2.0;             
//        }break;
//        case TKEY_LDO_2V4:
//        {
//            printfS("VLDO Output 2.4V\r\n");
//            AO1_Set_Voltage(2.4); 
//            temp1 = 2.4;             
//        }break;
//        case TKEY_LDO_VDD33:
//        {
//            printfS("VLDO Output 3.0V\r\n");
//            AO1_Set_Voltage(3.0);
//            temp1 = 3.0;             
//        };
//        default:break;
//    }
//    
//    switch(TKEY_Handle.Init.Vref)
//    {
//        case TKEY_VREF_QUARTER_VLDO:
//        {
//            printfS("VREF Output VLDO/4\r\n");
//            temp2 = temp1 / 4.0;
//            AO2_Set_Voltage(temp2);            
//        }break ;
//        case TKEY_VREF_HALF_VLDO:
//        {
//            printfS("VREF Output VLDO/2\r\n");
//            temp2 = temp1 / 2.0;
//            AO2_Set_Voltage(temp2);  
//        }break;
//        case TKEY_VREF_THREE_QUARTER_VLDO:
//        {
//            printfS("VREF Output VLDO*3/4\r\n");
//            temp2 = temp1 *3.0 / 4.0;
//            AO2_Set_Voltage(temp2);          
//        }break;
//        case TKEY_VREF_VBG:
//        {
//            printfS("VREF Output 1.2V\r\n");
//            AO2_Set_Voltage(1.2);          
//        };
//        default:break;
//    }
//    
//    /* 使能通道扫描结束中断 */
//    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);
//    
//    /* 开启扫描 */
//    HAL_TKEY_StartScan();
//    
//    while(1)
//    {
//        if(g32TkeyIrqFlag)
//        {
//           /*Disable TKEY module*/    
//            TKEY->CR &= 0xfffe; 
//            
//            g32TkeyIrqFlag = 0;
//            
//            HAL_TKEY_ReadChannelCount(g16Rawdata);
//            for(i=0;i<12;i++)
//            {
//                printfS("%d\r\n", g16Rawdata[i]);
//            }
//            printfS("\nTKEY->DR = %d\r\n", TKEY->DR);
//            
//        }        
//    }    
}

//测试案例46 -常规模式：保存最后一个通道的扫描计数结果 - 自动模式：该寄存器保存发生按键触摸事件通道的扫描计数结果
void CMD_Case_046_Test(void)
{
    Case_046_Normal_mode_test(); //常规模式：保存最后一个通道的扫描计数结果
}
//测试案例47
void CMD_Case_047_Test(void)
{
   
}
//测试案例48
void CMD_Case_048_Test(void)
{
   
}
//测试案例49
void CMD_Case_049_Test(void)
{
   
}
//测试案例50
void CMD_Case_050_Test(void)
{
 
}
//测试案例51
void CMD_Case_051_Test(void)
{
   
}
//测试案例52
void CMD_Case_052_Test(void)
{
   
}
//测试案例53
void CMD_Case_053_Test(void)
{
    
}
//测试案例54
void CMD_Case_054_Test(void)
{
   
}
//测试案例55
void CMD_Case_055_Test(void)
{
    
}
//测试案例56
void CMD_Case_056_Test(void)
{
   
}
//测试案例57
void CMD_Case_057_Test(void)
{
    
}//测试案例58
void CMD_Case_058_Test(void)
{
    
}
//测试案例59
void CMD_Case_059_Test(void)
{
   
}
//测试案例60
void CMD_Case_060_Test(void)
{
    
}
//测试案例61
void CMD_Case_061_Test(void)
{
   
}
//测试案例62
void CMD_Case_062_Test(void)
{
    
}
//测试案例63
void CMD_Case_063_Test(void)
{
   
}
//测试案例64
void CMD_Case_064_Test(void)
{
    
}
//测试案例65
void CMD_Case_065_Test(void)
{
   
}
//测试案例66
void CMD_Case_066_Test(void)
{
   
}
//测试案例67
void CMD_Case_067_Test(void)
{
    
}
//测试案例68
void CMD_Case_068_Test(void)
{
    
}
//测试案例69
void CMD_Case_069_Test(void)
{
 
}
//测试案例70
void CMD_Case_070_Test(void)
{
    
}
//测试案例71
void CMD_Case_071_Test(void)
{
    
}
//测试案例72
void CMD_Case_072_Test(void)
{
 
}
//测试案例73
void CMD_Case_073_Test(void)
{
    
}
//测试案例74
void CMD_Case_074_Test(void)
{
    
}
//测试案例75
void CMD_Case_075_Test(void)
{
   
}
//测试案例76
void CMD_Case_076_Test(void)
{
   
}
//测试案例77
void CMD_Case_077_Test(void)
{
    
}
//测试案例78
void CMD_Case_078_Test(void)
{
    
}
//测试案例79
void CMD_Case_079_Test(void)
{
    
}
//测试案例80
void CMD_Case_080_Test(void)
{
   
}
//测试案例81
void CMD_Case_081_Test(void)
{
    
}
//测试案例82
void CMD_Case_082_Test(void)
{
    
}
//测试案例83
void CMD_Case_083_Test(void)
{
   
}
//测试案例84
void CMD_Case_084_Test(void)
{
    
}
//测试案例85
void CMD_Case_085_Test(void)
{
   
}
//测试案例86
void CMD_Case_086_Test(void)
{

}
//测试案例87
void CMD_Case_087_Test(void)
{
   
}
//测试案例88
void CMD_Case_088_Test(void)
{
   
}
//测试案例89
void CMD_Case_089_Test(void)
{
   
}
//测试案例90
void CMD_Case_090_Test(void)
{
   
}
//测试案例91
void CMD_Case_091_Test(void)
{
  
}
//测试案例92
void CMD_Case_092_Test(void)
{
   
}
//测试案例93
void CMD_Case_093_Test(void)
{
   
}
//测试案例94
void CMD_Case_094_Test(void)
{
    
}
//测试案例95
void CMD_Case_095_Test(void)
{
    
}
//测试案例96
void CMD_Case_096_Test(void)
{
   
}
//测试案例97
void CMD_Case_097_Test(void)
{
    
}
//测试案例98
void CMD_Case_098_Test(void)
{
   
}
//测试案例99
void CMD_Case_099_Test(void)
{
    
}



void TKEY_case_test(void)
{
   //测试案例0--测试寄存器默认值
//    CMD_Case_000_Test();
   //测试案例1--测试寄存器的读写功能
    CMD_Case_001_Test(); 

//   //测试案例3--EOC中断及中断标志位测试
//    CMD_Case_003_Test();

//   //测试案例7--channel_num_test (CSD/CSA/AUTO)
//    CMD_Case_007_Test();    
//   //测试案例8--通道BUSY扫描标志测试
//    CMD_Case_008_Test();
//    //测试案例9--TIMEOUT标志位测试
//    CMD_Case_009_Test();
//    
//    //测试案例16--CSD timeout 中断
//    CMD_Case_016_Test();
//    //测试案例18--通道扫描结束中断使能
//    CMD_Case_018_Test();
//    //测试案例22 扫描时钟扩频使能及扫描时钟抖动控制 
//    CMD_Case_022_Test();    
    
    
    
    
    
    
    
}




