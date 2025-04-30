/******************************************************************************
*@file  : app.c
*@brief : I2C Master Demos 
******************************************************************************/


#include "app.h"
#include "IC_AT24.h"

uint8_t TxBuffer[BUFFER_LENGTH];
uint8_t RxBuffer[BUFFER_LENGTH];

I2C_HandleTypeDef I2C_Handle;

/******************************************************************************
*@brief : Initiation I2C as Master Mode
*         
*@param : None
*@return: None
******************************************************************************/
void I2C_Init(I2C_TypeDef* I2Cx)
{
    I2C_Handle.Instance           = I2Cx;
    I2C_Handle.Init.I2C_Mode      = I2C_MODE_MASTER;
    I2C_Handle.Init.Tx_Auto_En    = TX_AUTO_ENABLE;
    I2C_Handle.Init.Clock_Speed   = CLOCK_SPEED_STANDARD; 
    I2C_Handle.Init.Stretch_Mode  = STRETCH_MODE_DISABLE;   
    I2C_Handle.Init.filter_enable = FILTER_ALGO_DISABLE; 
    
    HAL_I2C_Init(&I2C_Handle);
}


/*************************For I2C EEprom Test******************/ 
#define AT24_HANDLE             I2C_Handle
#define AT24_DEV_ADDRESS        0xA0

#define I2C_EEWR_WRITE_READ_LENGTH  128

uint8_t I2C_EEWR_Test(uint32_t test_length, uint32_t base_value)  
{

    uint8_t i;
    uint8_t wr_buff[128], rd_buff[128];

    for(i = 0; i < 128; i++)
    {
        wr_buff[i] = i+base_value;
    }
    printfS("iic write data\n");
    IC_AT24_WriteBytes(&AT24_HANDLE, AT24_DEV_ADDRESS, 0x02, wr_buff, test_length,1000);

    printfS("iic read data\n"); 
    IC_AT24_ReadBytes(&AT24_HANDLE, AT24_DEV_ADDRESS, 0x02, rd_buff, test_length,1000);  

	
    if (0 == memcmp(wr_buff, rd_buff, test_length))
    {
        printfS("PASS, length:%d\n", test_length);   
        return 0;  
    }
    else
    {
        printfS("FAIL\n");
        for(i = 0; i < test_length; i++)
        {
            printfS("wr_buff[%d]= 0x%x,rd_buff[%d]= 0x%x \n", i, wr_buff[i], i, rd_buff[i]);
        }   
        return 1;   
    }
}

/******************************************************************************
*@brief : I2C Master EEPROM read/write Demo
*         
*@param : None
*@return: None
******************************************************************************/
void I2C_EEprom_Test(void)
{
    uint32_t test_succ_times,  test_fail_times, i;  
    
    test_succ_times = 0;  
    test_fail_times = 0;
    
    printfS("I2C EEPROM RW Demo(Please connect to an EEPROM. eg. AT24C256)\r\n\r\n");

    I2C_Init(I2C2);
    
//    printfS("Please press the USR_PB button to continue......\r\n\r\n");
//    
//    while(GPIO_PIN_SET == BSP_PB_GetState());
    
    i = 1; 
    
    while(i <= 128)    
    {
        if (0 != I2C_EEWR_Test(i, 0))         
        {
            test_fail_times++;  
            break; 
        }
        
        if (0 != I2C_EEWR_Test(i, 127))  
        {
            test_fail_times++;  
            break; 
        } 
        
        
        test_succ_times++; 
        i++;  
        
    } 
    
    printfS("iic write eeprom test ends, success times:%d, failed times:%d!\n", test_succ_times, test_fail_times);
}

void printf_buff_byte(uint8_t* buff, uint32_t length)
{
	uint32_t i;

	for(i=0;i<length;i++)
	{
		printf("%.2x ",buff[i]);	
	}
	printf("\n");
}

