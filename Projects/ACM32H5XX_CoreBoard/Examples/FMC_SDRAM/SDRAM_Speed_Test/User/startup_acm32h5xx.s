;/*****************************************************************************
; * @file:    startup_CM0.s
; * @purpose: CMSIS Cortex-M0 Core Device Startup File 
; *           for the ARM 'Microcontroller Prototyping System' 
; * @version: V1.0
; * @date:    
; *
; *****************************************************************************/
Stack_Size      EQU     0x00010000
Heap_Size       EQU     0x00004000
;__initial_sp    EQU    0x20003000       ;Define SP size

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT __Vectors

__Vectors       
				DCD     __initial_sp                    ; Top of Stack
                DCD     Reset_Handler                   ; Reset Handler
                DCD     NMI_Handler                     ; NMI Handler
                DCD     HardFault_Handler               ; Hard Fault Handler
                DCD     MemManage_Handler               ; MPU Fault Handler
                DCD     BusFault_Handler                ; Bus Fault Handler
                DCD     UsageFault_Handler              ; Usage Fault Handler
                DCD     0                               ; Reserved
                DCD     0                               ; Reserved
                DCD     0                               ; Reserved
                DCD     0                               ; Reserved
                DCD     SVC_Handler                     ; SVCall Handler,SWI
                DCD     DebugMon_Handler                ; Debug Monitor Handler
                DCD     0                               ; Reserved
                DCD     PendSV_Handler                  ; PendSV Handler
                DCD     SysTick_Handler                 ; SysTick Handler

                ; External Interrupts
                DCD     WDT_IRQHandler				    ; 0 
				DCD     LVD_IRQHandler				    ; 1				
                DCD     RTC_XTLSD_IRQHandler            ; 2
				DCD     RSV3_IRQHandler		            ; 3					
				DCD     RSV4_IRQHandler	                ; 4
				DCD     RSV5_IRQHandler                 ; 5
				DCD     CLKRDY_IRQHandler               ; 6
				DCD     EXTI0_IRQHandler		        ; 7
				DCD     EXTI1_IRQHandler		        ; 8
				DCD     EXTI2_IRQHandler		        ; 9
				DCD     EXTI3_IRQHandler		        ; 10
				DCD     EXTI4_IRQHandler		        ; 11
				DCD     RSV12_IRQHandler		        ; 12                   
				DCD     RSV13_IRQHandler		        ; 13
				DCD     ADC12_IRQHandler		        ; 14
				DCD     ADC3_IRQHandler		            ; 15
				DCD     DAC1_IRQHandler		            ; 16
				DCD     COMP1_IRQHandler                ; 17
				DCD     USBOTG1_IRQHandler		        ; 18                      
				DCD     FDCAN1_IRQHandler               ; 19	
				DCD     FDCAN2_IRQHandler               ; 20 	
				DCD     EXTI9_5_IRQHandler              ; 21
				DCD     TIM1_BRK_UP_TRG_COM_IRQHandler  ; 22
				DCD     TIM1_CC_IRQHandler              ; 23
				DCD     TIM2_IRQHandler	                ; 24
				DCD     TIM3_IRQHandler                 ; 25
				DCD     TIM6_IRQHandler	                ; 26
				DCD    	TIM7_IRQHandler		            ; 27
				DCD    	TIM8_BRK_UP_TRG_COM_IRQHandler  ; 28	
				DCD     TIM8_CC_IRQHandler              ; 29	
				DCD     TIM15_IRQHandler                ; 30
				DCD		TIM16_IRQHandler		        ; 31	
				DCD     TIM17_IRQHandler                ; 32
				DCD     I2C1_IRQHandler                 ; 33
                DCD     I2C2_IRQHandler                 ; 34                   
                DCD     SPI1_IRQHandler                 ; 35                     
                DCD     SPI2_IRQHandler	                ; 36:               
				DCD     SPI3_IRQHandler                 ; 37
				DCD     I2S1_IRQHandler	                ; 38
                DCD     I2S2_IRQHandler	                ; 39
                DCD     USART1_IRQHandler               ; 40                    
				DCD     USART2_IRQHandler		    	; 41
				DCD     USART3_IRQHandler		  	    ; 42                    
				DCD     USART4_IRQHandler		    	; 43
				DCD     EXTI15_10_IRQHandler	    	; 44  
				DCD     USBOTG1_WKUP_IRQHandler         ; 45
                DCD     LPUART1_IRQHandler              ; 46                    
                DCD     LPTIM1_IRQHandler               ; 47
				DCD     USBOTG2_WKUP_IRQHandler         ; 48
				DCD     AES_IRQHandler                  ; 49
				DCD     FPU_IRQHandler                  ; 50
				DCD     USBOTG2_IRQHandler              ; 51
				DCD     DCMI_IRQHandler                 ; 52
				DCD     TIM4_IRQHandler                 ; 53
				DCD     RSV54_IRQHandler                ; 54
				DCD     IWDT_WKUP_IRQHandler            ; 55
				DCD     LTDC_IRQHandler                 ; 56
				DCD     LTDC_ERR_IRQHandler             ; 57
				DCD     DMA2D_IRQHandler                ; 58
				DCD     LPTIM2_IRQHandler               ; 59
				DCD     LPTIM3_IRQHandler               ; 60
				DCD     LPTIM4_IRQHandler               ; 61
				DCD     LPTIM5_IRQHandler               ; 62
				DCD     LPTIM6_IRQHandler               ; 63
				DCD     AES_SPI1_IRQHandler             ; 64
				DCD     I2S3_IRQHandler                 ; 65
				DCD     SPI4_IRQHandler                 ; 66
				DCD     SPI5_IRQHandler                 ; 67
				DCD     SPI6_IRQHandler                 ; 68
				DCD     I2C3_IRQHandler                 ; 69
				DCD     I2C4_IRQHandler                 ; 70
				DCD     FDCAN3_IRQHandler               ; 71
				DCD     RSV72_IRQHandler                ; 72
				DCD     ETH_IRQHandler                  ; 73
				DCD     ETH_WKUP_IRQHandler             ; 74
				DCD     SDMMC_IRQHandler                ; 75
				DCD     USART5_IRQHandler               ; 76
				DCD     USART6_IRQHandler               ; 77
				DCD     USART7_IRQHandler               ; 78
				DCD     USART8_IRQHandler               ; 79
				DCD     USART9_IRQHandler               ; 80
				DCD     USART10_IRQHandler              ; 81
				DCD     DAC2_IRQHandler                 ; 82
				DCD     TIM5_IRQHandler                 ; 83
				DCD     TIM9_IRQHandler                 ; 84
				DCD     TIM10_IRQHandler                ; 85
				DCD     TIM11_IRQHandler                ; 86
				DCD     TIM12_IRQHandler                ; 87
				DCD     TIM13_IRQHandler                ; 88
				DCD     TIM14_IRQHandler                ; 89
				DCD     TIM18_IRQHandler                ; 90
				DCD     TIM19_IRQHandler                ; 91
				DCD     TIM20_BRK_UP_TRG_COM_IRQHandler ; 92
				DCD     TIM20_CC_IRQHandler             ; 93
				DCD     TIM21_IRQHandler                ; 94
				DCD     TIM22_IRQHandler                ; 95
				DCD     TIM23_IRQHandler                ; 96
				DCD     TIM24_IRQHandler                ; 97
				DCD     TIM25_IRQHandler                ; 98
				DCD     TIM26_IRQHandler                ; 99
				DCD     SPI7_IRQHandler                 ; 100
				DCD     SPI8_IRQHandler                 ; 101
				DCD     OSPI1_IRQHandler                ; 102
				DCD     OSPI2_IRQHandler                ; 103
				DCD     RSV104_IRQHandler               ; 104
				DCD     TKEY_IRQHandler                 ; 105
				DCD     LPT_IRQHandler                  ; 106
				DCD     RSV107_IRQHandler               ; 107
				DCD     OTG1_HS_EP_OUT_IRQHandler       ; 108
				DCD     OTG1_HS_EP_IN_IRQHandler        ; 109
				DCD     OTG2_HS_EP_OUT_IRQHandler       ; 110
				DCD     OTG2_HS_EP_IN_IRQHandler        ; 111
				DCD     NDL_IRQHandler                  ; 112
				DCD     THM_IRQHandler                  ; 113
				DCD     STM1_PWM_IRQHandler             ; 114
				DCD     STM2_PWM_IRQHandler             ; 115
				DCD     DCM1_PWM_IRQHandler             ; 116
				DCD     DCM2_PWM_IRQHandler             ; 117
				DCD     DCM3_PWM_IRQHandler             ; 118
				DCD     DCM4_PWM_IRQHandler             ; 119
				DCD     STM3_PWM_IRQHandler             ; 120
				DCD     STM4_PWM_IRQHandler             ; 121
				DCD     STM5_PWM_IRQHandler             ; 122
				DCD     STM6_PWM_IRQHandler             ; 123
				DCD     DCM5_PWM_IRQHandler             ; 124
				DCD     DCM6_PWM_IRQHandler             ; 125
				DCD     NAND_IRQHandler                 ; 126
				DCD     BCH_IRQHandler                  ; 127
				DCD     SDRAM_IRQHandler                ; 128
				DCD     DMA1_CH0_IRQHandler             ; 129
				DCD     DMA1_CH1_IRQHandler             ; 130
				DCD     DMA1_CH2_IRQHandler             ; 131
				DCD     DMA1_CH3_IRQHandler             ; 132
				DCD     DMA1_CH4_IRQHandler             ; 133
				DCD     DMA1_CH5_IRQHandler             ; 134
				DCD     DMA1_CH6_IRQHandler             ; 135
				DCD     DMA1_CH7_IRQHandler             ; 136
				DCD     DMA2_CH0_IRQHandler             ; 137
				DCD     DMA2_CH1_IRQHandler             ; 138
				DCD     DMA2_CH2_IRQHandler             ; 139
				DCD     DMA2_CH3_IRQHandler             ; 140
				DCD     DMA2_CH4_IRQHandler             ; 141
				DCD     DMA2_CH5_IRQHandler             ; 142
				DCD     DMA2_CH6_IRQHandler             ; 143
				DCD     DMA2_CH7_IRQHandler             ; 144
				DCD     SRAM1_SEC_IRQHandler            ; 145
				DCD     SRAM1_DED_IRQHandler            ; 146
				DCD     SRAM3_SEC_IRQHandler            ; 147
				DCD     SRAM3_DED_IRQHandler            ; 148
				DCD     BKPSRAM_SEC_IRQHandler          ; 149
				DCD     BKPSRAM_DED_IRQHandler          ; 150
                

                AREA    |.text|, CODE, READONLY

