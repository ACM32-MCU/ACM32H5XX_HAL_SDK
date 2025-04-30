/******************************************************************************
*@file  : main.c
*@brief : This file provides UART Demos
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include "app.h"  
#include "sdram.h"  
#include "xccela_opi.h" 


//mpu region config paramters, max 8 regions
//region can not overlap
const MPU_RegionConfigTypeDef MPU_regionConfigs[]=
{  
   //ROM and SPI Flash lower region, cacheable,write-through; ITCM 0x00000000-0x00008000 is uncacheable even though it is configured cachable by MPU  
   {
       0x00000000,
       0x08080000-1, 
       MPU_ACCESS_ALL_READ_WRITE, 
       MPU_EXECUTE_ENABLE, 
       MPU_ATTR_WRITE_THROUGH
   },
   
   //SPI Flash upper region, ROM(0x1FF0_0000-0x1FF0_7fff), non-cacheable  
   {
       0x08080000,
       0x20000000-1,    
       MPU_ACCESS_ALL_READ_WRITE,
       MPU_EXECUTE_ENABLE,  
       MPU_ATTR_NO_CACHE,
   },
   
   //SRAM lower region, cacheable, write-back; DTCM 0x20000000-0x20008000 is uncacheable even though it is configured cachable by MPU   
   {
       0x20000000,
       0x20058000-1, 
       MPU_ACCESS_ALL_READ_WRITE,
       MPU_EXECUTE_ENABLE,  
       MPU_ATTR_WRITE_BACK,
   }, 
   
   //SRAM upper region, non-cacheable  
   {
       0x20058000,
       0x20060000-1,
       MPU_ACCESS_ALL_READ_WRITE,
       MPU_EXECUTE_ENABLE,  
       MPU_ATTR_NO_CACHE,
   },
   
   
   //Device memory type          
   {
       0x40000000,
       0x60000000-1, 
       MPU_ACCESS_ALL_READ_WRITE,
       MPU_EXECUTE_DISABLE,  
       MPU_ATTR_DEVICE,
   },
   
   //extern ram/sdram region, cacheable, write-through, can be used for code region   
   {
       0x60000000,
       0x70100000-1, 
       MPU_ACCESS_ALL_READ_WRITE,
       MPU_EXECUTE_ENABLE,  
       MPU_ATTR_WRITE_BACK,   
   },   
   
   //extern ram/sdram region, non cacheable when use DMA, can be used for data region   
   {
       0x70100000,
       0xA0000000-1,  
       MPU_ACCESS_ALL_READ_WRITE,
       MPU_EXECUTE_DISABLE,  
       MPU_ATTR_NO_CACHE,   
   },  
      
   //Device memory type          
   {
       0xA0000000,
       0xFFFFFFFF, 
       MPU_ACCESS_ALL_READ_WRITE,
       MPU_EXECUTE_DISABLE,  
       MPU_ATTR_DEVICE,
   },

};


void MPU_Config(void)
{
    MPU_ConfigInitTypeDef mpuConfigInit;
    
    mpuConfigInit.HfNmi = MPU_HFNMI_ENABLE;
    mpuConfigInit.PrivDef = MPU_PRIVDEF_DISABLE; 
    mpuConfigInit.RegionCount = sizeof(MPU_regionConfigs) / sizeof(MPU_regionConfigs[0]);
    mpuConfigInit.RegionConfigs = (MPU_RegionConfigTypeDef *)MPU_regionConfigs;
    
    HAL_MPU_Config(&mpuConfigInit); 
}


/******************************************************************************
*@brief : redefine HAL_MspInit, config mpu
*@param : none
*@return: none
******************************************************************************/
void HAL_MspInit(void)
{
#if (DATA_ACCELERATE_ENABLE == 1)
    MPU_Config();    
#endif 
}

int main(void)
{  
    HAL_Init();  
    
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);    
    
    Copy_Vector_Table_Remap();    
    
    BSP_UART_Init(USART1, 115200); 

    printfS("UART1 TXD:PA9 , RXD:PA10\r\n");  
    printfS("HCLK: %u\r\n", HAL_RCC_GetHCLKFreq() );     
    
    printfS("MPU_CTRL:%08x\r\n",MPU->CTRL);     

    #ifdef LQFP100_PSRAM
    APS6408L_OBM_OSPI_Xccela_Test(); 
    #endif  
    
    #ifdef SDRAM_SOC    
    SDRAM_Init(); 
    SDRAM_32168Write_Read_Test(); 
    SDRAM_DMA_Test();    
    #endif 
    
    DTCM_Access_Test();      
    
    while(1)
    {
        
    }

}
