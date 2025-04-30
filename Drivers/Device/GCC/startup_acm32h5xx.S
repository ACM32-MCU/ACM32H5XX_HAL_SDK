/**
  *************** (C) COPYRIGHT 2025 aisinochip ************************
  * @file      startup_acm32h5xx.S
  * @brief     acm32h5xx Devices vector table for Atollic toolchain.
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Configure the clock system   
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Star-MC1 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  */

  .syntax unified
  .cpu cortex-m33
  .fpu softvfp 
  .thumb

.global g_pfnVectors
.global Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

/*.equ  BootRAM, 0xF108F85F */

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:

/* Copy the data segment initializers from flash to SRAM */
  movs r1, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r3, =_sidata
  ldr r3, [r3, r1]
  str r3, [r0, r1]
  adds r1, r1, #4

LoopCopyDataInit:
  ldr r0, =_sdata
  ldr r3, =_edata
  adds r2, r0, r1
  cmp r2, r3
  bcc CopyDataInit
  ldr r2, =_sbss
  b LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
  movs r3, #0
  str r3, [r2], #4

LoopFillZerobss:
  ldr r3, = _ebss
  cmp r2, r3
  bcc FillZerobss

/* Call the clock system intitialization function.*/
  /*  bl  SystemInit */
/* Call static constructors */
  /*  bl __libc_init_array */
/* Call the application's entry point.*/
  bl main
  bx lr
.size Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
  .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
  .size g_pfnVectors, .-g_pfnVectors