Reset_Handler   PROC
                EXPORT  Reset_Handler                       [WEAK]
                IMPORT  __main
                IMPORT  main
                IMPORT  SystemInit

                ;Enable DTCM     
                MOVW     r0,#0xe014
                MOVT     r0,#0xe001
                LDR      r1,[r0,#0]
                ORR      r1,r1,#1
                STR      r1,[r0,#0] 
                
                LDR     R0, =SystemInit
                BLX     R0
 
                
                LDR     R0, =__main
                BX      R0                  ;
                ENDP


NMI_Handler     PROC
                EXPORT  NMI_Handler                         [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler                   [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler                   [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler                    [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler                  [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                         [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler                    [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler                      [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler                     [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT     WDT_IRQHandler                   [WEAK]
                EXPORT     LVD_IRQHandler                   [WEAK]                
                EXPORT     RTC_XTLSD_IRQHandler             [WEAK]
                EXPORT     RSV3_IRQHandler                  [WEAK]        
                EXPORT     RSV4_IRQHandler                  [WEAK]
                EXPORT     RSV5_IRQHandler                  [WEAK]
                EXPORT     CLKRDY_IRQHandler                [WEAK]
                EXPORT     EXTI0_IRQHandler                 [WEAK]
                EXPORT     EXTI1_IRQHandler                 [WEAK]
                EXPORT     EXTI2_IRQHandler                 [WEAK]
                EXPORT     EXTI3_IRQHandler                 [WEAK]
                EXPORT     EXTI4_IRQHandler                 [WEAK]
                EXPORT     RSV12_IRQHandler                 [WEAK]           
                EXPORT     RSV13_IRQHandler                 [WEAK]
                EXPORT     ADC12_IRQHandler                 [WEAK]
                EXPORT     ADC3_IRQHandler                  [WEAK]
                EXPORT     DAC1_IRQHandler                  [WEAK]
                EXPORT     COMP1_IRQHandler                 [WEAK]
                EXPORT     USBOTG1_IRQHandler               [WEAK]
                EXPORT     FDCAN1_IRQHandler                [WEAK]
                EXPORT     FDCAN2_IRQHandler                [WEAK]
                EXPORT     EXTI9_5_IRQHandler               [WEAK]
                EXPORT     TIM1_BRK_UP_TRG_COM_IRQHandler   [WEAK]
                EXPORT     TIM1_CC_IRQHandler               [WEAK]
                EXPORT     TIM2_IRQHandler                  [WEAK]
                EXPORT     TIM3_IRQHandler                  [WEAK] 
                EXPORT     TIM6_IRQHandler                  [WEAK]
                EXPORT     TIM7_IRQHandler                  [WEAK]
                EXPORT     TIM8_BRK_UP_TRG_COM_IRQHandler   [WEAK]
                EXPORT     TIM8_CC_IRQHandler               [WEAK]
                EXPORT     TIM15_IRQHandler                 [WEAK]
                EXPORT     TIM16_IRQHandler                 [WEAK]
                EXPORT     TIM17_IRQHandler                 [WEAK]
                EXPORT     I2C1_IRQHandler                  [WEAK]
                EXPORT     I2C2_IRQHandler                  [WEAK]
                EXPORT     SPI1_IRQHandler                  [WEAK]
                EXPORT     SPI2_IRQHandler                  [WEAK]
                EXPORT     SPI3_IRQHandler                  [WEAK]          
                EXPORT     I2S1_IRQHandler                  [WEAK]
                EXPORT     I2S2_IRQHandler                  [WEAK]
                EXPORT     USART1_IRQHandler                [WEAK]
                EXPORT     USART2_IRQHandler                [WEAK]
                EXPORT     USART3_IRQHandler                [WEAK]
                EXPORT     USART4_IRQHandler                [WEAK]
                EXPORT     EXTI15_10_IRQHandler             [WEAK]
                EXPORT     USBOTG1_WKUP_IRQHandler          [WEAK]
                EXPORT     LPUART1_IRQHandler               [WEAK]
                EXPORT     LPTIM1_IRQHandler                [WEAK]
                EXPORT     USBOTG2_WKUP_IRQHandler          [WEAK]
                EXPORT     AES_IRQHandler                   [WEAK]
                EXPORT     FPU_IRQHandler                   [WEAK]
                EXPORT     USBOTG2_IRQHandler               [WEAK]
                EXPORT     DCMI_IRQHandler                  [WEAK]
                EXPORT     TIM4_IRQHandler                  [WEAK]
                EXPORT     RSV54_IRQHandler                 [WEAK]
                EXPORT     IWDT_WKUP_IRQHandler             [WEAK]
                EXPORT     LTDC_IRQHandler                  [WEAK]
                EXPORT     LTDC_ERR_IRQHandler              [WEAK]
                EXPORT     DMA2D_IRQHandler                 [WEAK]
                EXPORT     LPTIM2_IRQHandler                [WEAK]
                EXPORT     LPTIM3_IRQHandler                [WEAK]
                EXPORT     LPTIM4_IRQHandler                [WEAK]
                EXPORT     LPTIM5_IRQHandler                [WEAK]
                EXPORT     LPTIM6_IRQHandler                [WEAK]
                EXPORT     AES_SPI1_IRQHandler              [WEAK]
                EXPORT     I2S3_IRQHandler                  [WEAK]
                EXPORT     SPI4_IRQHandler                  [WEAK]
                EXPORT     SPI5_IRQHandler                  [WEAK]
                EXPORT     SPI6_IRQHandler                  [WEAK]
                EXPORT     I2C3_IRQHandler                  [WEAK]
                EXPORT     I2C4_IRQHandler                  [WEAK]
                EXPORT     FDCAN3_IRQHandler                [WEAK]
                EXPORT     RSV72_IRQHandler                 [WEAK]
                EXPORT     ETH_IRQHandler                   [WEAK]
                EXPORT     ETH_WKUP_IRQHandler              [WEAK]
                EXPORT     SDMMC_IRQHandler                 [WEAK]
                EXPORT     USART5_IRQHandler                [WEAK]
                EXPORT     USART6_IRQHandler                [WEAK]
                EXPORT     USART7_IRQHandler                [WEAK]
                EXPORT     USART8_IRQHandler                [WEAK]
                EXPORT     USART9_IRQHandler                [WEAK]
                EXPORT     USART10_IRQHandler               [WEAK]
                EXPORT     DAC2_IRQHandler                  [WEAK]
                EXPORT     TIM5_IRQHandler                  [WEAK]
                EXPORT     TIM9_IRQHandler                  [WEAK]
                EXPORT     TIM10_IRQHandler                 [WEAK]
                EXPORT     TIM11_IRQHandler                 [WEAK]
                EXPORT     TIM12_IRQHandler                 [WEAK]
                EXPORT     TIM13_IRQHandler                 [WEAK]
                EXPORT     TIM14_IRQHandler                 [WEAK]
                EXPORT     TIM18_IRQHandler                 [WEAK]
                EXPORT     TIM19_IRQHandler                 [WEAK]
                EXPORT     TIM20_BRK_UP_TRG_COM_IRQHandler  [WEAK]
                EXPORT     TIM20_CC_IRQHandler              [WEAK]
                EXPORT     TIM21_IRQHandler                 [WEAK]
                EXPORT     TIM22_IRQHandler                 [WEAK]
                EXPORT     TIM23_IRQHandler                 [WEAK]
                EXPORT     TIM24_IRQHandler                 [WEAK]
                EXPORT     TIM25_IRQHandler                 [WEAK]
                EXPORT     TIM26_IRQHandler                 [WEAK]
                EXPORT     SPI7_IRQHandler                  [WEAK]
                EXPORT     SPI8_IRQHandler                  [WEAK]
                EXPORT     OSPI1_IRQHandler                 [WEAK]
                EXPORT     OSPI2_IRQHandler                 [WEAK]
                EXPORT     RSV104_IRQHandler                [WEAK]
                EXPORT     TKEY_IRQHandler                  [WEAK]
                EXPORT     LPT_IRQHandler                   [WEAK]
                EXPORT     RSV107_IRQHandler                [WEAK]
                EXPORT     OTG1_HS_EP_OUT_IRQHandler        [WEAK]
                EXPORT     OTG1_HS_EP_IN_IRQHandler         [WEAK]
                EXPORT     OTG2_HS_EP_OUT_IRQHandler        [WEAK]
                EXPORT     OTG2_HS_EP_IN_IRQHandler         [WEAK]
                EXPORT     NDL_IRQHandler                   [WEAK]
                EXPORT     THM_IRQHandler                   [WEAK]
                EXPORT     STM1_PWM_IRQHandler              [WEAK]
                EXPORT     STM2_PWM_IRQHandler              [WEAK]
                EXPORT     DCM1_PWM_IRQHandler              [WEAK]
                EXPORT     DCM2_PWM_IRQHandler              [WEAK]
                EXPORT     DCM3_PWM_IRQHandler              [WEAK]
                EXPORT     DCM4_PWM_IRQHandler              [WEAK]
                EXPORT     STM3_PWM_IRQHandler              [WEAK]
                EXPORT     STM4_PWM_IRQHandler              [WEAK]
                EXPORT     STM5_PWM_IRQHandler              [WEAK]
                EXPORT     STM6_PWM_IRQHandler              [WEAK]
                EXPORT     DCM5_PWM_IRQHandler              [WEAK]
                EXPORT     DCM6_PWM_IRQHandler              [WEAK]
                EXPORT     NAND_IRQHandler                  [WEAK]
                EXPORT     BCH_IRQHandler                   [WEAK]
                EXPORT     SDRAM_IRQHandler                 [WEAK]
                EXPORT     DMA1_CH0_IRQHandler              [WEAK]
                EXPORT     DMA1_CH1_IRQHandler              [WEAK]
                EXPORT     DMA1_CH2_IRQHandler              [WEAK]
                EXPORT     DMA1_CH3_IRQHandler              [WEAK]
                EXPORT     DMA1_CH4_IRQHandler              [WEAK]
                EXPORT     DMA1_CH5_IRQHandler              [WEAK]
                EXPORT     DMA1_CH6_IRQHandler              [WEAK]
                EXPORT     DMA1_CH7_IRQHandler              [WEAK]
                EXPORT     DMA2_CH0_IRQHandler              [WEAK]
                EXPORT     DMA2_CH1_IRQHandler              [WEAK]
                EXPORT     DMA2_CH2_IRQHandler              [WEAK]
                EXPORT     DMA2_CH3_IRQHandler              [WEAK]
                EXPORT     DMA2_CH4_IRQHandler              [WEAK]
                EXPORT     DMA2_CH5_IRQHandler              [WEAK]
                EXPORT     DMA2_CH6_IRQHandler              [WEAK]
                EXPORT     DMA2_CH7_IRQHandler              [WEAK]
                EXPORT     SRAM1_SEC_IRQHandler             [WEAK]
                EXPORT     SRAM1_DED_IRQHandler             [WEAK]
                EXPORT     SRAM3_SEC_IRQHandler             [WEAK]
                EXPORT     SRAM3_DED_IRQHandler             [WEAK]
                EXPORT     BKPSRAM_SEC_IRQHandler           [WEAK]
                EXPORT     BKPSRAM_DED_IRQHandler           [WEAK]

WDT_IRQHandler
LVD_IRQHandler
RTC_XTLSD_IRQHandler
RSV3_IRQHandler
RSV4_IRQHandler
RSV5_IRQHandler
CLKRDY_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
RSV12_IRQHandler
RSV13_IRQHandler
ADC12_IRQHandler
ADC3_IRQHandler
DAC1_IRQHandler
COMP1_IRQHandler
USBOTG1_IRQHandler
FDCAN1_IRQHandler
FDCAN2_IRQHandler
EXTI9_5_IRQHandler
TIM1_BRK_UP_TRG_COM_IRQHandler
TIM1_CC_IRQHandler
TIM2_IRQHandler
TIM3_IRQHandler
TIM6_IRQHandler
TIM7_IRQHandler
TIM8_BRK_UP_TRG_COM_IRQHandler
TIM8_CC_IRQHandler
TIM15_IRQHandler
TIM16_IRQHandler
TIM17_IRQHandler
I2C1_IRQHandler
I2C2_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
SPI3_IRQHandler
I2S1_IRQHandler
I2S2_IRQHandler
USART1_IRQHandler
USART2_IRQHandler
USART3_IRQHandler
USART4_IRQHandler
EXTI15_10_IRQHandler
USBOTG1_WKUP_IRQHandler
LPUART1_IRQHandler
LPTIM1_IRQHandler
USBOTG2_WKUP_IRQHandler
AES_IRQHandler
FPU_IRQHandler
USBOTG2_IRQHandler
DCMI_IRQHandler
TIM4_IRQHandler
RSV54_IRQHandler
IWDT_WKUP_IRQHandler
LTDC_IRQHandler
LTDC_ERR_IRQHandler
DMA2D_IRQHandler
LPTIM2_IRQHandler
LPTIM3_IRQHandler
LPTIM4_IRQHandler
LPTIM5_IRQHandler
LPTIM6_IRQHandler
AES_SPI1_IRQHandler
I2S3_IRQHandler
SPI4_IRQHandler
SPI5_IRQHandler
SPI6_IRQHandler
I2C3_IRQHandler
I2C4_IRQHandler
FDCAN3_IRQHandler
RSV72_IRQHandler
ETH_IRQHandler
ETH_WKUP_IRQHandler
SDMMC_IRQHandler
USART5_IRQHandler
USART6_IRQHandler
USART7_IRQHandler
USART8_IRQHandler
USART9_IRQHandler
USART10_IRQHandler
DAC2_IRQHandler
TIM5_IRQHandler
TIM9_IRQHandler
TIM10_IRQHandler
TIM11_IRQHandler
TIM12_IRQHandler
TIM13_IRQHandler
TIM14_IRQHandler
TIM18_IRQHandler
TIM19_IRQHandler
TIM20_BRK_UP_TRG_COM_IRQHandler
TIM20_CC_IRQHandler
TIM21_IRQHandler
TIM22_IRQHandler
TIM23_IRQHandler
TIM24_IRQHandler
TIM25_IRQHandler
TIM26_IRQHandler
SPI7_IRQHandler
SPI8_IRQHandler
OSPI1_IRQHandler
OSPI2_IRQHandler
RSV104_IRQHandler
TKEY_IRQHandler
LPT_IRQHandler
RSV107_IRQHandler
OTG1_HS_EP_OUT_IRQHandler
OTG1_HS_EP_IN_IRQHandler
OTG2_HS_EP_OUT_IRQHandler
OTG2_HS_EP_IN_IRQHandler
NDL_IRQHandler
THM_IRQHandler
STM1_PWM_IRQHandler
STM2_PWM_IRQHandler
DCM1_PWM_IRQHandler
DCM2_PWM_IRQHandler
DCM3_PWM_IRQHandler
DCM4_PWM_IRQHandler
STM3_PWM_IRQHandler
STM4_PWM_IRQHandler
STM5_PWM_IRQHandler
STM6_PWM_IRQHandler
DCM5_PWM_IRQHandler
DCM6_PWM_IRQHandler
NAND_IRQHandler
BCH_IRQHandler
SDRAM_IRQHandler
DMA1_CH0_IRQHandler
DMA1_CH1_IRQHandler
DMA1_CH2_IRQHandler
DMA1_CH3_IRQHandler
DMA1_CH4_IRQHandler
DMA1_CH5_IRQHandler
DMA1_CH6_IRQHandler
DMA1_CH7_IRQHandler
DMA2_CH0_IRQHandler
DMA2_CH1_IRQHandler
DMA2_CH2_IRQHandler
DMA2_CH3_IRQHandler
DMA2_CH4_IRQHandler
DMA2_CH5_IRQHandler
DMA2_CH6_IRQHandler
DMA2_CH7_IRQHandler
SRAM1_SEC_IRQHandler
SRAM1_DED_IRQHandler      
SRAM3_SEC_IRQHandler
SRAM3_DED_IRQHandler
BKPSRAM_SEC_IRQHandler
BKPSRAM_DED_IRQHandler


                B        .
                ENDP                                                                  
                 
                ALIGN
                                 
; User Initial Stack & Heap
                 
                IF      :DEF:__MICROLIB
                 
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                 
                ELSE
                 
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap
                 
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

                END
