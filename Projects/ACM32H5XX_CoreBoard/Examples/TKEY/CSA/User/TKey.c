/*****************************************************************
Copyright(C) 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	Teky.c
@功能描述:	TKEY模块函数。
@开发人员:	Sam 
@完成日期:	2021.06.11
@当前版本:	1.2
@修改记录:	
修改日期        修改人              修改说明
2021-12-03      Sam                添加自动校准机制 
2021-12-29      Sam                快速校准函数TKEY_Quick_Calibrate()添加自动调整扫描等待时间,RawData波动范围             
******************************************************************/
#include "TKey.h"
#include "ac_fifo.h"

/*信息输出*/
#define TKEY_DEBUG_ON         0

#define TKEY_DEBUG(fmt,arg...)          do{\
                                            if(TKEY_DEBUG_ON)\
                                                printf("\r\n<<-TKey.c->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)


//#ifdef TKEY_WAVEFORM_OUTPUT
    UART_HandleTypeDef UART2_Handle;
//#endif

static uint8_t sgu8_TKEYScanTime;
static uint16_t sgu16_RawDataRangRatio;
                                          
extern UART_HandleTypeDef BSP_UART_Handle;
extern volatile uint32_t g32TkeyIrqFlag;
extern volatile uint32_t g32ExtiIrqLine;
extern volatile uint16_t g16TkeyScanInterval;
                                          
uint8_t checkSum;
uint8_t reset_cnt = 0;                                          
#ifdef TKEY_FIFO_ENABLE
     static uint8_t TKEY_FifoBuffer[TOTAL_CHANNELS];
     static FIFO_TypeDef TKEY_Fifo;  
#endif  
extern TKEY_HandleTypeDef TKEY_Handle;
TKEY_DataDef TKey_Channel_Data[TOTAL_CHANNELS]={0};
TKEY_CalibartData TKey_Calibart_Data[TOTAL_CHANNELS]={0};

extern void Internal_InitEXTI(uint8_t linex, uint8_t triggr, void (*EXTIxx_IRQCallback)(void));

/*****************************************************************
*   设置每个通道基本参数
*   {DetectInTH[按下阀值], WakeUpTh[唤醒阀值], }
******************************************************************/
TKEY_BaseParaDef TKEY_Channel_BasePara[TOTAL_CHANNELS] = {   
#ifdef  TKEY_CHANNEL_0_ENABLE   //TKEY_CHANNEL_0 UART_RX  
    {TKEY_0_SENSITIVITY, TKEY_0_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_1_ENABLE   //TKEY_CHANNEL_1   
    {TKEY_1_SENSITIVITY, TKEY_1_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_2_ENABLE   //TKEY_CHANNEL_2   
    {TKEY_2_SENSITIVITY, TKEY_2_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_3_ENABLE   //TKEY_CHANNEL_3 SWDIO   
    {TKEY_3_SENSITIVITY, TKEY_3_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_4_ENABLE   //TKEY_CHANNEL_4   
    {TKEY_4_SENSITIVITY, TKEY_4_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_5_ENABLE   //TKEY_CHANNEL_5   
    {TKEY_5_SENSITIVITY, TKEY_5_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_6_ENABLE   //TKEY_CHANNEL_6 SWDCLK   
    {TKEY_6_SENSITIVITY, TKEY_6_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_7_ENABLE   //TKEY_CHANNEL_7   
    {TKEY_7_SENSITIVITY, TKEY_7_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_8_ENABLE   //TKEY_CHANNEL_8   
    {TKEY_8_SENSITIVITY, TKEY_8_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_9_ENABLE   //TKEY_CHANNEL_9   
    {TKEY_9_SENSITIVITY, TKEY_9_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_10_ENABLE  //TKEY_CHANNEL_10   
    {TKEY_10_SENSITIVITY, TKEY_10_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_11_ENABLE  //TKEY_CHANNEL_11   
    {TKEY_11_SENSITIVITY, TKEY_11_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_12_ENABLE  //TKEY_CHANNEL_12   
    {TKEY_12_SENSITIVITY, TKEY_12_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_13_ENABLE  //TKEY_CHANNEL_13   
    {TKEY_13_SENSITIVITY, TKEY_13_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_14_ENABLE  //TKEY_CHANNEL_14   
    {TKEY_14_SENSITIVITY, TKEY_14_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_15_ENABLE  //TKEY_CHANNEL_15   
    {TKEY_15_SENSITIVITY, TKEY_15_SLEEP_SENSITIVITY},
#endif
};


/*****************************************************************
*   TKey通道配置
*   ChannelId为TKEY_CHANNEL_1-TKEY_CHANNEL_15
******************************************************************/
TKEY_ChannelDataDef TKEY_Config[] = {	
#ifdef  TKEY_CHANNEL_0_ENABLE  
    {&TKEY_Channel_BasePara[TK_0], &TKey_Channel_Data[TK_0], &TKey_Calibart_Data[TK_0], TKEY_CHANNEL_0 },
#endif
#ifdef  TKEY_CHANNEL_1_ENABLE     
    {&TKEY_Channel_BasePara[TK_1], &TKey_Channel_Data[TK_1], &TKey_Calibart_Data[TK_1], TKEY_CHANNEL_1 },
#endif
#ifdef  TKEY_CHANNEL_2_ENABLE      
    {&TKEY_Channel_BasePara[TK_2], &TKey_Channel_Data[TK_2], &TKey_Calibart_Data[TK_2], TKEY_CHANNEL_2 },
#endif
#ifdef  TKEY_CHANNEL_3_ENABLE     
    {&TKEY_Channel_BasePara[TK_3], &TKey_Channel_Data[TK_3], &TKey_Calibart_Data[TK_3], TKEY_CHANNEL_3 },
#endif
#ifdef  TKEY_CHANNEL_4_ENABLE     
    {&TKEY_Channel_BasePara[TK_4], &TKey_Channel_Data[TK_4], &TKey_Calibart_Data[TK_4], TKEY_CHANNEL_4 },
#endif
#ifdef  TKEY_CHANNEL_5_ENABLE      
    {&TKEY_Channel_BasePara[TK_5], &TKey_Channel_Data[TK_5], &TKey_Calibart_Data[TK_5], TKEY_CHANNEL_5 },
#endif
#ifdef  TKEY_CHANNEL_6_ENABLE     
    {&TKEY_Channel_BasePara[TK_6], &TKey_Channel_Data[TK_6], &TKey_Calibart_Data[TK_6], TKEY_CHANNEL_6 },
#endif
#ifdef  TKEY_CHANNEL_7_ENABLE      
    {&TKEY_Channel_BasePara[TK_7], &TKey_Channel_Data[TK_7], &TKey_Calibart_Data[TK_7], TKEY_CHANNEL_7 },
#endif
#ifdef  TKEY_CHANNEL_8_ENABLE      
    {&TKEY_Channel_BasePara[TK_8], &TKey_Channel_Data[TK_8], &TKey_Calibart_Data[TK_8], TKEY_CHANNEL_8 },
#endif
#ifdef  TKEY_CHANNEL_9_ENABLE      
    {&TKEY_Channel_BasePara[TK_9], &TKey_Channel_Data[TK_9], &TKey_Calibart_Data[TK_9], TKEY_CHANNEL_9 },
#endif
#ifdef  TKEY_CHANNEL_10_ENABLE      
    {&TKEY_Channel_BasePara[TK_10], &TKey_Channel_Data[TK_10], &TKey_Calibart_Data[TK_10], TKEY_CHANNEL_10},
#endif
#ifdef  TKEY_CHANNEL_11_ENABLE      
    {&TKEY_Channel_BasePara[TK_11], &TKey_Channel_Data[TK_11], &TKey_Calibart_Data[TK_11], TKEY_CHANNEL_11},
#endif
#ifdef  TKEY_CHANNEL_12_ENABLE      
    {&TKEY_Channel_BasePara[TK_12], &TKey_Channel_Data[TK_12], &TKey_Calibart_Data[TK_12], TKEY_CHANNEL_12},
#endif
#ifdef  TKEY_CHANNEL_13_ENABLE      
    {&TKEY_Channel_BasePara[TK_13], &TKey_Channel_Data[TK_13], &TKey_Calibart_Data[TK_13], TKEY_CHANNEL_13},
#endif
#ifdef  TKEY_CHANNEL_14_ENABLE      
    {&TKEY_Channel_BasePara[TK_14], &TKey_Channel_Data[TK_14], &TKey_Calibart_Data[TK_14], TKEY_CHANNEL_14},
#endif
#ifdef  TKEY_CHANNEL_15_ENABLE      
    {&TKEY_Channel_BasePara[TK_15], &TKey_Channel_Data[TK_15], &TKey_Calibart_Data[TK_15], TKEY_CHANNEL_15},
#endif
   
    {0, 0, 0, TKEY_CHANNEL_NULL},
 
};


/*****************************************************************
@功能	TKey通道扫描完成中断处理回调函数。
@参数	无
@返回	无
******************************************************************/
void TKEY_ScanFinish_Callback(void)
{
#ifdef  MULTI_KEY_SHIELD_OUTPUT_MAX       
    HAL_TKEY_MultiKeyShieldScanProcess(&TKEY_Handle);  //耗时1ms            
    if(TKEY_Handle.ChannelDetectedNum)
    {
    #ifdef TKEY_FIFO_ENABLE
        if(TKEY_Handle.ChannelDetectedNum  == 1)
        {
            FIFO_In(&TKEY_Fifo, TKEY_Handle.ChannelValue);
        }
    #endif     
        TKEY_Handle.ChannelDetectedNum = 0;
        TKEY_Handle.ChannelValue = 0;     
    }
#else
    TKEY_ScanProcess(&TKEY_Handle);
    TKEY_Press_Relaese_Process();
#endif 
}

/*****************************************************************
@功能	TKey校验参数初始化。
@参数	无
@返回	无
******************************************************************/
void TKEY_Calibrate_ParaInit(void)
{
    uint8_t ucI;
    for(ucI = 0; TKEY_Handle.ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
        TKEY_Handle.ChannelData[ucI].Tkey_CalData->pos = 0;
        TKEY_Handle.ChannelData[ucI].Tkey_CalData->ScanTimer = 0;
        TKEY_Handle.ChannelData[ucI].Tkey_CalData->DetectingTimer = 0;
        TKEY_Handle.ChannelData[ucI].Tkey_CalData->CalFlag = TKEY_CALIBRAT_IDLE;
    }
    
    TKEY_Handle.ChannelData[ucI].Tkey_Data->Noise = 20;
  
}
/*****************************************************************
@功能	TKey触摸快速校准。
@参数	无
@返回	0-成功，非0-失败：1-扫描等待时间异常；2-RawData波动范围异常
******************************************************************/
uint8_t TKEY_Quick_Calibrate(void)
{
    uint8_t ucI,ucJ = 0;
    static uint16_t RawData_Ref[TOTAL_CHANNELS] = {0};
    uint32_t RawData_Sum[TOTAL_CHANNELS] = {0};
    uint8_t count[TOTAL_CHANNELS] = {0};
    __IO uint32_t *gu32RegTemp;
    uint8_t Temp_Delta = 0;
    uint8_t Temp_timeout_cnt = 0;
    uint8_t Temp_outrange_cnt = 0;
    uint8_t Temp_reg;

QUICK_CALIBRATE:
    for(ucJ = 0; ucJ < TKEY_QUICK_QUICK_CALIBRATE_TIMES; ucJ++)
    {
        if(!(TKEY->CR & TKEY_CR_CONT))
        {
            /*Start the Tkey scan*/    
            SET_BIT(TKEY->CR, TKEY_CR_START); 
        }
        while(READ_BIT(TKEY->SR, TKEY_SR_EOC) == 0)
        {
            if(READ_BIT(TKEY->SR, TKEY_SR_BUSY) == 0) //Some times will stop.restart. 
                SET_BIT(TKEY->CR, TKEY_CR_START); 
            
            if(READ_BIT(TKEY->SR, TKEY_SR_TIMEOUT))
            {
                SET_BIT(TKEY->SR, TKEY_SR_TIMEOUT); //Clear the timeout flag
                Temp_timeout_cnt++;
                if(Temp_timeout_cnt == TKEY_QUICK_QUICK_CALIBRATE_TIMES)
                {
                    //CSD模式下，当采样数据为0xffff时，扫描超时标志置位
                    //此时应该降低扫描时钟频率，或者增大放电速度，或者减小PWM窗口，从而减小采样值
                    Temp_reg = READ_REG(TKEY->TSETR);
                    Temp_reg = Temp_reg - 0x10;                
                    WRITE_REG(TKEY->TSETR, Temp_reg);
                    
                }
                goto QUICK_CALIBRATE;
            }
        }
        TKEY->SR = TKEY_SR_EOC;
        gu32RegTemp = &TKEY->CH[0];

        for(ucI = 0; TKEY_Handle.ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
        {
            /* Read the data and calculate the delta.*/
            TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData = *(gu32RegTemp + TKEY_Handle.ChannelData[ucI].ChannelId);
            Temp_Delta = TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData / sgu16_RawDataRangRatio;        
            if((RawData_Ref[ucI])&&((RawData_Ref[ucI] > (int32_t)(TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData + Temp_Delta)) \
            || (RawData_Ref[ucI] < ((int32_t)(TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData - Temp_Delta)))))
            {
                for(uint8_t i=0;TKEY_Handle.ChannelData[i].ChannelId != 0xFFFF;i++)
                {
                    count[i] = 0;
                    RawData_Ref[i] = 0;
                    RawData_Sum[i] = 0;                 
                }
                Temp_outrange_cnt++; 
                if(Temp_outrange_cnt == TKEY_QUICK_QUICK_CALIBRATE_TIMES)
                {
                    Temp_outrange_cnt = 0;                
                    //自动减小RawData上下波动范围系数:sgu16_RawDataRangRatio,每次减小当前值的(1/TKEY_RAWDATA_RANGE_DEC_RATIO)
                    sgu16_RawDataRangRatio -= (sgu16_RawDataRangRatio / TKEY_RAWDATA_RANGE_DEC_RATIO); 
                    if(sgu16_RawDataRangRatio < TKEY_RAWDATA_RANGE_MIN_RATIO)
                    {
                        TKEY_DEBUG("sgu16_RawDataRangRatio less than TKEY_RAWDATA_RANGE_MIN_RATIO\n");
                        return 2;
                    }                   
                }
                goto  QUICK_CALIBRATE;            
            }
            else
            {
                RawData_Sum[ucI] +=  TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData;             
                count[ucI]++;
                if(count[ucI] == TKEY_QUICK_QUICK_CALIBRATE_TIMES)
                {
                    TKEY_Handle.ChannelData[ucI].Tkey_Data->BaseLine =  RawData_Sum[ucI] / TKEY_QUICK_QUICK_CALIBRATE_TIMES;
                }
            }
            RawData_Ref[ucI] = (int32_t)TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData;
        }
    } 
    return 0; 
}

/************************************************************************
 * function   : HAL_TKEY_Quick_Calibrate_RefData_OneChannel
 * Description: TKEY Quick Calibration Single Channel Basic Reference Data. 
 * input      : htkey : TKEY handle
                CalTimes: The calibrat times.
                ChannelId: the TKEY channel
 * return     : None
 ************************************************************************/
void HAL_TKEY_Quick_Calibrate_RefData_OneChannel(TKEY_HandleTypeDef* htkey, uint16_t ChannelId, uint8_t CalTimes)
{
    uint8_t ucI,ucJ;
    uint32_t sum[16];
    
    memset(sum,0,sizeof(sum));
 
    for(ucJ=0; ucJ < CalTimes; ucJ++)
    {
        HAL_TKEY_ReadChannelData(htkey); 
        for(ucI = 0; htkey->ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
        {
            if(htkey->ChannelData[ucI].ChannelId == ChannelId)
            {
                if(htkey->ChannelData[ucI].Tkey_Data->RawData)
                {
										//printf("*****quick*****\n");
                    sum[ucI] +=  htkey->ChannelData[ucI].Tkey_Data->RawData;
                }
                if(ucJ == (CalTimes-1))
                {
                    htkey->ChannelData[ucI].Tkey_Data->BaseLine =  sum[ucI]/CalTimes;
                }
            }

        }
    }    
}

/*****************************************************************
@功能	TKey触摸自校准处理。
@参数	无
@返回	无
******************************************************************/
void TKEY_Calibrate_Process(void)
{
    uint8_t ucI;
    const TKEY_ChannelDataDef *ChannelData;
    uint16_t average, differ;
    for(ucI = 0; TKEY_Handle.ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
				
        ChannelData = &TKEY_Handle.ChannelData[ucI];

        if(ChannelData->Tkey_CalData->CalFlag == TKEY_CALIBRAT_SELF)//自我校准
        {

            //排序
            Bubble_Sort(ChannelData->Tkey_CalData->Caldata, TKEY_CALIBRATE_BUFF_MAX);
            //去掉两个最大值&最小值，避免脉冲干扰，并求平均值
            average = Calculate_Sample_Average(&ChannelData->Tkey_CalData->Caldata[2], TKEY_CALIBRATE_BUFF_MAX-4);

            //计算滑动平均滤波
            differ = Calculate_Sample_differ(&ChannelData->Tkey_CalData->Caldata[2], TKEY_CALIBRATE_BUFF_MAX-4, average);     

			if(differ < 10)
            {
                ChannelData->Tkey_Data->BaseLine = average;
            }
            ChannelData->Tkey_CalData->CalFlag = TKEY_CALIBRAT_IDLE;

            ChannelData->Tkey_Data->StateId = TKEY_STATEID_RELEASE;
        }
        else if(ChannelData->Tkey_CalData->CalFlag == TKEY_CALIBRAT_QUICK)//快速校准
        {
            ChannelData->Tkey_CalData->CalFlag = TKEY_CALIBRAT_IDLE;
            TKEY_DEBUG("TK_%d:TKEY_Quick_Calibrate\n",TKEY_Handle.ChannelData[ucI].ChannelId);
            TKEY_DEBUG("TK_%d:Baseline[%d]-Rawdata[%d]\n", TKEY_Handle.ChannelData[ucI].ChannelId, \
            TKEY_Handle.ChannelData[ucI].Tkey_Data->BaseLine, TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData);
            //HAL_TKEY_Quick_Calibrate_RefData(&TKEY_Handle,TKEY_QUICK_QUICK_CALIBRATE_TIMES);
            HAL_TKEY_Quick_Calibrate_RefData_OneChannel(&TKEY_Handle, TKEY_Handle.ChannelData[ucI].ChannelId, TKEY_QUICK_QUICK_CALIBRATE_TIMES);
            TKEY_DEBUG("TK_%d:TKEY_CALIBRAT_SELF BaseLine=%d\n",TKEY_Handle.ChannelData[ucI].ChannelId,TKEY_Handle.ChannelData[ucI].Tkey_Data->BaseLine);
            ChannelData->Tkey_Data->StateId = TKEY_STATEID_RELEASE;
        } 
    }
}

/*****************************************************************
@功能	TKey触摸RTC自校准处理。
@参数	无
@返回	无
******************************************************************/
void TKEY_RTC_Calibrate_Process(void)
{
    uint8_t ucI;
    __IO uint32_t *gu32RegTemp;
    uint16_t average, differ;
    
    while(READ_BIT(TKEY->SR, TKEY_SR_EOC) == 0)
    {
        if(READ_BIT(TKEY->SR, TKEY_SR_BUSY) == 0) //Some times will stop.restart. 
            SET_BIT(TKEY->CR, TKEY_CR_START); 
        
        if(READ_BIT(TKEY->SR, TKEY_SR_TIMEOUT))
        {
            SET_BIT(TKEY->SR, TKEY_SR_TIMEOUT); //Clear the timeout flag
            return;
        }
    }
    TKEY->SR = TKEY_SR_EOC;
    gu32RegTemp = &TKEY->CH[0];
    for(ucI = 0; TKEY_Handle.ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
        TKEY_Handle.ChannelData[ucI].Tkey_CalData->Caldata[TKEY_Handle.ChannelData[ucI].Tkey_CalData->pos] = *(gu32RegTemp + TKEY_Handle.ChannelData[ucI].ChannelId);
        if((TKEY_Handle.ChannelData[ucI].Tkey_CalData->ScanTimer)++ >= TKEY_CALIBRATE_TIMES)
        {
            TKEY_Handle.ChannelData[ucI].Tkey_CalData->ScanTimer = 0;           
            //排序
            Bubble_Sort(TKEY_Handle.ChannelData[ucI].Tkey_CalData->Caldata, TKEY_CALIBRATE_BUFF_MAX);
            //去掉两个最大值&最小值，避免脉冲干扰，并求平均值
            average = Calculate_Sample_Average(&TKEY_Handle.ChannelData[ucI].Tkey_CalData->Caldata[2], TKEY_CALIBRATE_BUFF_MAX-4);
            //计算滑动平均滤波
            differ = Calculate_Sample_differ(&TKEY_Handle.ChannelData[ucI].Tkey_CalData->Caldata[2], TKEY_CALIBRATE_BUFF_MAX-4, average);     

            if(differ <= TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData / sgu16_RawDataRangRatio)
            {
                TKEY_Handle.ChannelData[ucI].Tkey_Data->BaseLine = average;
            }
        }
        if((TKEY_Handle.ChannelData[ucI].Tkey_CalData->pos)++ >= TKEY_CALIBRATE_BUFF_MAX)
        {
            TKEY_Handle.ChannelData[ucI].Tkey_CalData->pos = 0;
        }
    }
     
}

/*****************************************************************
@功能	串口输出TKEY波形,与ACTouchKeyTool V1.01.exe工具配合使用。
@参数	无 
@参数   无  
@返回	无
******************************************************************/
static void TKEY_DebugDataOut(void)
{
    uint8_t ucI, i, tk_ch_h, tk_ch_l;
    uint8_t length = 0;
    unsigned short data_checksum = 0;
    
    unsigned short raw_data[16];
    unsigned short base_line[16];
    
    uint8_t gucSendBuff[128], ucSendLength;

    uint8_t opened_tk_amount = TOTAL_CHANNELS;

    for(i = 0; i < opened_tk_amount; i++)
    {
        raw_data[i] = TKEY_Handle.ChannelData[i].Tkey_Data->RawData;
        base_line[i] = TKEY_Handle.ChannelData[i].Tkey_Data->BaseLine;
    }

    tk_ch_h = 0;
    tk_ch_l = 0;
    for(ucI = 0; TKEY_Handle.ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
        if(TKEY_Handle.ChannelData[ucI].ChannelId < 8)
        {
            tk_ch_l |= (1 << TKEY_Handle.ChannelData[ucI].ChannelId);
        }
        else
        {
            tk_ch_h |= (1 << (TKEY_Handle.ChannelData[ucI].ChannelId-8));
        }
    }

    ucSendLength = 0;
    memset(gucSendBuff, 0x00, sizeof(gucSendBuff));
    gucSendBuff[ucSendLength++] = 0x55;
    gucSendBuff[ucSendLength++] = 0xAA;
    gucSendBuff[ucSendLength++] = 0x55;
    gucSendBuff[ucSendLength++] = 0xAA;
    length = 1 + 2 + 4 + 1 + 16 + 4 * opened_tk_amount + 2;
    
    gucSendBuff[ucSendLength++] = length;
    gucSendBuff[ucSendLength++] = 0;
    gucSendBuff[ucSendLength++] = 0;
    gucSendBuff[ucSendLength++] = 0;
    gucSendBuff[ucSendLength++] = 0;
    data_checksum = length;

    gucSendBuff[ucSendLength++] = tk_ch_h;
    gucSendBuff[ucSendLength++] = tk_ch_l;
    //TK Channel H
    data_checksum += tk_ch_h;
    
    //TK Channel L
    data_checksum += tk_ch_l;

    gucSendBuff[ucSendLength++] = 16;
    //TK Channel MAX
    data_checksum += 16;
    for(i = 0; i < 16; i++)
    {
        gucSendBuff[ucSendLength++] = i;
        data_checksum += i;
    }
    for(i = 0; i < opened_tk_amount; i++)
    {
        gucSendBuff[ucSendLength++] = (uint8_t)(base_line[i] >> 8);
        gucSendBuff[ucSendLength++] = (uint8_t)base_line[i];
        data_checksum += (uint8_t)(base_line[i] >> 8);
        data_checksum += (uint8_t)base_line[i];
    }
    for(i = 0; i < opened_tk_amount; i++)
    {
        gucSendBuff[ucSendLength++] = (uint8_t)(raw_data[i] >> 8);
        gucSendBuff[ucSendLength++] = (uint8_t)raw_data[i];
        data_checksum += (uint8_t)(raw_data[i] >> 8);
        data_checksum += (uint8_t)raw_data[i];
    }
    data_checksum ^= 0x55AA;

    gucSendBuff[ucSendLength++] = (uint8_t)(data_checksum >> 8);
    gucSendBuff[ucSendLength++] = (uint8_t)data_checksum;
    
#ifdef TKEY_WAVEFORM_OUTPUT     
    HAL_UART_Transmit(&BSP_UART_Handle, gucSendBuff, ucSendLength, 0xFFFFFFFF);  
#endif 
}

/*****************************************************************
@功能	TKey触摸调试波形输出处流程
@参数	无
@返回	无
******************************************************************/
void TKEY_Waveform_Output_Process(void)
{
    while(1)
    {
        HAL_TKEY_ReadChannelData(&TKEY_Handle); 
        TKEY_DebugDataOut();  //Send the date to the PC tools
    }    
}

/*****************************************************************
@功能	Tkey唤醒回调函数
@参数	无
@返回	无
******************************************************************/
__weak void TKEY_Wakeup_Callback(void)
{
    /* 
    NOTE: This function should be modified, when the callback is needed,
    the TKEY_Wakeup_Callback can be implemented in the user file.
    */       
}

/*****************************************************************
@功能	TKEY按压与释放标志处理函数。
@参数	无 
@返回	无
******************************************************************/
void TKEY_Press_Relaese_Process(void) 
{
    uint8_t i=0, key_num = 0;
    uint16_t TouchBits = 0;
    uint16_t TouchBits_ref = 0;
#ifdef   MULTI_KEY_SHIELD_OUT_NONE      
    uint16_t key_value_ref = 0;
#endif
    static uint8_t key_flags = NO_KEY_DOWN;
    static uint16_t key_value = 0;

    TouchBits = TKEY_Handle.PressRelaeseFlag;
    TouchBits_ref = TouchBits;
    

    if(TouchBits == 0)
    {
        key_flags = NO_KEY_DOWN;
        key_value = 0;       
        return;
    }
    
    for(i = 0; i < 16; i++)
    {		
        TouchBits &= (~key_value);
        if((TouchBits >> i) & 0x0001)
        {
            key_num++;
        }
    }
    
#ifdef  MULTI_KEY_SHIELD_OUT_NONE  

    key_value_ref = key_value;
    if(key_flags == NO_KEY_DOWN)
    {
        if(key_num == 1)
        {
            key_flags = KEY_DOWN;
        }
    }
    else if((key_value & TouchBits) == 0)
    {
        if(key_value != TouchBits_ref)
        {
            key_flags = NO_KEY_DOWN;
            key_value = 0;
        }
        if(key_num == 1)
        {
            key_flags = OTHER_KEY_DOWN;
        }
    }

    if(key_flags != NO_KEY_DOWN)
    {
        if(key_num == 1 && (key_value & TouchBits) == 0)
        {
            for(i = 0; i < 16; i++)
            {
                if((key_num == 1) && ((TouchBits >> i) & 0x0001))//判断按键值
                {							              	       							
                    if((key_value_ref & TouchBits_ref) == 0)
                    {
                        key_value = TouchBits;
                        FIFO_In(&TKEY_Fifo, i);
                    }
                    else
                    {
                        key_value = key_value_ref;
                    }
                    break;
                }
            }
        }
    }

#else


    if(key_flags == NO_KEY_DOWN)
    {
        key_flags = KEY_DOWN;
        TKEY_DEBUG("KEY_DOWN\r\n");
    }
    else if((key_value & TouchBits) == 0)
    {
        TKEY_DEBUG("0 TouchBits = 0x%04x, key_value = 0x%04x, key_flags = 0x%02x\r\n", TouchBits, key_value, key_flags);
        key_value = 0;
        key_flags = OTHER_KEY_DOWN;
        TKEY_DEBUG("1 TouchBits = 0x%04x, key_value = 0x%04x, key_flags = 0x%02x\r\n", TouchBits, key_value, key_flags);
    }

    if(key_flags != NO_KEY_DOWN)
    {
        TKEY_DEBUG("3 TouchBits = 0x%04x, key_value = 0x%04x,TKEY_Handle.PressRelaeseFlag = 0x%04x,key_flags = 0x%02x\r\n", TouchBits, key_value,TKEY_Handle.PressRelaeseFlag,key_flags);   
        if((key_value & TouchBits) == 0)
        {
            TKEY_DEBUG("4 TouchBits = 0x%04x, key_value = 0x%04x,TKEY_Handle.PressRelaeseFlag = 0x%04x\r\n", TouchBits, key_value,TKEY_Handle.PressRelaeseFlag);   
            key_value = TouchBits_ref;
            TKEY_DEBUG("5 TouchBits = 0x%04x, key_value = 0x%04x, key_flags = 0x%02x\r\n", TouchBits, key_value, key_flags);    
            
            TKEY_DEBUG("TouchBits = 0x%x\n", TouchBits);
            for(i = 0; i < 16; i++)
            {
                if(((TouchBits >> i) & 0x0001))//判断按键值
                {							              	       							
                    FIFO_In(&TKEY_Fifo, i);
                }
            }
        }
    }
    
#endif
 
}

void TKey_BaseLine_Init(TKEY_HandleTypeDef* htkey)
{
    uint32_t sample_sum[16];
    uint16_t sample_max[16] = {0};
    int i;
    uint8_t ucI;
    
    for(i = 0;i < 10; i++) 
    {
        HAL_TKEY_ReadChannelData_SingleAll_GND(&TKEY_Handle);
    }
    for(i = 0;i < 10; i++) 
    {
        HAL_TKEY_ReadChannelData_SingleAll(&TKEY_Handle);
    }
		
    memset(sample_sum, 0, 64);
    for(i = 0;i < TKEY_QUICK_QUICK_CALIBRATE_TIMES;) //取通道初始均值作为baseline
    {            
        HAL_TKEY_ReadChannelData_SingleAll(&TKEY_Handle);
        
        for(ucI = 0; htkey->ChannelData[ucI].ChannelId != 0xFFFF; ucI++) //扫描12个通道生成baseline
        {        
            sample_sum[ucI] += htkey->ChannelData[ucI].Tkey_Data->RawData;
            if(htkey->ChannelData[ucI].Tkey_Data->RawData > sample_max[ucI])
            {
                sample_max[ucI] = htkey->ChannelData[ucI].Tkey_Data->RawData;
            }
        }

        HAL_TKEY_ClearFlag(TKEY_IT_EOCIE);
        NVIC_ClearPendingIRQ(TKEY_IRQn);
        HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);
        i++;
    }
    for(ucI = 0; htkey->ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
         htkey->ChannelData[ucI].Tkey_Data->BaseLine = sample_sum[ucI]/TKEY_QUICK_QUICK_CALIBRATE_TIMES;
    }	   
}

/*****************************************************************
@原型	uint8_t TouchKey_Init(void)
@功能	按键模块参数初始化
@参数	无
@返回	无
******************************************************************/
uint8_t TouchKey_Init(void)
{
    uint8_t ret = 0;
    uint8_t ucI;

    g32TkeyIrqFlag = 0;    

    TKEY_Handle.ChannelData = TKEY_Config;
    sgu16_RawDataRangRatio = TKEY_RAWDATA_RANGE_RATIO;
	#ifdef TKEY_COVER_PLATE_THICKNESS
	  switch (TKEY_COVER_PLATE_THICKNESS)
		{
			case 0:
				
					//break;
			case 1:

					//break;
			case 2:

					//break;
			case 3:

			case 4:

			case 5:
						for(ucI = 0; TKEY_Handle.ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
						{
						   TKEY_Handle.ChannelData[ucI].Tkey_Data->StateId = TKEY_STATEID_RELEASE;
	                       TKEY_Handle.ChannelData[ucI].Tkey_Data->DebIn = 0;
						   TKEY_Handle.ChannelData[ucI].Delta_Sum_Cnt = 2;	
							
							if(TKEY_Handle.ChannelData[ucI].ChannelId == 0)
							{

								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 25;

							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 1)
							{
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH =15;

							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 2)
							{
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 15;

							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 3)
							{
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 18;

							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 4)
							{
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 25;

							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 5)
							{
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 20;

							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 6)
							{
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 20;

							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 7)
							{
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 30;

							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 8)
							{
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 25;

							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 9)
							{
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 15;

							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 10)
							{
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 15;	

							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 11)
							{
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 20;
							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 12)
							{
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 25;

							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 13)
							{

								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 15;
							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 14)
							{

								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 15;
							}
							else if(TKEY_Handle.ChannelData[ucI].ChannelId == 15)
							{
	
								TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 25;

							}
							else
							{
							  TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 10;

							}
                            TKEY_Handle.ChannelData[ucI].Tkey_Data->DetectOutTH = TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH * 80 / 100;
							TKEY_Handle.ChannelData[ucI].Delta_Sum = TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH << TKEY_Handle.ChannelData[ucI].Delta_Sum_Cnt;
							TKEY_Handle.ChannelData[ucI].Predict_Touch_Delta  = TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH;
							
							
						}
					break;
					
			default:
			    
					break;
     }
		 #else
    //按键状态初始化
    for(ucI = 0; TKEY_Handle.ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
        TKEY_Handle.ChannelData[ucI].Tkey_Data->StateId = TKEY_STATEID_RELEASE;
        TKEY_Handle.ChannelData[ucI].Tkey_RefPara->DetectInTH = 17000;
        TKEY_Handle.ChannelData[ucI].Tkey_Data->DebIn = 0;
    }
		#endif
    
    TKEY_Calibrate_ParaInit();
	TKEY_CSA_Init_Single();

#ifdef TKEY_FIFO_ENABLE
    memset(TKEY_FifoBuffer, 0, sizeof(TKEY_FifoBuffer));
    FIFO_Create(&TKEY_Fifo, TKEY_FifoBuffer, sizeof(TKEY_FifoBuffer));
    FIFO_Clear(&TKEY_Fifo);
#endif
    
    /* 开启扫描 */
    HAL_TKEY_StartScan();
    TKey_BaseLine_Init(&TKEY_Handle);    
	
	#ifndef TKEY_WAVEFORM_PRINTF              	
	printf("*************************<Start>**********************************\n");
	#endif
    return ret;
}


/*****************************************************************
@功能	TKey定时扫描并获取键值。
@参数	无
@返回	TK_NULL,键值
******************************************************************/
uint8_t TouchKey_TimerSacn_GetKeyVal(void)
{
	uint8_t  ucKey = 0xFF;
#ifdef TKEY_FIFO_ENABLE
    if(FIFO_Query(&TKEY_Fifo))
    {
        FIFO_Out(&TKEY_Fifo, &ucKey);
    }
#endif
#ifdef TKEY_WAVEFORM_OUTPUT    
    TKEY_DebugDataOut();
#endif
    return ucKey;
}

/*****************************************************************
@功能	获取TKey按键按压或释放标志。
@参数	无
@返回	按压与释放标志，bit0-15对应TK0-15，bit为1表示按键按下，为0表示释放
******************************************************************/
uint16_t TouchKey_GetPressRelaeseFlag(void)
{
#ifdef TKEY_WAVEFORM_OUTPUT    
    TKEY_DebugDataOut();
#endif
    return TKEY_Handle.PressRelaeseFlag;
}
/*****************************************************************
@功能	清除TKey按键按压或释放标志。注意：按键释放时系统会自动清除。但手动清除该标志并不代表按键释放
@参数	无
@返回	无
******************************************************************/
void TouchKey_ClearPressRelaeseFlag(void)
{
    TKEY_Handle.PressRelaeseFlag = 0;
}

/*****************************************************************
@功能	获取TKey按键长按标志
@参数	无
@返回	长按标志，bit0-15对应TK0-15，bit为1表示按键长按，为0表示非长按
******************************************************************/
uint16_t TouchKey_GetLongPressFlag(void)
{
    return TKEY_Handle.LongPressFlag;
}

/*****************************************************************
@功能	冒泡排序算法: 比较相邻的两个数，如果前者比后者大，则进行交换。
        每一轮排序结束，选出一个未排序中最大的数放到数组后面。
@参数	arr: 指向要转换的数据
@参数   n[IN]:要转换的数据长度
@返回	无	
******************************************************************/
void Bubble_Sort(uint16_t *arr, uint16_t n) 
{
    uint16_t i, j, temp;
    for (i = 0; i<n - 1; i++)
	{
        for (j = 0; j < n - i - 1; j++)
        {
            //如果前面的数比后面大，进行交换
            if (arr[j] > arr[j + 1]) 
            {
                temp = arr[j]; 
                arr[j] = arr[j + 1]; 
                arr[j + 1] = temp;
            }
        }
	}
}

/*****************************************************************
@功能	计算平均值
@参数	arr[IN]: 指向要计算的数据
@参数   n[IN]:要计算的数据长度
@返回	平均值	
******************************************************************/
uint16_t Calculate_Sample_Average(uint16_t *arr, uint16_t n)
{
    uint16_t average = 0;
    for(uint16_t i = 0; i < n; i++)
    {
        average += arr[i];
        average >>= 1;
    }
    return average;
}


/*****************************************************************
@功能	计算采样值的波动值
@参数	arr[IN]: 指向要计算的数据
@参数   n[IN]:要计算的数据长度
@参数   average[IN]:平均值
@返回	波动值	
******************************************************************/
uint16_t Calculate_Sample_differ(uint16_t *arr, uint16_t n, uint16_t average)
{
    uint32_t sum = 0;
    uint16_t differ;
    for(uint16_t i = 0; i < n; i++)
    {
        if(arr[i] < average)
        {
            sum += average - arr[i];
        }
        else
        {
            sum += arr[i] - average;
        }
    }
    differ = sum/n;
    return differ;
}
/*****************************************************************
@功能	uart发送多字节数据
@参数	UARTx - UART端口，UART1,UART2,UART3,UART4
@参数	len - 数据长度
@参数	pdata - 数据地址
@返回	无
******************************************************************/
void UART_SendBytes(UART_TypeDef *UARTx, uint8_t *pdata, uint32_t len)
{
    while(len--)
    {
        UARTx->DR = *pdata++; 
        //while (UARTx->FR & UART_FR_TXFF);          
        while (!(UARTx->FR & UART_FR_TXFE));
    }
}

static void TKEY_DebugDataPrintf(uint16_t channelId,uint16_t rawdata,uint16_t baseline)
{

    uint8_t gucSendBuff[5];
    
    gucSendBuff[0] = (uint8_t)channelId;

        gucSendBuff[1] = (uint8_t)(rawdata >> 8);
        gucSendBuff[2] = (uint8_t)rawdata;

        gucSendBuff[3] = (uint8_t)(baseline >> 8);
        gucSendBuff[4] = (uint8_t)baseline;

    checkSum ^= gucSendBuff[0] ^ gucSendBuff[1]^gucSendBuff[2]^gucSendBuff[3]^gucSendBuff[4];
#ifdef TKEY_WAVEFORM_PRINTF    
    UART_SendBytes(USART1, gucSendBuff, 5);  
#endif    
}

void Reset_Predict_Touch_Delta(TKEY_HandleTypeDef* htkey)
{
    uint8_t ucI;
	for(ucI = 0; htkey->ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
		htkey->ChannelData[ucI].Delta_Sum = htkey->ChannelData[ucI].Tkey_RefPara->DetectInTH << htkey->ChannelData[ucI].Delta_Sum_Cnt;
		htkey->ChannelData[ucI].Predict_Touch_Delta  = htkey->ChannelData[ucI].Tkey_RefPara->DetectInTH;
	}
}

/************************************************************************
 * function   : TKEY_ScanProcess
 * Description: TKEY Scan Result main process. 
 * input      : htkey : TKEY handle
 * return     : None.
 ************************************************************************/
void TKEY_ScanProcess(TKEY_HandleTypeDef* htkey)
{
    uint8_t ucI, i, detect_index;
    int32_t Time_Differ = 0;
    uint32_t u32RegTemp;  //add
    uint8_t gucHeadBuff[3];
    uint8_t gucSendBuff[1];
    checkSum = 0;
    TKEY_ChannelDataDef *ChannelData;
     
    uint8_t max_ucI = 0;
    uint8_t max_delta_ucI = 0;
    int16_t max_delta = 0;

    u32RegTemp = TKEY->CXENR;

#ifdef TKEY_WAVEFORM_PRINTF
    gucHeadBuff[0] = 0xAA;
    gucHeadBuff[1] = 0x55;
    uint8_t tk_amount = TOTAL_CHANNELS;
    gucHeadBuff[2] = (uint8_t)(tk_amount * 5 + 1);
    UART_SendBytes(USART1, gucHeadBuff, 3);
#endif
    
    htkey->ExistDetecting = 0;
    
    /* Find the index with the highest Delta value */
    for (ucI = 0; htkey->ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
        
        htkey->ChannelData[ucI].Tkey_Data->Delta = (int32_t)htkey->ChannelData[ucI].Tkey_Data->BaseLine - (int32_t)htkey->ChannelData[ucI].Tkey_Data->RawData;

        ChannelData = &htkey->ChannelData[ucI];
        
        if (ChannelData->Tkey_Data->Delta > max_delta)
        {
            max_delta = ChannelData->Tkey_Data->Delta;
            max_delta_ucI = ucI;
        }
    }

    for (ucI = 0; htkey->ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
        
        ChannelData = &htkey->ChannelData[ucI];

#ifdef TKEY_WAVEFORM_PRINTF

        TKEY_DebugDataPrintf(htkey->ChannelData[ucI].ChannelId, htkey->ChannelData[ucI].Tkey_Data->RawData, htkey->ChannelData[ucI].Tkey_Data->BaseLine);
#endif

        switch (ChannelData->Tkey_Data->StateId)
        {
            case TKEY_STATEID_RELEASE:
            {
                htkey->PressRelaeseFlag &= ~(1 << ChannelData->ChannelId);
                htkey->LongPressFlag &= ~(1 << ChannelData->ChannelId);
                htkey->DetectingFlag &= ~(1 << ChannelData->ChannelId);
                ChannelData->Tkey_CalData->DetectingTimer = 0;
                //printf("ucI=%d\n,Delta = %d\n",ucI,htkey->ChannelData[ucI].Tkey_Data->Delta);
                if (ChannelData->Tkey_Data->Delta > ChannelData->Tkey_RefPara->DetectInTH)
                {
                
                    /* Only one button is allowed to be in the TKEY_STATEID_DETECTING state to prevent multi key triggering */
                    for (detect_index = 0; htkey->ChannelData[detect_index].ChannelId != 0xFFFF; detect_index++)
                    {
                        if (htkey->ChannelData[detect_index].Tkey_Data->StateId == TKEY_STATEID_DETECTING)
                        {
                            htkey->ExistDetecting = 1;
                            break;
                        }
                    }
                
                    if(!htkey->ExistDetecting)
                    {
                        /* Only allow the channel with the maximum Delta update threshold */
                        if ((ChannelData->Tkey_Data->Delta > (ChannelData->Predict_Touch_Delta * 3 / 5)) && (ucI == max_delta_ucI))
                        {
                            ChannelData->Delta_Sum = ChannelData->Delta_Sum - ChannelData->Predict_Touch_Delta + ChannelData->Tkey_Data->Delta;
                            ChannelData->Predict_Touch_Delta = ChannelData->Delta_Sum >> ChannelData->Delta_Sum_Cnt;
                        }
                
                        /* Only allow the channel with the largest Delta to trigger */
                        if ( (ChannelData->Tkey_Data->Delta > (ChannelData->Predict_Touch_Delta * 4 / 5)))
                        {

                            if (ChannelData->Tkey_Data->DebIn == 0)
                            {
                                /* When triggered by a button, turn off baseline updates for all channels */
                                for (int index = 0; htkey->ChannelData[index].ChannelId != 0xFFFF; index++)
                                {
                                    htkey->ChannelData[index].Tkey_CalData->pos = 0;
                                    htkey->ChannelData[index].Tkey_CalData->ScanTimer = 0;
                                }
                        
                                if (ucI == max_delta_ucI)
                                {
                                     
                                    if (htkey->DetectingFlag == 0)
                                    {

                                        htkey->PressRelaeseFlag = (1 << ChannelData->ChannelId);
                                        htkey->ChannelValue = ChannelData->ChannelId;
                                        htkey->ChannelDetectedNum = 1;
                                        ChannelData->Tkey_Data->StateId = TKEY_STATEID_DETECTING;
                                        ChannelData->Tkey_CalData->DetectingTimer = HAL_GetTick();
                                        ChannelData->Tkey_Data->DebOut = TKEY_DEBOUT;                          
                                    }
                                }

                            }
                            else
                            {
                                ChannelData->Tkey_Data->DebIn--;
                            }
                        }
                        // add
                        else
                        {
                            if (ChannelData->Tkey_CalData->ScanTimer++ >= TKEY_CALIBRATE_TIMES)
                            {
                                ChannelData->Tkey_CalData->ScanTimer = 0;

                                ChannelData->Tkey_CalData->Caldata[ChannelData->Tkey_CalData->pos++] = ChannelData->Tkey_Data->RawData;

                                if (ChannelData->Tkey_CalData->pos >= TKEY_CALIBRATE_BUFF_MAX)
                                {
                                    ChannelData->Tkey_CalData->pos = 0;
                                    ChannelData->Tkey_CalData->CalFlag = TKEY_CALIBRAT_SELF; //need self calibrate
                                }
                            }
                            /* Count when the button is not pressed, and reset to the initial 
                                threshold when the RESET_THRESHOLD_CNT count continues 
                            */
                            //if (g_u32Exti_Irq_line == 22)
                            if (1)
                            {
                                reset_cnt++;
                                if (reset_cnt == RESET_THRESHOLD_CNT)
                                {                       
                                    Reset_Predict_Touch_Delta(htkey);
                                    reset_cnt = 0;
                                    //g_u32Exti_Irq_line = 0;

                                }
                            }
                        }
                    }
                
                }
                else
                {
                    // printf("Delta = %d\n",htkey->ChannelData[ucI].Tkey_Data->Delta);
                    ChannelData->Tkey_Data->StateId = TKEY_STATEID_RELEASE;
                    ChannelData->Tkey_Data->DebIn = TKEY_DEBIN;

                    if (ChannelData->Tkey_CalData->ScanTimer++ >= TKEY_CALIBRATE_TIMES)
                    {
                        ChannelData->Tkey_CalData->ScanTimer = 0;

                        /* Resolve the issue of too many negative Delta values and slow baseline recovery */
                        if (ChannelData->Tkey_Data->Delta < -(ChannelData->Tkey_RefPara->DetectInTH * 10))
                        {

                            ChannelData->Tkey_CalData->CalFlag = TKEY_CALIBRAT_QUICK; // need quick calibrate
                        }
                        else if (ChannelData->Tkey_Data->Delta < -ChannelData->Tkey_RefPara->DetectInTH)
                        {
                            ChannelData->Tkey_CalData->Caldata[ChannelData->Tkey_CalData->pos++] = ChannelData->Tkey_Data->BaseLine + ChannelData->Tkey_RefPara->DetectInTH;
                        }
                        else
                        {
                            ChannelData->Tkey_CalData->Caldata[ChannelData->Tkey_CalData->pos++] = ChannelData->Tkey_Data->RawData;
                        }

                        if (ChannelData->Tkey_CalData->pos >= TKEY_CALIBRATE_BUFF_MAX)
                        {
                            ChannelData->Tkey_CalData->pos = 0;
                            ChannelData->Tkey_CalData->CalFlag = TKEY_CALIBRAT_SELF;
                        }
                    }
                }
            }break;

            case TKEY_STATEID_DETECTING:
            {

                htkey->DetectingFlag |= (1 << ChannelData->ChannelId);
                if (ChannelData->Tkey_Data->Delta < (ChannelData->Tkey_RefPara->DetectInTH * 3 / 5))
                //if (ChannelData->Tkey_Data->Delta < ChannelData->Predict_Touch_Delta * 3 / 5)
                {
                    if (ChannelData->Tkey_Data->DebOut == 0)
                    {
                        ChannelData->Tkey_Data->StateId = TKEY_STATEID_RELEASE;
                        ChannelData->Tkey_Data->DebIn = TKEY_DEBIN;
                        ChannelData->Tkey_CalData->DetectingTimer = 0;
                        htkey->PressRelaeseFlag &= ~(1 << ChannelData->ChannelId);
                        htkey->LongPressFlag &= ~(1 << ChannelData->ChannelId);
                        htkey->DetectingFlag &= ~(1 << ChannelData->ChannelId);
                    }
                    else
                    {
                        ChannelData->Tkey_Data->DebOut--;
                    }
                }
                else
                {
                #ifdef MULTI_KEY_DIFF_ROUND
                    if (htkey->PressRelaeseFlag & (~htkey->DetectingFlag))
                    {
                        htkey->PressRelaeseFlag = htkey->DetectingFlag;
                        htkey->ChannelValue = 0;
                        htkey->ChannelDetectedNum = 0;
                        }
                #endif
                    Time_Differ = HAL_GetTick() - ChannelData->Tkey_CalData->DetectingTimer;
                    if (Time_Differ > TKEY_DETECTING_CALIBRATE_TIME) // If the button is not released for a long time, it will trigger rapid self calibration 
                    {
                        /* Forced calibration */
                        for (int index = 0; htkey->ChannelData[index].ChannelId != 0xFFFF; index++)
                        {
                            htkey->ChannelData[index].Tkey_CalData->DetectingTimer = 0;
                            /* Adjust BaseLine to the current Rawdata value */
                            htkey->ChannelData[index].Tkey_Data->BaseLine =  htkey->ChannelData[index].Tkey_Data->RawData;
                        }
                    }
                    else if (Time_Differ > TKEY_LONG_DETECTING_TIME)
                    {

                        if ((htkey->PressRelaeseFlag) & (1 << ChannelData->ChannelId)) // add by sam 20220617
                        {
                            htkey->LongPressFlag |= (1 << ChannelData->ChannelId);
                        }
                    }
                }
            }break;
            default:
            break;
        }
    }

#ifdef TKEY_WAVEFORM_PRINTF
    gucSendBuff[0] = checkSum;
    UART_SendBytes(USART1, gucSendBuff, 1);
#endif
}


