/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 


volatile uint32_t g_press;

/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    uint32_t state;
    
	printfS("GPIO Demo\r\n");
	printfS("\r\n");
	printfS("LED flashing frequency: 1Hz.\r\n");
	printfS("The user presses the key to stop flashing, press the key again, and flash again.\r\n");
	printfS("\r\n");
    
	BSP_LED_Init();
	BSP_LED_On();
     
    g_press = 0;
    BSP_PB_Init();
    
    state = 0;
    while(1)
    {
        if (g_press != 0)
        {
            g_press = 0;
            HAL_Delay(20);
         
            if (HAL_GPIO_ReadPin(BSP_PB_GPIO,BSP_PB_PIN)== 0)
            {
            
               
                if (state == 0)
                {
                    state = 1;
                    printfS("\r\nStop the LED Blink\r\n");
                }
                else
                {
                    state = 0;
                    printfS("\r\nRun the LED Blink\r\n");    
                          
                }
            }
        }
        
        if (state == 0)
        {
        
            for(int i=0;i<500;i++)
            { 
                HAL_Delay(1);             
                if (g_press == 1)  
                break ;
            }
            BSP_LED_Toggle();
            printfS("BSP_LED_Toggle\r\n");
        }
        else
        {
            BSP_LED_Off();
        }
       
		
    };
}


/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void HAL_EXTI_LineCallback(uint32_t Line)
{
	if (Line == BSP_PB_EXTI_LINE)
	{
    
        g_press = 1  ;
      
	}
}
