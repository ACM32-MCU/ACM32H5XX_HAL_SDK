/******************************************************************************
*@file  : acm32h5xx_hal_conf.c
*@brief : HAL configuration file
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

#define USB_DEBUG

#ifdef USB_DEBUG
    #define USB_PrintfS(fmt, ...)       printf(fmt, ##__VA_ARGS__)
#else
    #define USB_PrintfS(fmt, ...)         
#endif

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /* ARM Compiler V6 */
  #ifndef __weak
    #define __weak  __attribute__((weak))
  #endif
#endif

/******* enable assert in debug stage ****************************************/
//#define USE_FULL_ASSERT

#ifdef USE_FULL_ASSERT
  void assert_failed(uint8_t* file, uint32_t line);
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
#else
  #define assert_param(expr) ((void)0U)
#endif

 /******* config data if or not in extern sram *******************************/ 
//#define DATA_IN_ExtSRAM  
  
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
//#define DATA_ACCELERATE_ENABLE      (1U)    //data accelerate enable

/******* module selection ****************************************************/

#define HAL_CORTEX_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_EXTI_MODULE_ENABLED
//#define HAL_DMA_MODULE_ENABLED
//#define HAL_IWDT_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
//#define HAL_PMU_MODULE_ENABLED
//#define HAL_TIMER_MODULE_ENABLED
//#define HAL_WDT_MODULE_ENABLED 
//#define HAL_CRC_ENABLED  
//#define HAL_FAU_ENABLED  
//#define HAL_AES_ENABLED  
//#define HAL_HASH_SHA1_ENABLED 
//#define HAL_HASH_SHA256_ENABLED 
//#define HAL_HRNG_ENABLED  
//#define HAL_LPTIM_ENABLED    
//#define HAL_SPI_MODULE_ENABLED

#define USE_USB_HS  
#define HAL_PCD_MODULE_ENABLED  
#define HAL_HCD_MODULE_ENABLED  
#define HAL_EFUSE_MODULE_ENABLED
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



#ifdef HAL_OPA_MODULE_ENABLED
    #include "hal_opa.h"
#endif

#ifdef HAL_COMP_MODULE_ENABLED
    #include "hal_comp.h"
#endif

#ifdef HAL_CAN_MODULE_ENABLED
    #include "hal_can.h"
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


#ifdef HAL_CRC_ENABLED  
    #include  "hal_crc.h"
#endif  

#ifdef  HAL_AES_ENABLED
    #include  "hal_aes.h"  
#endif    

#ifdef HAL_FAU_ENABLED    
    #include  "hal_fau.h"       
#endif 

#ifdef HAL_HASH_SHA1_ENABLED 
    #include "hal_sha1.h"  
#endif 

#ifdef HAL_HASH_SHA256_ENABLED 
    #include "hal_sha256.h"  
#endif  

#ifdef HAL_HRNG_ENABLED     
    #include "hal_hrng.h"    
#endif    

#ifdef HAL_LPTIM_ENABLED        
    #include "hal_lptim.h"                            
#endif     

#ifdef HAL_PCD_MODULE_ENABLED
 
#include "LL_USB.h" 
#include "HAL_PCD.h" 
#include "HAL_USB_Def.h" 
#endif 

#ifdef HAL_HCD_MODULE_ENABLED
#include "HAL_HCD.h" 
#endif  

#ifdef HAL_EFUSE_MODULE_ENABLED
    #include "hal_efuse.h"  
#endif  

#endif 

  