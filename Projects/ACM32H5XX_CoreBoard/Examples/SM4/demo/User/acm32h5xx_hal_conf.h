
/******************************************************************************
*@file  : acm32h5xx_hal_conf.h
*@brief : HAL configuration file
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#ifndef __ACM32H5XX_HAL_CONF_H__
#define __ACM32H5XX_HAL_CONF_H__

#include "hal_def.h"
#include "acm32h5xx.h"
#include "hal.h"

/******* enable printf in debug stage ****************************************/
#define DEBUG

#ifdef DEBUG
    #define printfS(fmt, ...)       printf(fmt, ##__VA_ARGS__)
#else
    #define printfS(fmt, ...)         
#endif

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /* ARM Compiler V6 */
  #ifndef __weak
    #define __weak  __attribute__((weak))
  #endif
#endif

/******* enable assert in debug stage ****************************************/
#define USE_FULL_ASSERT

#ifdef USE_FULL_ASSERT
  void assert_failed(uint8_t* file, uint32_t line);
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
#else
  #define assert_param(expr) ((void)0U)
#endif

 /******* config data if or not in extern sram *******************************/ 
//#define DATA_IN_ExtSRAM
  
/* Configure the Vector Table location add offset address ------------------*/
  

#ifdef  VECT_TAB_ROM
  #define VECT_TAB_ADDR     (0)
#elif defined ( VECT_TAB_SRAM )
  #define VECT_TAB_ADDR     ( SRAM_BASE_ADDR )
#elif defined ( VECT_TAB_SPI_FLASH )
  #define VECT_TAB_ADDR     ( SPI7_MEM_BASE_ADDR + 0x00002000 )
#else
  #warning vector table definition error.
#endif  
  

/******* config extern high speed osc freq and low osc freq in Hz ************/
#define XTH_VALUE                   ( 12000000U )
#define XTL_VALUE                   ( 32768U )


/******* SysTick interrupt priority  *****************************************/
#define TICK_INT_PRIORITY           ((1<<__NVIC_PRIO_BITS)-1)  

/******* SysTick ms period set ,1ms or 10ms **********************************/
#define TICK_PERIOD_1MS             (1U)
#define TICK_PERIOD_10MS            (10U)
#define TICK_PERIOD_MS              (TICK_PERIOD_1MS)  


/******* instruction and data accelerate enable ******************************/
#define INS_ACCELERATE_ENABLE       (1U)    //instruction accelerate enable 
#define DATA_ACCELERATE_ENABLE      (0U)    //data accelerate enable

/******* module selection ****************************************************/

#define HAL_CORTEX_MODULE_ENABLED
//#define HAL_EXMC_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
//#define HAL_ADC_MODULE_ENABLED
//#define HAL_DAC_MODULE_ENABLED
#define HAL_EXTI_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
//#define HAL_I2S_MODULE_ENABLED
//#define HAL_IWDT_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
//#define HAL_RTC_MODULE_ENABLED
//#define HAL_PMU_MODULE_ENABLED
//#define HAL_TIMER_MODULE_ENABLED
//#define HAL_EFLASH_MODULE_ENABLED
//#define HAL_LPUART_MODULE_ENABLED
//#define HAL_WDT_MODULE_ENABLED 
//#define HAL_FSUSB_MODULE_ENABLED
//#define HAL_CRC_ENABLED  
#define HAL_CORDIC_MODULE_ENABLED  
#define HAL_AES_MODULE_ENABLED  
#define HAL_HASH_MODULE_SHA1_ENABLED 
#define HAL_HASH_MODULE_SHA256_ENABLED 
#define HAL_HRNG_MODULE_ENABLED  
#define HAL_CDE_MODULE_ENABLED 
//#define HAL_NFM_ENABLED  
//#define HAL_LPTIM_ENABLED    
//#define HAL_FDCAN_MODULE_ENABLED    
//#define HAL_COMP_MODULE_ENABLED
//#define HAL_OPA_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#define HAL_NAND_MODULE_ENABLED
#define HAL_EFUSE_MODULE_ENABLED
#define HAL_SM4_MODULE_ENABLED