/******************************************************************************
*@brief : I2C Master communication with slave Demo
*         
*@param : None
*@return: None
******************************************************************************/
void I2C_Master_Comm_Test(void)
{
	
	uint32_t i;
    uint32_t Length  = 1;
    uint32_t COM_OK  = 0;
    uint32_t COM_Err = 0;
    HAL_StatusTypeDef Status;
    
    printfS("I2C Master Comm Demo. (Please connect to another board running I2C Slave Comm Demo. Slave should run first)\r\n\r\n");

    I2C_Init(I2C1);
    
    printfS("Please press the USR_PB button to continue......\r\n\r\n");
    
    //while(GPIO_PIN_SET == BSP_PB_GetState());
    
    while (1) 
    {
        if (Length > BUFFER_LENGTH) 
        {
            Length = 1;
        }

        for (i = 0; i < Length; i++)
        {
            TxBuffer[i] = 1 + i;
        }
        
        printfS("I2C Master Test is Ready!!! \r\n");
        
        do
        {
            Status = HAL_I2C_Master_Transmit(&I2C_Handle, SLAVE_ADDRESS, TxBuffer, Length, 0);
        }while(Status != HAL_OK);
        
        HAL_Delay(100);
        
        do
        {
            Status = HAL_I2C_Master_Receive(&I2C_Handle, SLAVE_ADDRESS, RxBuffer, Length, 0);
        }while(Status != HAL_OK);
        
        for (i = 0; i < Length; i++)
        {
            if (TxBuffer[i] != RxBuffer[i]) 
            {
                printfS("ERROR@ %x \r\n", i);
            
                COM_Err++;
                goto end;
            }
            else 
            {
                COM_OK++;
            }
        }

        printfS("I2C Master Test OK count %d times \r\n",  COM_OK);
        printfS("I2C Master Test Err count %d times \r\n", COM_Err);
        
        printfS("I2C Master Test End!!! \r\n");

        for (i = 0; i < BUFFER_LENGTH; i++)
        {
            TxBuffer[i] = 0;
            RxBuffer[i] = 0;
        }

        COM_Err  = 0;
        COM_OK   = 0;

        Length++;

        HAL_Delay(100);
    }
end:    
    printf_buff_byte(RxBuffer, Length);
}


void I2C_Master_Comm_IT(void)
{
	
	uint32_t i;
    uint32_t Length  = 1;
    uint32_t COM_OK  = 0;
    uint32_t COM_Err = 0;
    HAL_StatusTypeDef Status;
    
    printfS("I2C Master IT Demo. (Please connect to another board running I2C Slave Comm Demo. Slave should run first)\r\n\r\n");

    I2C_Init(I2C1);
    
    printfS("Please press the USR_PB button to continue......\r\n\r\n");
    
//    while(GPIO_PIN_SET == BSP_PB_GetState());
    
    while (1) 
    {
        if (Length > BUFFER_LENGTH) 
        {
            Length = 1;
        }

        for (i = 0; i < Length; i++)
        {
            TxBuffer[i] = 1 + i;
        }
        
        printfS("I2C Master Test is Ready!!! \r\n");
        
        do
        {
            Status = HAL_I2C_Master_Transmit_IT(&I2C_Handle, SLAVE_ADDRESS, TxBuffer, Length);
        }while(Status != HAL_OK);
        
        while(I2C_Handle.state != I2C_STATE_IDLE);
        HAL_Delay(100);
        
        do
        {
            Status = HAL_I2C_Master_Receive_IT(&I2C_Handle, SLAVE_ADDRESS, RxBuffer, Length);
        }while(Status != HAL_OK);
        
        while(I2C_Handle.state != I2C_STATE_IDLE);
        
        for (i = 0; i < Length; i++)
        {
            if (TxBuffer[i] != RxBuffer[i]) 
            {
                printfS("ERROR@ %x \r\n", i);
            
                COM_Err++;
                goto end2;
            }
            else 
            {
                COM_OK++;
            }
        }
        
        
        
        printfS("I2C Master Test OK count %d times \r\n",  COM_OK);
        printfS("I2C Master Test Err count %d times \r\n", COM_Err);
        
        printfS("I2C Master Test End!!! \r\n");

        for (i = 0; i < BUFFER_LENGTH; i++)
        {
            TxBuffer[i] = 0;
            RxBuffer[i] = 0;
        }

        COM_Err  = 0;
        COM_OK   = 0;

        Length++;

        HAL_Delay(100);
    }
end2:    
    printf_buff_byte(RxBuffer, Length);
}


void I2C_Master_Test(enum_TEST_MODE_t fe_Mode)
{       
    switch (fe_Mode)
    {
        case TEST_MASTER_COMM:
            I2C_Master_Comm_Test();
            break; 
        
        case TEST_MASTER_IT:
            I2C_Master_Comm_IT();
            break;
        
        case TEST_MASTER_EEPROM: 
            I2C_EEprom_Test();
            break;
        
        default: break;     
    }
}




