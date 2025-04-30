
/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_coreboard.h"



#define FMC_BANK_NOR           ((uint32_t)(0x60000000))	//CS1

#define FMC_BANK_SRAM_MUX      ((uint32_t)(0x68000000))    //CS3
		
#define FMC_BANK_PSRAM         ((uint32_t)(0x68000000))    //CS3
	
#define FMC_BANK_LCD           ((uint32_t)(0x68000000))    //CS3

#define FMC_BANK_SRAM          ((uint32_t)(0x64000000))	//CS2



/*A地址信号线*/    
#define FMC_A0_GPIO_PORT               GPIOF
#define FMC_A0_GPIO_PIN                GPIO_PIN_0      //AF11

#define FMC_A1_GPIO_PORT               GPIOF
#define FMC_A1_GPIO_PIN                GPIO_PIN_1      //AF11

#define FMC_A2_GPIO_PORT               GPIOF
#define FMC_A2_GPIO_PIN                GPIO_PIN_2      //AF11

#define FMC_A3_GPIO_PORT               GPIOF
#define FMC_A3_GPIO_PIN                GPIO_PIN_3      //AF11

#define FMC_A4_GPIO_PORT               GPIOF
#define FMC_A4_GPIO_PIN                GPIO_PIN_4      //AF11

#define FMC_A5_GPIO_PORT               GPIOF
#define FMC_A5_GPIO_PIN                GPIO_PIN_5      //AF11

#define FMC_A6_GPIO_PORT               GPIOF
#define FMC_A6_GPIO_PIN                GPIO_PIN_12     //AF11

#define FMC_A7_GPIO_PORT               GPIOF
#define FMC_A7_GPIO_PIN                GPIO_PIN_13     //AF11

#define FMC_A8_GPIO_PORT               GPIOF
#define FMC_A8_GPIO_PIN                GPIO_PIN_14     //AF11
    
#define FMC_A9_GPIO_PORT               GPIOF
#define FMC_A9_GPIO_PIN                GPIO_PIN_15     //AF11

#define FMC_A10_GPIO_PORT              GPIOG
#define FMC_A10_GPIO_PIN               GPIO_PIN_0      //AF11

#define FMC_A11_GPIO_PORT              GPIOG
#define FMC_A11_GPIO_PIN               GPIO_PIN_1      //AF11

#define FMC_A12_GPIO_PORT              GPIOG
#define FMC_A12_GPIO_PIN               GPIO_PIN_2      //AF11

#define FMC_A13_GPIO_PORT              GPIOG
#define FMC_A13_GPIO_PIN               GPIO_PIN_3      //AF11

#define FMC_A14_GPIO_PORT              GPIOG
#define FMC_A14_GPIO_PIN               GPIO_PIN_4      //AF11

#define FMC_A15_GPIO_PORT              GPIOG
#define FMC_A15_GPIO_PIN               GPIO_PIN_5      //AF11

#define FMC_A16_GPIO_PORT              GPIOD
#define FMC_A16_GPIO_PIN               GPIO_PIN_11     //AF10    

#define FMC_A17_GPIO_PORT              GPIOD
#define FMC_A17_GPIO_PIN               GPIO_PIN_12     //AF10 

#define FMC_A18_GPIO_PORT              GPIOD
#define FMC_A18_GPIO_PIN               GPIO_PIN_13     //AF10 

#define FMC_A19_GPIO_PORT              GPIOE
#define FMC_A19_GPIO_PIN               GPIO_PIN_3      //AF10 
        
#define FMC_A20_GPIO_PORT              GPIOE
#define FMC_A20_GPIO_PIN               GPIO_PIN_4      //AF10
        
#define FMC_A21_GPIO_PORT              GPIOE
#define FMC_A21_GPIO_PIN               GPIO_PIN_5      //AF10

#define FMC_A22_GPIO_PORT              GPIOE
#define FMC_A22_GPIO_PIN               GPIO_PIN_6      //AF10

#define FMC_A23_GPIO_PORT              GPIOE
#define FMC_A23_GPIO_PIN               GPIO_PIN_2      //AF10

#define FMC_A24_GPIO_PORT              GPIOG           
#define FMC_A24_GPIO_PIN               GPIO_PIN_13     //AF10



/*D 数据信号线*/
#define FMC_D0_GPIO_PORT               GPIOD
#define FMC_D0_GPIO_PIN                GPIO_PIN_14     //AF10

#define FMC_D1_GPIO_PORT               GPIOD
#define FMC_D1_GPIO_PIN                GPIO_PIN_15     //AF10

