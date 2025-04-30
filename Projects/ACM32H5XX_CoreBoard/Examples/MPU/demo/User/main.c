/******************************************************************************
*@file  : main.c
*@brief : main program
******************************************************************************/

#include "main.h" 
#include "app.h" 

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
   
   //extern ram  region, cacheable,write-back  
   {
       0x60000000,
       0xA0000000-1, 
       MPU_ACCESS_ALL_READ_WRITE,
       MPU_EXECUTE_ENABLE,  
       MPU_ATTR_WRITE_BACK,
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


void MPU_PrintReg(void)
{
    uint32_t i,index,attr,mair;
    printfS("CTRL:%08x\r\n",MPU->CTRL);
    for(i=0;i<8;i++)
    {
        MPU->RNR = i;
        printfS("%d,RBAR:%08x,RLAR:%08x,",i,MPU->RBAR,MPU->RLAR);
        index = (MPU->RLAR & MPU_RLAR_AttrIndx_Msk)>>MPU_RLAR_AttrIndx_Pos;
        printfS("ATTR%d:",index);
        if(index<4)
            mair = MPU->MAIR0;
        else
        {
            mair = MPU->MAIR1; 
            index -= 4;
        }
        attr = (mair>>(index*8)) & 0xff;
        
        printfS("%02x\r\n",attr);  
    }
    
    printfS("MAIR0:%08x,MAIR1:%08x\r\n",MPU->MAIR0,MPU->MAIR1); 
}

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


/******************************************************************************
*@brief : main program
*@param : none
*@return: none
******************************************************************************/
int main(void)
{
    int tick=0;
    HAL_Init();

    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);    
    HAL_DWT_Init();
    BSP_UART_Init(USART1, 115200); 
    
    printfS("\r\n\r\n******** ACM32H5XX MCU is runing ********\r\n");
    printfS("HCK: %u, PCLK1: %u\r\n", HAL_RCC_GetHCLKFreq(),HAL_RCC_GetPCLK1Freq()); 
    get_reset_source();
    printfS("\r\n"); 
    
    MPU_PrintReg();
      
    UART_TX_DMA_Init();  
    UART_Send_By_DMA();   
    
    while(1)
    {
        HAL_Delay(1000);
        printfS("tick %d\r\n",tick++);
    }

}