g_pfnVectors:

  .word _estack
  .word Reset_Handler
  .word NMI_Handler
  .word HardFault_Handler
  .word MemManage_Handler
  .word BusFault_Handler
  .word UsageFault_Handler
  .word 0
  .word 0
  .word 0
  .word 0
  .word SVC_Handler
  .word DebugMon_Handler
  .word 0
  .word PendSV_Handler
  .word SysTick_Handler

  /* External Interrupts */
    .word   WDT_IRQHandler                  // 0 
    .word   LVD_IRQHandler                  // 1                
    .word   RTC_XTLSD_IRQHandler            // 2
    .word   RSV3_IRQHandler                 // 3                    
    .word   RSV4_IRQHandler                 // 4
    .word   RSV5_IRQHandler                 // 5
    .word   CLKRDY_IRQHandler               // 6
    .word   EXTI0_IRQHandler                // 7
    .word   EXTI1_IRQHandler                // 8
    .word   EXTI2_IRQHandler                // 9
    .word   EXTI3_IRQHandler                // 10
    .word   EXTI4_IRQHandler                // 11
    .word   RSV12_IRQHandler                // 12                   
    .word   RSV13_IRQHandler                // 13
    .word   ADC12_IRQHandler                // 14
    .word   ADC3_IRQHandler                 // 15
    .word   DAC1_IRQHandler                 // 16
    .word   COMP1_IRQHandler                // 17
    .word   USBOTG1_IRQHandler              // 18                      
    .word   FDCAN1_IRQHandler               // 19    
    .word   FDCAN2_IRQHandler               // 20     
    .word   EXTI9_5_IRQHandler              // 21
    .word   TIM1_BRK_UP_TRG_COM_IRQHandler  // 22
    .word   TIM1_CC_IRQHandler              // 23
    .word   TIM2_IRQHandler                 // 24
    .word   TIM3_IRQHandler                 // 25
    .word   TIM6_IRQHandler                 // 26
    .word   TIM7_IRQHandler                 // 27
    .word   TIM8_BRK_UP_TRG_COM_IRQHandler  // 28    
    .word   TIM8_CC_IRQHandler              // 29    
    .word   TIM15_IRQHandler                // 30
    .word   TIM16_IRQHandler                // 31    
    .word   TIM17_IRQHandler                // 32
    .word   I2C1_IRQHandler                 // 33
    .word   I2C2_IRQHandler                 // 34                   
    .word   SPI1_IRQHandler                 // 35                     
    .word   SPI2_IRQHandler                 // 36:               
    .word   SPI3_IRQHandler                 // 37
    .word   I2S1_IRQHandler                 // 38
    .word   I2S2_IRQHandler                 // 39
    .word   USART1_IRQHandler               // 40                    
    .word   USART2_IRQHandler               // 41
    .word   USART3_IRQHandler               // 42                    
    .word   USART4_IRQHandler               // 43
    .word   EXTI15_10_IRQHandler            // 44  
    .word   USBOTG1_WKUP_IRQHandler         // 45
    .word   LPUART1_IRQHandler              // 46                    
    .word   LPTIM1_IRQHandler               // 47
    .word   USBOTG2_WKUP_IRQHandler         // 48
    .word   AES_IRQHandler                  // 49
    .word   FPU_IRQHandler                  // 50
    .word   USBOTG2_IRQHandler              // 51
    .word   DCMI_IRQHandler                 // 52
    .word   TIM4_IRQHandler                 // 53
    .word   RSV54_IRQHandler                // 54
    .word   IWDT_WKUP_IRQHandler            // 55
    .word   LTDC_IRQHandler                 // 56
    .word   LTDC_ERR_IRQHandler             // 57
    .word   DMA2D_IRQHandler                // 58
    .word   LPTIM2_IRQHandler               // 59
    .word   LPTIM3_IRQHandler               // 60
    .word   LPTIM4_IRQHandler               // 61
    .word   LPTIM5_IRQHandler               // 62
    .word   LPTIM6_IRQHandler               // 63
    .word   AES_SPI1_IRQHandler             // 64
    .word   I2S3_IRQHandler                 // 65
    .word   SPI4_IRQHandler                 // 66
    .word   SPI5_IRQHandler                 // 67
    .word   SPI6_IRQHandler                 // 68
    .word   I2C3_IRQHandler                 // 69
    .word   I2C4_IRQHandler                 // 70
    .word   FDCAN3_IRQHandler               // 71
    .word   RSV72_IRQHandler                // 72
    .word   ETH_IRQHandler                  // 73
    .word   ETH_WKUP_IRQHandler             // 74
    .word   SDMMC_IRQHandler                // 75
    .word   USART5_IRQHandler               // 76
    .word   USART6_IRQHandler               // 77
    .word   USART7_IRQHandler               // 78
    .word   USART8_IRQHandler               // 79
    .word   USART9_IRQHandler               // 80
    .word   USART10_IRQHandler              // 81
    .word   DAC2_IRQHandler                 // 82
    .word   TIM5_IRQHandler                 // 83
    .word   TIM9_IRQHandler                 // 84
    .word   TIM10_IRQHandler                // 85
    .word   TIM11_IRQHandler                // 86
    .word   TIM12_IRQHandler                // 87
    .word   TIM13_IRQHandler                // 88
    .word   TIM14_IRQHandler                // 89
    .word   TIM18_IRQHandler                // 90
    .word   TIM19_IRQHandler                // 91
    .word   TIM20_BRK_UP_TRG_COM_IRQHandler // 92
    .word   TIM20_CC_IRQHandler             // 93
    .word   TIM21_IRQHandler                // 94
    .word   TIM22_IRQHandler                // 95
    .word   TIM23_IRQHandler                // 96
    .word   TIM24_IRQHandler                // 97
    .word   TIM25_IRQHandler                // 98
    .word   TIM26_IRQHandler                // 99
    .word   SPI7_IRQHandler                 // 100
    .word   SPI8_IRQHandler                 // 101
    .word   OSPI1_IRQHandler                // 102
    .word   OSPI2_IRQHandler                // 103
    .word   RSV104_IRQHandler               // 104
    .word   TKEY_IRQHandler                 // 105
    .word   RSV106_IRQHandler               // 106
    .word   RSV107_IRQHandler               // 107
    .word   OTG1_HS_EP_OUT_IRQHandler       // 108
    .word   OTG1_HS_EP_IN_IRQHandler        // 109
    .word   OTG2_HS_EP_OUT_IRQHandler       // 110
    .word   OTG2_HS_EP_IN_IRQHandler        // 111
    .word   RSV112_IRQHandler               // 112
    .word   RSV113_IRQHandler               // 113
    .word   RSV114_IRQHandler               // 114
    .word   RSV115_IRQHandler               // 115
    .word   RSV116_IRQHandler               // 116
    .word   RSV117_IRQHandler               // 117
    .word   RSV118_IRQHandler               // 118
    .word   RSV119_IRQHandler               // 119
    .word   RSV120_IRQHandler               // 120
    .word   RSV121_IRQHandler               // 121
    .word   RSV122_IRQHandler               // 122
    .word   RSV123_IRQHandler               // 123
    .word   RSV124_IRQHandler               // 124
    .word   RSV125_IRQHandler               // 125
    .word   NAND_IRQHandler                 // 126
    .word   BCH_IRQHandler                  // 127
    .word   SDRAM_IRQHandler                // 128
    .word   DMA1_CH0_IRQHandler             // 129
    .word   DMA1_CH1_IRQHandler             // 130
    .word   DMA1_CH2_IRQHandler             // 131
    .word   DMA1_CH3_IRQHandler             // 132
    .word   DMA1_CH4_IRQHandler             // 133
    .word   DMA1_CH5_IRQHandler             // 134
    .word   DMA1_CH6_IRQHandler             // 135
    .word   DMA1_CH7_IRQHandler             // 136
    .word   DMA2_CH0_IRQHandler             // 137
    .word   DMA2_CH1_IRQHandler             // 138
    .word   DMA2_CH2_IRQHandler             // 139
    .word   DMA2_CH3_IRQHandler             // 140
    .word   DMA2_CH4_IRQHandler             // 141
    .word   DMA2_CH5_IRQHandler             // 142
    .word   DMA2_CH6_IRQHandler             // 143
    .word   DMA2_CH7_IRQHandler             // 144
    .word   SRAM1_SEC_IRQHandler            // 145
    .word   SRAM1_DED_IRQHandler            // 146
    .word   SRAM3_SEC_IRQHandler            // 147
    .word   SRAM3_DED_IRQHandler            // 148
    .word   BKPSRAM_SEC_IRQHandler          // 149
    .word   BKPSRAM_DED_IRQHandler          // 150

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

    .weak   WDT_IRQHandler                  // 0  
    .weak   LVD_IRQHandler                  // 1  
    .weak   RTC_XTLSD_IRQHandler            // 2  
    .weak   RSV3_IRQHandler                 // 3  
    .weak   RSV4_IRQHandler                 // 4  
    .weak   RSV5_IRQHandler                 // 5  
    .weak   CLKRDY_IRQHandler               // 6  
    .weak   EXTI0_IRQHandler                // 7  
    .weak   EXTI1_IRQHandler                // 8  
    .weak   EXTI2_IRQHandler                // 9  
    .weak   EXTI3_IRQHandler                // 10 
    .weak   EXTI4_IRQHandler                // 11 
    .weak   RSV12_IRQHandler                // 12 
    .weak   RSV13_IRQHandler                // 13 
    .weak   ADC12_IRQHandler                // 14 
    .weak   ADC3_IRQHandler                 // 15 
    .weak   DAC1_IRQHandler                 // 16 
    .weak   COMP1_IRQHandler                // 17 
    .weak   USBOTG1_IRQHandler              // 18 
    .weak   FDCAN1_IRQHandler               // 19 
    .weak   FDCAN2_IRQHandler               // 20 
    .weak   EXTI9_5_IRQHandler              // 21 
    .weak   TIM1_BRK_UP_TRG_COM_IRQHandler  // 22 
    .weak   TIM1_CC_IRQHandler              // 23 
    .weak   TIM2_IRQHandler                 // 24 
    .weak   TIM3_IRQHandler                 // 25 
    .weak   TIM6_IRQHandler                 // 26 
    .weak   TIM7_IRQHandler                 // 27 
    .weak   TIM8_BRK_UP_TRG_COM_IRQHandler  // 28 
    .weak   TIM8_CC_IRQHandler              // 29 
    .weak   TIM15_IRQHandler                // 30 
    .weak   TIM16_IRQHandler                // 31 
    .weak   TIM17_IRQHandler                // 32 
    .weak   I2C1_IRQHandler                 // 33 
    .weak   I2C2_IRQHandler                 // 34 
    .weak   SPI1_IRQHandler                 // 35 
    .weak   SPI2_IRQHandler                 // 36:
    .weak   SPI3_IRQHandler                 // 37 
    .weak   I2S1_IRQHandler                 // 38 
    .weak   I2S2_IRQHandler                 // 39 
    .weak   USART1_IRQHandler               // 40 
    .weak   USART2_IRQHandler               // 41 
    .weak   USART3_IRQHandler               // 42 
    .weak   USART4_IRQHandler               // 43 
    .weak   EXTI15_10_IRQHandler            // 44 
    .weak   USBOTG1_WKUP_IRQHandler         // 45 
    .weak   LPUART1_IRQHandler              // 46 
    .weak   LPTIM1_IRQHandler               // 47 
    .weak   USBOTG2_WKUP_IRQHandler         // 48 
    .weak   AES_IRQHandler                  // 49 
    .weak   FPU_IRQHandler                  // 50 
    .weak   USBOTG2_IRQHandler              // 51 
    .weak   DCMI_IRQHandler                 // 52 
    .weak   TIM4_IRQHandler                 // 53 
    .weak   RSV54_IRQHandler                // 54 
    .weak   IWDT_WKUP_IRQHandler            // 55 
    .weak   LTDC_IRQHandler                 // 56 
    .weak   LTDC_ERR_IRQHandler             // 57 
    .weak   DMA2D_IRQHandler                // 58 
    .weak   LPTIM2_IRQHandler               // 59 
    .weak   LPTIM3_IRQHandler               // 60 
    .weak   LPTIM4_IRQHandler               // 61 
    .weak   LPTIM5_IRQHandler               // 62 
    .weak   LPTIM6_IRQHandler               // 63 
    .weak   AES_SPI1_IRQHandler             // 64 
    .weak   I2S3_IRQHandler                 // 65 
    .weak   SPI4_IRQHandler                 // 66 
    .weak   SPI5_IRQHandler                 // 67 
    .weak   SPI6_IRQHandler                 // 68 
    .weak   I2C3_IRQHandler                 // 69 
    .weak   I2C4_IRQHandler                 // 70 
    .weak   FDCAN3_IRQHandler               // 71 
    .weak   RSV72_IRQHandler                // 72 
    .weak   ETH_IRQHandler                  // 73 
    .weak   ETH_WKUP_IRQHandler             // 74 
    .weak   SDMMC_IRQHandler                // 75 
    .weak   USART5_IRQHandler               // 76 
    .weak   USART6_IRQHandler               // 77 
    .weak   USART7_IRQHandler               // 78 
    .weak   USART8_IRQHandler               // 79 
    .weak   USART9_IRQHandler               // 80 
    .weak   USART10_IRQHandler              // 81 
    .weak   DAC2_IRQHandler                 // 82 
    .weak   TIM5_IRQHandler                 // 83 
    .weak   TIM9_IRQHandler                 // 84 
    .weak   TIM10_IRQHandler                // 85 
    .weak   TIM11_IRQHandler                // 86 
    .weak   TIM12_IRQHandler                // 87 
    .weak   TIM13_IRQHandler                // 88 
    .weak   TIM14_IRQHandler                // 89 
    .weak   TIM18_IRQHandler                // 90 
    .weak   TIM19_IRQHandler                // 91 
    .weak   TIM20_BRK_UP_TRG_COM_IRQHandler // 92 
    .weak   TIM20_CC_IRQHandler             // 93 
    .weak   TIM21_IRQHandler                // 94 
    .weak   TIM22_IRQHandler                // 95 
    .weak   TIM23_IRQHandler                // 96 
    .weak   TIM24_IRQHandler                // 97 
    .weak   TIM25_IRQHandler                // 98 
    .weak   TIM26_IRQHandler                // 99 
    .weak   SPI7_IRQHandler                 // 100
    .weak   SPI8_IRQHandler                 // 101
    .weak   OSPI1_IRQHandler                // 102
    .weak   OSPI2_IRQHandler                // 103
    .weak   RSV104_IRQHandler               // 104
    .weak   TKEY_IRQHandler                 // 105
    .weak   RSV106_IRQHandler               // 106
    .weak   RSV107_IRQHandler               // 107
    .weak   OTG1_HS_EP_OUT_IRQHandler       // 108
    .weak   OTG1_HS_EP_IN_IRQHandler        // 109
    .weak   OTG2_HS_EP_OUT_IRQHandler       // 110
    .weak   OTG2_HS_EP_IN_IRQHandler        // 111
    .weak   RSV112_IRQHandler               // 112
    .weak   RSV113_IRQHandler               // 113
    .weak   RSV114_IRQHandler               // 114
    .weak   RSV115_IRQHandler               // 115
    .weak   RSV116_IRQHandler               // 116
    .weak   RSV117_IRQHandler               // 117
    .weak   RSV118_IRQHandler               // 118
    .weak   RSV119_IRQHandler               // 119
    .weak   RSV120_IRQHandler               // 120
    .weak   RSV121_IRQHandler               // 121
    .weak   RSV122_IRQHandler               // 122
    .weak   RSV123_IRQHandler               // 123
    .weak   RSV124_IRQHandler               // 124
    .weak   RSV125_IRQHandler               // 125
    .weak   NAND_IRQHandler                 // 126
    .weak   BCH_IRQHandler                  // 127
    .weak   SDRAM_IRQHandler                // 128
    .weak   DMA1_CH0_IRQHandler             // 129
    .weak   DMA1_CH1_IRQHandler             // 130
    .weak   DMA1_CH2_IRQHandler             // 131
    .weak   DMA1_CH3_IRQHandler             // 132
    .weak   DMA1_CH4_IRQHandler             // 133
    .weak   DMA1_CH5_IRQHandler             // 134
    .weak   DMA1_CH6_IRQHandler             // 135
    .weak   DMA1_CH7_IRQHandler             // 136
    .weak   DMA2_CH0_IRQHandler             // 137
    .weak   DMA2_CH1_IRQHandler             // 138
    .weak   DMA2_CH2_IRQHandler             // 139
    .weak   DMA2_CH3_IRQHandler             // 140
    .weak   DMA2_CH4_IRQHandler             // 141
    .weak   DMA2_CH5_IRQHandler             // 142
    .weak   DMA2_CH6_IRQHandler             // 143
    .weak   DMA2_CH7_IRQHandler             // 144
    .weak   SRAM1_SEC_IRQHandler            // 145
    .weak   SRAM1_DED_IRQHandler            // 146
    .weak   SRAM3_SEC_IRQHandler            // 147
    .weak   SRAM3_DED_IRQHandler            // 148
    .weak   BKPSRAM_SEC_IRQHandler          // 149
    .weak   BKPSRAM_DED_IRQHandler          // 150

    .thumb_set   WDT_IRQHandler                  ,Default_Handler    // 0 
    .thumb_set   LVD_IRQHandler                  ,Default_Handler    // 1 
    .thumb_set   RTC_XTLSD_IRQHandler            ,Default_Handler    // 2
    .thumb_set   RSV3_IRQHandler                 ,Default_Handler    // 3 
    .thumb_set   RSV4_IRQHandler                 ,Default_Handler    // 4
    .thumb_set   RSV5_IRQHandler                 ,Default_Handler    // 5
    .thumb_set   CLKRDY_IRQHandler               ,Default_Handler    // 6
    .thumb_set   EXTI0_IRQHandler                ,Default_Handler    // 7
    .thumb_set   EXTI1_IRQHandler                ,Default_Handler    // 8
    .thumb_set   EXTI2_IRQHandler                ,Default_Handler    // 9
    .thumb_set   EXTI3_IRQHandler                ,Default_Handler    // 10
    .thumb_set   EXTI4_IRQHandler                ,Default_Handler    // 11
    .thumb_set   RSV12_IRQHandler                ,Default_Handler    // 12
    .thumb_set   RSV13_IRQHandler                ,Default_Handler    // 13
    .thumb_set   ADC12_IRQHandler                ,Default_Handler    // 14
    .thumb_set   ADC3_IRQHandler                 ,Default_Handler    // 15
    .thumb_set   DAC1_IRQHandler                 ,Default_Handler    // 16
    .thumb_set   COMP1_IRQHandler                ,Default_Handler    // 17
    .thumb_set   USBOTG1_IRQHandler              ,Default_Handler    // 18
    .thumb_set   FDCAN1_IRQHandler               ,Default_Handler    // 19    
    .thumb_set   FDCAN2_IRQHandler               ,Default_Handler    // 20     
    .thumb_set   EXTI9_5_IRQHandler              ,Default_Handler    // 21
    .thumb_set   TIM1_BRK_UP_TRG_COM_IRQHandler  ,Default_Handler    // 22
    .thumb_set   TIM1_CC_IRQHandler              ,Default_Handler    // 23
    .thumb_set   TIM2_IRQHandler                 ,Default_Handler    // 24
    .thumb_set   TIM3_IRQHandler                 ,Default_Handler    // 25
    .thumb_set   TIM6_IRQHandler                 ,Default_Handler    // 26
    .thumb_set   TIM7_IRQHandler                 ,Default_Handler    // 27
    .thumb_set   TIM8_BRK_UP_TRG_COM_IRQHandler  ,Default_Handler    // 28    
    .thumb_set   TIM8_CC_IRQHandler              ,Default_Handler    // 29    
    .thumb_set   TIM15_IRQHandler                ,Default_Handler    // 30
    .thumb_set   TIM16_IRQHandler                ,Default_Handler    // 31    
    .thumb_set   TIM17_IRQHandler                ,Default_Handler    // 32
    .thumb_set   I2C1_IRQHandler                 ,Default_Handler    // 33
    .thumb_set   I2C2_IRQHandler                 ,Default_Handler    // 34                   
    .thumb_set   SPI1_IRQHandler                 ,Default_Handler    // 35                     
    .thumb_set   SPI2_IRQHandler                 ,Default_Handler    // 36               
    .thumb_set   SPI3_IRQHandler                 ,Default_Handler    // 37
    .thumb_set   I2S1_IRQHandler                 ,Default_Handler    // 38
    .thumb_set   I2S2_IRQHandler                 ,Default_Handler    // 39
    .thumb_set   USART1_IRQHandler               ,Default_Handler    // 40                    
    .thumb_set   USART2_IRQHandler               ,Default_Handler    // 41
    .thumb_set   USART3_IRQHandler               ,Default_Handler    // 42                    
    .thumb_set   USART4_IRQHandler               ,Default_Handler    // 43
    .thumb_set   EXTI15_10_IRQHandler            ,Default_Handler    // 44  
    .thumb_set   USBOTG1_WKUP_IRQHandler         ,Default_Handler    // 45
    .thumb_set   LPUART1_IRQHandler              ,Default_Handler    // 46                    
    .thumb_set   LPTIM1_IRQHandler               ,Default_Handler    // 47
    .thumb_set   USBOTG2_WKUP_IRQHandler         ,Default_Handler    // 48
    .thumb_set   AES_IRQHandler                  ,Default_Handler    // 49
    .thumb_set   FPU_IRQHandler                  ,Default_Handler    // 50
    .thumb_set   USBOTG2_IRQHandler              ,Default_Handler    // 51
    .thumb_set   DCMI_IRQHandler                 ,Default_Handler    // 52
    .thumb_set   TIM4_IRQHandler                 ,Default_Handler    // 53
    .thumb_set   RSV54_IRQHandler                ,Default_Handler    // 54
    .thumb_set   IWDT_WKUP_IRQHandler            ,Default_Handler    // 55
    .thumb_set   LTDC_IRQHandler                 ,Default_Handler    // 56
    .thumb_set   LTDC_ERR_IRQHandler             ,Default_Handler    // 57
    .thumb_set   DMA2D_IRQHandler                ,Default_Handler    // 58
    .thumb_set   LPTIM2_IRQHandler               ,Default_Handler    // 59
    .thumb_set   LPTIM3_IRQHandler               ,Default_Handler    // 60
    .thumb_set   LPTIM4_IRQHandler               ,Default_Handler    // 61
    .thumb_set   LPTIM5_IRQHandler               ,Default_Handler    // 62
    .thumb_set   LPTIM6_IRQHandler               ,Default_Handler    // 63
    .thumb_set   AES_SPI1_IRQHandler             ,Default_Handler    // 64
    .thumb_set   I2S3_IRQHandler                 ,Default_Handler    // 65
    .thumb_set   SPI4_IRQHandler                 ,Default_Handler    // 66
    .thumb_set   SPI5_IRQHandler                 ,Default_Handler    // 67
    .thumb_set   SPI6_IRQHandler                 ,Default_Handler    // 68
    .thumb_set   I2C3_IRQHandler                 ,Default_Handler    // 69
    .thumb_set   I2C4_IRQHandler                 ,Default_Handler    // 70
    .thumb_set   FDCAN3_IRQHandler               ,Default_Handler    // 71
    .thumb_set   RSV72_IRQHandler                ,Default_Handler    // 72
    .thumb_set   ETH_IRQHandler                  ,Default_Handler    // 73
    .thumb_set   ETH_WKUP_IRQHandler             ,Default_Handler    // 74
    .thumb_set   SDMMC_IRQHandler                ,Default_Handler    // 75
    .thumb_set   USART5_IRQHandler               ,Default_Handler    // 76
    .thumb_set   USART6_IRQHandler               ,Default_Handler    // 77
    .thumb_set   USART7_IRQHandler               ,Default_Handler    // 78
    .thumb_set   USART8_IRQHandler               ,Default_Handler    // 79
    .thumb_set   USART9_IRQHandler               ,Default_Handler    // 80
    .thumb_set   USART10_IRQHandler              ,Default_Handler    // 81
    .thumb_set   DAC2_IRQHandler                 ,Default_Handler    // 82
    .thumb_set   TIM5_IRQHandler                 ,Default_Handler    // 83
    .thumb_set   TIM9_IRQHandler                 ,Default_Handler    // 84
    .thumb_set   TIM10_IRQHandler                ,Default_Handler    // 85
    .thumb_set   TIM11_IRQHandler                ,Default_Handler    // 86
    .thumb_set   TIM12_IRQHandler                ,Default_Handler    // 87
    .thumb_set   TIM13_IRQHandler                ,Default_Handler    // 88
    .thumb_set   TIM14_IRQHandler                ,Default_Handler    // 89
    .thumb_set   TIM18_IRQHandler                ,Default_Handler    // 90
    .thumb_set   TIM19_IRQHandler                ,Default_Handler    // 91
    .thumb_set   TIM20_BRK_UP_TRG_COM_IRQHandler ,Default_Handler    // 92
    .thumb_set   TIM20_CC_IRQHandler             ,Default_Handler    // 93
    .thumb_set   TIM21_IRQHandler                ,Default_Handler    // 94
    .thumb_set   TIM22_IRQHandler                ,Default_Handler    // 95
    .thumb_set   TIM23_IRQHandler                ,Default_Handler    // 96
    .thumb_set   TIM24_IRQHandler                ,Default_Handler    // 97
    .thumb_set   TIM25_IRQHandler                ,Default_Handler    // 98
    .thumb_set   TIM26_IRQHandler                ,Default_Handler    // 99
    .thumb_set   SPI7_IRQHandler                 ,Default_Handler    // 100
    .thumb_set   SPI8_IRQHandler                 ,Default_Handler    // 101
    .thumb_set   OSPI1_IRQHandler                ,Default_Handler    // 102
    .thumb_set   OSPI2_IRQHandler                ,Default_Handler    // 103
    .thumb_set   RSV104_IRQHandler               ,Default_Handler    // 104
    .thumb_set   TKEY_IRQHandler                 ,Default_Handler    // 105
    .thumb_set   RSV106_IRQHandler               ,Default_Handler    // 106
    .thumb_set   RSV107_IRQHandler               ,Default_Handler    // 107
    .thumb_set   OTG1_HS_EP_OUT_IRQHandler       ,Default_Handler    // 108
    .thumb_set   OTG1_HS_EP_IN_IRQHandler        ,Default_Handler    // 109
    .thumb_set   OTG2_HS_EP_OUT_IRQHandler       ,Default_Handler    // 110
    .thumb_set   OTG2_HS_EP_IN_IRQHandler        ,Default_Handler    // 111
    .thumb_set   RSV112_IRQHandler               ,Default_Handler    // 112
    .thumb_set   RSV113_IRQHandler               ,Default_Handler    // 113
    .thumb_set   RSV114_IRQHandler               ,Default_Handler    // 114
    .thumb_set   RSV115_IRQHandler               ,Default_Handler    // 115
    .thumb_set   RSV116_IRQHandler               ,Default_Handler    // 116
    .thumb_set   RSV117_IRQHandler               ,Default_Handler    // 117
    .thumb_set   RSV118_IRQHandler               ,Default_Handler    // 118
    .thumb_set   RSV119_IRQHandler               ,Default_Handler    // 119
    .thumb_set   RSV120_IRQHandler               ,Default_Handler    // 120
    .thumb_set   RSV121_IRQHandler               ,Default_Handler    // 121
    .thumb_set   RSV122_IRQHandler               ,Default_Handler    // 122
    .thumb_set   RSV123_IRQHandler               ,Default_Handler    // 123
    .thumb_set   RSV124_IRQHandler               ,Default_Handler    // 124
    .thumb_set   RSV125_IRQHandler               ,Default_Handler    // 125
    .thumb_set   NAND_IRQHandler                 ,Default_Handler    // 126
    .thumb_set   BCH_IRQHandler                  ,Default_Handler    // 127
    .thumb_set   SDRAM_IRQHandler                ,Default_Handler    // 128
    .thumb_set   DMA1_CH0_IRQHandler             ,Default_Handler    // 129
    .thumb_set   DMA1_CH1_IRQHandler             ,Default_Handler    // 130
    .thumb_set   DMA1_CH2_IRQHandler             ,Default_Handler    // 131
    .thumb_set   DMA1_CH3_IRQHandler             ,Default_Handler    // 132
    .thumb_set   DMA1_CH4_IRQHandler             ,Default_Handler    // 133
    .thumb_set   DMA1_CH5_IRQHandler             ,Default_Handler    // 134
    .thumb_set   DMA1_CH6_IRQHandler             ,Default_Handler    // 135
    .thumb_set   DMA1_CH7_IRQHandler             ,Default_Handler    // 136
    .thumb_set   DMA2_CH0_IRQHandler             ,Default_Handler    // 137
    .thumb_set   DMA2_CH1_IRQHandler             ,Default_Handler    // 138
    .thumb_set   DMA2_CH2_IRQHandler             ,Default_Handler    // 139
    .thumb_set   DMA2_CH3_IRQHandler             ,Default_Handler    // 140
    .thumb_set   DMA2_CH4_IRQHandler             ,Default_Handler    // 141
    .thumb_set   DMA2_CH5_IRQHandler             ,Default_Handler    // 142
    .thumb_set   DMA2_CH6_IRQHandler             ,Default_Handler    // 143
    .thumb_set   DMA2_CH7_IRQHandler             ,Default_Handler    // 144
    .thumb_set   SRAM1_SEC_IRQHandler            ,Default_Handler    // 145
    .thumb_set   SRAM1_DED_IRQHandler            ,Default_Handler    // 146
    .thumb_set   SRAM3_SEC_IRQHandler            ,Default_Handler    // 147
    .thumb_set   SRAM3_DED_IRQHandler            ,Default_Handler    // 148
    .thumb_set   BKPSRAM_SEC_IRQHandler          ,Default_Handler    // 149
    .thumb_set   BKPSRAM_DED_IRQHandler          ,Default_Handler    // 150


/************************ (C) COPYRIGHT aisinochip *****END OF FILE****/