/******* include modules header file *****************************************/

    
#if ((INS_ACCELERATE_ENABLE==1) ||  (DATA_ACCELERATE_ENABLE==1))
    #include "system_accelerate.h"
#endif  
    
#ifdef HAL_CORTEX_MODULE_ENABLED
    #include "hal_cortex.h"
#endif    

#ifdef HAL_EXMC_MODULE_ENABLED
    #include "hal_exmc.h"
#endif

#ifdef HAL_DMA_MODULE_ENABLED
    #include "hal_dma.h"
#endif

#ifdef HAL_GPIO_MODULE_ENABLED
    #include "hal_gpio.h"
#endif

#ifdef HAL_UART_MODULE_ENABLED
    #include "hal_usart.h"
    #include "hal_uart.h"
    #include "hal_uart_ex.h"
#endif

#ifdef HAL_ADC_MODULE_ENABLED
    #include "hal_adc.h"
#endif

#ifdef HAL_DAC_MODULE_ENABLED
    #include "hal_dac.h"
#endif

#ifdef HAL_EXTI_MODULE_ENABLED
    #include "hal_exti.h"
#endif

#ifdef HAL_I2C_MODULE_ENABLED
    #include "hal_i2c.h"
#endif  

#ifdef HAL_I2S_MODULE_ENABLED
    #include "hal_i2s.h"
#endif

#ifdef HAL_RCC_MODULE_ENABLED
    #include "hal_rcc.h"
#endif

#ifdef HAL_RTC_MODULE_ENABLED
    #include "hal_rtc.h"
#endif

#ifdef HAL_PMU_MODULE_ENABLED
	#include "hal_pmu.h"
#endif

#ifdef HAL_SPI_MODULE_ENABLED
    #include "hal_spi.h"
#endif

#ifdef HAL_IWDT_MODULE_ENABLED
    #include "hal_iwdt.h"
#endif

#ifdef HAL_EFLASH_MODULE_ENABLED
    #include "hal_eflash.h"
#endif

#ifdef HAL_OPA_MODULE_ENABLED
    #include "hal_opa.h"
#endif

#ifdef HAL_COMP_MODULE_ENABLED
    #include "hal_comp.h"
#endif

#ifdef HAL_FDCAN_MODULE_ENABLED
    #include "hal_fdcan.h"
#endif

#ifdef HAL_LPUART_MODULE_ENABLED
    #include "hal_lpuart.h"
#endif

#ifdef HAL_WDT_MODULE_ENABLED
    #include "hal_wdt.h"
#endif

#ifdef HAL_TIMER_MODULE_ENABLED
    #include "hal_timer.h"  
    #include "hal_timer_ex.h"
#endif  

#ifdef HAL_FSUSB_MODULE_ENABLED
    #include "hal_fusb.h"
#endif

#ifdef HAL_CRC_MODULE_ENABLED  
    #include  "hal_crc.h"
#endif  

#ifdef  HAL_AES_MODULE_ENABLED
    #include  "hal_aes.h"  
#endif    

#ifdef HAL_CORDIC_MODULE_ENABLED    
    #include  "hal_cordic.h"       
#endif 

#ifdef HAL_HASH_MODULE_SHA1_ENABLED 
    #include "hal_sha1.h"  
#endif 

#ifdef HAL_HASH_MODULE_SHA256_ENABLED 
    #include "hal_sha256.h"  
#endif  

#ifdef HAL_HRNG_MODULE_ENABLED     
    #include "hal_hrng.h"    
#endif    

#ifdef HAL_LPTIM_ENABLED        
    #include "hal_lptim.h"                            
#endif 

#ifdef HAL_NAND_MODULE_ENABLED        
    #include "hal_nand.h"      
#endif 

#ifdef HAL_CDE_MODULE_ENABLED        
    #include "hal_cde.h"      
#endif 

#ifdef HAL_EFUSE_MODULE_ENABLED        
    #include "hal_efuse.h"      
#endif 

#ifdef HAL_SM4_MODULE_ENABLED        
	#include  "hal_sm4.h"    
#endif 



#endif /* __ACM32H5XX_HAL_CONF_H__  */

  