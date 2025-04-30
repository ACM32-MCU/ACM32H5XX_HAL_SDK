
#include "dac7311.h"


void DAC7311_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Drive     = GPIO_DRIVE_LEVEL3;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;
    
    GPIO_InitStruct.Pin       = DAC1_SYNC_PIN;
    HAL_GPIO_Init(DAC1_SYNC_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin       = DAC2_SYNC_PIN;
    HAL_GPIO_Init(DAC2_SYNC_PORT, &GPIO_InitStruct);
    
    
    GPIO_InitStruct.Pin       = DAC_SCK_PIN;
    HAL_GPIO_Init(DAC_SCK_PORT, &GPIO_InitStruct);
    
    
    GPIO_InitStruct.Pin       = DAC_DIN_PIN;
    HAL_GPIO_Init(DAC_DIN_PORT, &GPIO_InitStruct);
}


void delay(volatile uint8_t us)
{
	for(; us !=0; us--);
}

void DAC7311_Init(void)
{
    DAC7311_GPIO_Init();
	AO1_SYNC_H();
	AO1_SCLK_L();
	AO2_SYNC_H();
	AO2_SCLK_L();
}

void AO1_Write(uint16_t Data)
{
	uint16_t temp;
	uint8_t	ui;
	
	temp=Data << 2;//7311需要移两位
	temp &= 0x3FFF;//选择模式为Normal
	AO1_SYNC_H();
	AO1_SCLK_H();
	delay(10);
	AO1_SYNC_L();//使能输入移位寄存器
	delay(10);
	
	for(ui=0; ui<16; ui++)
	{
		
		AO1_SCLK_H();
		
		if(0x8000 == (temp & 0x8000))
		{
			AO1_DIN_H();
		}
		else
		{
			AO1_DIN_L();
		}
		delay(10);
		AO1_SCLK_L();
		delay(10);
		temp <<=1;
	}
	
	AO1_SYNC_H();

}
void AO2_Write(uint16_t Data)
{
	uint16_t temp;
	uint8_t	ui;
	
	temp=Data << 2;//7311需要移两位
	temp &= 0x3FFF;//选择模式为Normal
	AO2_SYNC_H();
	AO2_SCLK_H();
	delay(10);
	AO2_SYNC_L();//使能输入移位寄存器
	delay(10);
	
	for(ui=0; ui<16; ui++)
	{
		
		AO2_SCLK_H();
		
		if(0x8000 == (temp & 0x8000))
		{
			AO2_DIN_H();
		}
		else
		{
			AO2_DIN_L();
		}
		delay(10);
		AO2_SCLK_L();
		delay(10);
		temp <<=1;
	}
	
	AO2_SYNC_H();
}

void AO1_Set_Voltage(float voltage)
{
	uint16_t data = 0;
	voltage = voltage * 1000;
	data = (voltage * 4096) / 3300;
	AO1_Write(data);
}


void AO2_Set_Voltage(float voltage)
{
	uint16_t data = 0;
	voltage = voltage * 1000;
	data = (voltage * 4096) / 3300;
	AO2_Write(data);
}
