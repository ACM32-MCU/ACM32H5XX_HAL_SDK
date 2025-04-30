
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

    SimpleDelay(50000);
}

void Enter_Standby_RunInSram(uint32_t mode)    
{
    /* 关闭Systick中断 */     
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;    
    /* 关闭中断 */
    __set_PRIMASK(1);
    /* 清除Systick中断挂起标志 */    
    SCB->ICSR = BIT25;// clear systick pending bit
    
    spi_flash_power_down((uint32_t)SPIX_FLASH_INSTANCE); 
    
    SimpleDelay(1000);
   
    /* 使能CS低超时拉高 */
    SPIX_FLASH_INSTANCE->CS_TOUT_VAL = 0x80;
    *(uint32_t *)(SPI7_BASE_ADDR + 0x2C ) |= SPI_MEMO_ACC_CON_CS_WAIT_EN;
        
    PMU->STCLR = ALL_WANKEUP_STATUS;

    MODIFY_REG(PMU->CTRL0, PMU_CTRL0_LPMS_Msk, PMU_CTRL0_LPMS_STANDBY);//Standby Mode

    SimpleDelay(1000);//Waiting for PMU area clock synchronization
    
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk)); 


    if(mode == WAIT_FOR_INT)
    {
        /* Wait For Interrupt */   
        __WFI();
    }
    else
    {
        /* Wait For Event */  
        __SEV();   
        __WFE();
        __WFE();    
    }


    /* clear SLEEPDEEP bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

    spi_flash_exit_power_down((uint32_t)SPIX_FLASH_INSTANCE); 


    /* 使能全部中断 */    
    __set_PRIMASK(0);    
    /* 使能Systick中断 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;     
}

#if 1
void Enter_Stop_RunInSram(uint32_t mode)
{
    /* 关闭Systick中断 */     
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;    
    /* 关闭中断 */
    __set_PRIMASK(1);
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

    
    /* 使能RCH */ 
    RCC->RCHCR |= RCC_RCHCR_RCHEN;
    /* 等待RCH时钟稳定 */
    while (!(RCC->RCHCR & RCC_RCHCR_RCHRDY));
    /* 选择RCH模块时钟 */
    RCC->RCHCR &= ~RCC_RCHCR_RCHSEL;
    /* 系统时钟选择RCH */ 
    RCC->CCR1 &= ~RCC_CCR1_SYSCLKSEL_Msk;
    /* RCH不分频输出 */
    RCC->RCHCR &= (~(RCC_RCHCR_RCHDIV));  
    
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
    
    
    /* 调整MLDO12电压为1.0V */
    //PMU->CTRL0 = (PMU->CTRL0 & (~PMU_CTRL0_MLDO12)) | (0x1 << PMU_CTRL0_MLDO12_Pos);
    
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


    if(mode == WAIT_FOR_INT)
    {
        /* Wait For Interrupt */   
        __WFI();
    }
    else
    {
        /* Wait For Event */  
        __SEV();   
        __WFE();
        __WFE();    
    }


    /* clear SLEEPDEEP bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

    /* 调整MLDO12电压为1.2V */
    //PMU->CTRL0 &= ~PMU_CTRL0_MLDO12;    

    /* 禁止CS低超时拉高 */
    *(uint32_t *)(SPI7_BASE_ADDR + 0x2C ) &= ~SPI_MEMO_ACC_CON_CS_WAIT_EN;    
    
    /* spi flash退出powerdown */
    spi_flash_exit_power_down((uint32_t)SPIX_FLASH_INSTANCE);


    /* 使能全部中断 */    
    __set_PRIMASK(0);    
    /* 使能Systick中断 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; 
}

#else

void Enter_Stop_RunInSram(uint32_t mode)
{
    uint32_t ahb1ck, ahb2ck, ahb3ck, apb1ck1, apb1ck2, apb2ck, apb3ck, apb4ck;
    uint32_t pamode, pbmode, pcmode, pdmode, pemode, pfmode, pgmode, phmode;
    uint32_t pimode, pjmode, pkmode, plmode, pmmode, pnmode, pomode, ppmode, pqmode;  


    /* 关闭Systick中断 */     
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;    
    /* 关闭中断 */
    __set_PRIMASK(1);
    /* 清除Systick中断挂起标志 */    
    SCB->ICSR = BIT25;// clear systick pending bit
    
    /* 保存原有使能的外设时钟 */
    ahb1ck  = RCC->AHB1CKENR;
    ahb2ck  = RCC->AHB2CKENR;
    ahb3ck  = RCC->AHB3CKENR;
    apb1ck1 = RCC->APB1CKENR1;
    apb1ck2 = RCC->APB1CKENR2;
    apb2ck  = RCC->APB2CKENR;
    apb3ck  = RCC->APB3CKENR;
    apb4ck  = RCC->APB4CKENR;
    
    /* 保存原有GPIO模式 */
    pamode = GPIOA->MD;
    pbmode = GPIOB->MD;
    pcmode = GPIOC->MD;
    pdmode = GPIOD->MD;
    pemode = GPIOE->MD;
    pfmode = GPIOF->MD;
    pgmode = GPIOG->MD;
    phmode = GPIOH->MD;
    pimode = GPIOI->MD;
    pjmode = GPIOJ->MD;
    pkmode = GPIOK->MD;
    plmode = GPIOL->MD;
    pmmode = GPIOM->MD;
    pnmode = GPION->MD;
    pomode = GPIOO->MD;
    ppmode = GPIOP->MD;
    pqmode = GPIOQ->MD;


    
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


    /* GPIO配置为模拟 */
    RCC->AHB2CKENR = 0x0001FFFF;
    GPIOA->MD = 0xffffffff;
    GPIOB->MD = 0xffffffff;
    GPIOC->MD = 0xf3ffffff;//PC13
    GPIOD->MD = 0xffffffff;
    GPIOE->MD = 0xffffffff;
    GPIOF->MD = 0xffffffff;
    GPIOG->MD = 0xffffffff;
    GPIOH->MD = 0xffffffff;
    GPIOI->MD = 0xffffffff;
    GPIOJ->MD = 0xffffffff;
    GPIOK->MD = 0xffffffff;
    GPIOL->MD = 0xffffffff;
    GPIOM->MD = 0xffffffff;
    GPION->MD = 0xffffffff;
    GPIOO->MD = 0xffffffff;
    GPIOP->MD = 0xffffffff;
    GPIOQ->MD = 0xffffffff;
    
    
    /* 关闭不必要的外设时钟 */
    RCC->AHB1CKENR  = 0;
    RCC->AHB2CKENR  = RCC_AHB2CKENR_GPIOCCKEN;
    RCC->AHB3CKENR  = 0;
    RCC->APB1CKENR1 = RCC_APB1CKENR1_PMUCKEN;
    RCC->APB1CKENR2 = 0;
    RCC->APB2CKENR  = RCC_APB2CKENR_EXTICKEN;
    RCC->APB3CKENR  = 0;
    RCC->APB4CKENR  = 0;
    
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
    
    
    /* 调整MLDO12电压为1.0V */
    PMU->CTRL0 = (PMU->CTRL0 & (~PMU_CTRL0_MLDO12)) | (0x1 << PMU_CTRL0_MLDO12_Pos);
    
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


    if(mode == WAIT_FOR_INT)
    {
        /* Wait For Interrupt */   
        __WFI();
    }
    else
    {
        /* Wait For Event */  
        __SEV();   
        __WFE();
        __WFE();    
    }


    /* clear SLEEPDEEP bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

    /* 调整MLDO12电压为1.2V */
    //PMU->CTRL0 &= ~PMU_CTRL0_MLDO12;    
    
    /* 恢复原有使能外设时钟 */
    RCC->AHB1CKENR  = ahb1ck;
    RCC->AHB2CKENR  = ahb2ck;
    RCC->AHB3CKENR  = ahb3ck;
    RCC->APB1CKENR1 = apb1ck1;
    RCC->APB1CKENR2 = apb1ck2;
    RCC->APB2CKENR  = apb2ck;
    RCC->APB3CKENR  = apb3ck;
    RCC->APB4CKENR  = apb4ck;
    
    /* 恢复原有GPIO模式 */
    GPIOA->MD = pamode;
    GPIOB->MD = pbmode;
    GPIOC->MD = pcmode;
    GPIOD->MD = pdmode;
    GPIOE->MD = pemode;
    GPIOF->MD = pfmode;
    GPIOG->MD = pgmode;
    GPIOH->MD = phmode;
    GPIOI->MD = pimode;
    GPIOJ->MD = pjmode;
    GPIOK->MD = pkmode;
    GPIOL->MD = plmode;
    GPIOM->MD = pmmode;
    GPION->MD = pnmode;
    GPIOO->MD = pomode;
    GPIOP->MD = ppmode;
    GPIOQ->MD = pqmode;

    /* 禁止CS低超时拉高 */
    *(uint32_t *)(SPI7_BASE_ADDR + 0x2C ) &= ~SPI_MEMO_ACC_CON_CS_WAIT_EN;    
    
    /* spi flash退出powerdown */
    spi_flash_exit_power_down((uint32_t)SPIX_FLASH_INSTANCE);


    /* 使能全部中断 */    
    __set_PRIMASK(0);    
    /* 使能Systick中断 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; 
}

#endif