#define FMC_D2_GPIO_PORT               GPIOD
#define FMC_D2_GPIO_PIN                GPIO_PIN_0      //AF10

#define FMC_D3_GPIO_PORT               GPIOD
#define FMC_D3_GPIO_PIN                GPIO_PIN_1      //AF10

#define FMC_D4_GPIO_PORT               GPIOE
#define FMC_D4_GPIO_PIN                GPIO_PIN_7      //AF10

#define FMC_D5_GPIO_PORT               GPIOE
#define FMC_D5_GPIO_PIN                GPIO_PIN_8      //AF10

#define FMC_D6_GPIO_PORT               GPIOE
#define FMC_D6_GPIO_PIN                GPIO_PIN_9      //AF10

#define FMC_D7_GPIO_PORT               GPIOE
#define FMC_D7_GPIO_PIN                GPIO_PIN_10     //AF10

#define FMC_D8_GPIO_PORT               GPIOE
#define FMC_D8_GPIO_PIN                GPIO_PIN_11     //AF10

#define FMC_D9_GPIO_PORT               GPIOE
#define FMC_D9_GPIO_PIN                GPIO_PIN_12     //AF10

#define FMC_D10_GPIO_PORT              GPIOE
#define FMC_D10_GPIO_PIN               GPIO_PIN_13     //AF10

#define FMC_D11_GPIO_PORT              GPIOE
#define FMC_D11_GPIO_PIN               GPIO_PIN_14     //AF10

#define FMC_D12_GPIO_PORT              GPIOE
#define FMC_D12_GPIO_PIN               GPIO_PIN_15     //AF10

#define FMC_D13_GPIO_PORT              GPIOD
#define FMC_D13_GPIO_PIN               GPIO_PIN_8      //AF10

#define FMC_D14_GPIO_PORT              GPIOD
#define FMC_D14_GPIO_PIN               GPIO_PIN_9      //AF10

#define FMC_D15_GPIO_PORT              GPIOD
#define FMC_D15_GPIO_PIN               GPIO_PIN_10     //AF10



/*控制信号线*/  
/*CS片选*/
/*FMC_NE1 ,对应的基地址0x60000000*/
#define FMC_NORFLASH_CS_GPIO_PORT      GPIOD
#define FMC_NORFLASH_CS_GPIO_PIN       GPIO_PIN_7      //AF10

/*FMC_NE2 ,对应的基地址0x64000000*/
#define FMC_SRAM_CS_GPIO_PORT          GPIOG
#define FMC_SRAM_CS_GPIO_PIN           GPIO_PIN_9      //AF11

/*FMC_NE3 ,对应的基地址0x68000000*/
#define FMC_LCD_CS_GPIO_PORT           GPIOG           //AF11
#define FMC_LCD_CS_GPIO_PIN            GPIO_PIN_6

/*FMC_NE3 ,对应的基地址0x68000000*/
#define FMC_PSRAM_CS_GPIO_PORT         GPIOG
#define FMC_PSRAM_CS_GPIO_PIN          GPIO_PIN_6      //AF11


/*FMC_NWE写使能*/
#define FMC_WE_GPIO_PORT               GPIOD
#define FMC_WE_GPIO_PIN                GPIO_PIN_5      //AF10      

/*FMC_NOE读使能*/
#define FMC_OE_GPIO_PORT               GPIOD           //AF10
#define FMC_OE_GPIO_PIN                GPIO_PIN_4


/*FMC_NBL1 UB数据掩码*/
#define FMC_UDQM_GPIO_PORT             GPIOE            
#define FMC_UDQM_GPIO_PIN              GPIO_PIN_1       //AF10

/*FMC_NBL0 LB数据掩码*/
#define FMC_LDQM_GPIO_PORT             GPIOE
#define FMC_LDQM_GPIO_PIN              GPIO_PIN_0       //AF10

/*NADV*/
#define FMC_NADV_GPIO_PORT             GPIOB
#define FMC_NADV_GPIO_PIN              GPIO_PIN_7

/*FSMC_CLK  PD3*/
#define FMC_CLK_GPIO_PORT              GPIOD
#define FMC_CLK_GPIO_PIN               GPIO_PIN_3

/*FSMC_NWATI  PB7*/
#define FMC_NWATI_GPIO_PORT            GPIOD
#define FMC_NWATI_GPIO_PIN             GPIO_PIN_6




typedef enum
{
    Test_LCD8080,
    Test_SRAM,
    Test_PSRAM,
    Test_NORFLASH,
    Test_SRAM_MUX,
}Test_mode_t;   


void APP_Test(void);




#endif /* __APP_H */

