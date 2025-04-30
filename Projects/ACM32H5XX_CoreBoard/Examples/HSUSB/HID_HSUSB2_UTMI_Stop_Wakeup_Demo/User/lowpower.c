
#include "lowpower.h"


#define ROM_API_TABLE_ENTRY_ADDR    0x1FF07FD0
#define SPIX_FLASH_INSTANCE         SPI7
#define SPI_CMD_ID_NOT_CARE         0x00

typedef struct
{
    uint32_t SPI_Instance;     
    
    uint8_t  Command; //must 
    uint8_t  Delay;   //must  
    uint8_t  Cont_MID;  //must   
    uint8_t  Dummy_clks; //must     
    
    uint32_t Addr;   //must
    uint32_t Operation_length; //must 
    uint32_t Input_data_Addr; //must
    uint32_t Output_data_Addr;  //must  
            
}SPI_Interface_Parameter;  

typedef uint8_t (*SPI_API_Func)(SPI_Interface_Parameter *); 


typedef void (*ROM_Func)(uint32_t v1);


static void SimpleDelay(volatile uint32_t delay)  
{
    while(delay--)
    {
        ;
    }
}


uint8_t API_Switch_To_FIFO_Mode(void)   
{
    SPI_API_Func *ptr_api;  
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
          
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_CMD_ID_NOT_CARE;     
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = 0;  
    s_Flash_Parameter.Delay = 0xFFU;      

    ret = ptr_api[8](&s_Flash_Parameter);   

    return ret;   
}

uint8_t API_Switch_To_XIP_Mode(void)   
{
    SPI_API_Func *ptr_api;  
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_CMD_ID_NOT_CARE;     
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = 0;  
    s_Flash_Parameter.Delay = 0xFFU;      

    ret = ptr_api[9](&s_Flash_Parameter);   

    return ret;   
}  


void spi_flash_power_down(uint32_t boot_instance)
{
    ROM_Func low_power_func; 
    
    API_Switch_To_FIFO_Mode(); 
    
    low_power_func = (ROM_Func)0x1ff02eb5;   
    
    low_power_func(boot_instance);   
}

void spi_flash_exit_power_down(uint32_t boot_instance)
{
    ROM_Func low_power_exit_func; 
    
    low_power_exit_func = (ROM_Func)0x1ff02f49;   
    
    low_power_exit_func(boot_instance);   
    
    API_Switch_To_XIP_Mode();

    SimpleDelay(5000); 
}

#define ALL_WANKEUP_STATUS                      (0xFFFFFFFF)  


void Enter_Stop_RunInSram(void)
{
    /* 关闭中断 */
    __set_PRIMASK(1);
    /* 关闭Systick中断 */     
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    /* 清除Systick中断挂起标志 */    
    SCB->ICSR = BIT25;
    
    /* spi flash进入powerdown */
    spi_flash_power_down((uint32_t)SPIX_FLASH_INSTANCE); 
    
    SimpleDelay(1000);
    
    /* 使能CS低超时拉高 */
    SPIX_FLASH_INSTANCE->CS_TOUT_VAL = 0x80;
    *(uint32_t *)(SPI7_BASE_ADDR + 0x2C ) |= SPI_MEMO_ACC_CON_CS_WAIT_EN;

    
    /* BKPSRAM不进入保持状态，进入睡眠状态 */
    RCC->AHB1CKENR |= RCC_AHB1CKENR_BKPSRAMCKEN; 
    PMU->CTRL0 = (PMU->CTRL0 & (~(PMU_CTRL0_BKPRAMREN | PMU_CTRL0_BKPRAMSEN))) | \
                        PMU_CTRL0_BKPRAMREN;

    
    /* 使能RCH，且不分频输出 */ 
    RCC->RCHCR = (RCC->RCHCR & (~(RCC_RCHCR_RCHDIV))) | RCC_RCHCR_RCHEN;
    /* 等待RCH时钟稳定 */
    while (!(RCC->RCHCR & RCC_RCHCR_RCHRDY));
    /* 选择RCH模块时钟 */
    RCC->RCHCR &= ~RCC_RCHCR_RCHSEL;
    /* 系统时钟选择RCH */ 
    RCC->CCR1 &= ~RCC_CCR1_SYSCLKSEL_Msk;
    
    /* 关闭XTH */
    RCC->XTHCR &= ~RCC_XTHCR_XTHEN;
    /* 关闭XTL */
    RCC->STDBYCTRL &= ~RCC_STDBYCTRL_XTLEN; 
    /* 关闭RC32K */
    RCC->STDBYCTRL = (RCC->STDBYCTRL & ~(RCC_STDBYCTRL_RCLDIS | RCC_STDBYCTRL_RCLEN)) | \
                          (0x0A << RCC_STDBYCTRL_RCLDIS_Pos);
    /* 关闭PLL */
    RCC->PLL1CR &= ~RCC_PLL1CR_PLL1EN;
    RCC->PLL2CR &= ~RCC_PLL2CR_PLL2EN;
    RCC->PLL3CR &= ~RCC_PLL3CR_PLL3EN;
    
    /* 关闭MCO输出 */
    RCC->CLKOCR &= ~(RCC_CLKOCR_MCO2EN | RCC_CLKOCR_MCO1EN);
    
    
    /* 调整MLDO12电压 */
    
    /* RCH选择16分频输出，且自动关闭 */
    PMU->CTRL0 |= PMU_CTRL0_RCHDIV | PMU_CTRL0_RCHPDEN;

    /* 关闭LVD */
    PMU->CTRL1 &= ~PMU_CTRL1_LVDEN;
    
    /* 先关闭BOR复位，再关闭BOR */
    PMU->CTRL2 &= ~PMU_CTRL2_BORRSTEN;
    PMU->CTRL2 &= ~PMU_CTRL2_BOREN;
    
    /* 清除所有EXTI中断挂起标志 */
    EXTI->PDR1 = EXTI_ALL_LINE_MASK;
    EXTI->PDR2 = EXTI_ALL_LINE_MASK; 

    /* 清除PMU状态 */    
    PMU->STCLR = ALL_WANKEUP_STATUS;

    /* 低功耗模式选择Stop */
    PMU->CTRL0 &= ~PMU_CTRL0_LPMS;
    

    /* 等待PMU域时钟同步 */
    SimpleDelay(1000);
    
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk)); 

    /* Wait For Interrupt */   
    __WFI();
   
    /* clear SLEEPDEEP bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

    /* 调整MLDO12电压为1.2V */
    PMU->CTRL0 &= ~PMU_CTRL0_MLDO12;    

    /* 禁止CS低超时拉高 */
    *(uint32_t *)(SPI7_BASE_ADDR + 0x2C ) &= ~SPI_MEMO_ACC_CON_CS_WAIT_EN;    
    
    /* spi flash退出powerdown */
    spi_flash_exit_power_down((uint32_t)SPIX_FLASH_INSTANCE);

    /* 使能Systick中断 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; 
    /* 使能全部中断 */    
    __set_PRIMASK(0);
}

