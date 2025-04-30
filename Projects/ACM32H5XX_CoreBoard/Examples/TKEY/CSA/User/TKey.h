
#ifndef __TKEY_H__
#define __TKEY_H__

#include "acm32h5xx_hal_conf.h"

/*
通过设置'TKEY_COVER_PLATE_THICKNESS'的值来初始化不同厚度盖板的参数
------------------------------------------
#define TKEY_COVER_PLATE_THICKNESS <value>
------------------------------------------
0-3mm -> <value> = 3
5mm-> <value> = 5
8mm-> <value> = 8
------------------------------------------
*/

#define TKEY_COVER_PLATE_THICKNESS 5

//#define TKEY_WAVEFORM_PRINTF

#define TKEY_FIFO_ENABLE   //键值装入fifo使能


/*信息输出*/
//#define TKEY_DEBUG_ON         1

#define TKEY_DEBUG(fmt,arg...)          do{\
                                            if(TKEY_DEBUG_ON)\
                                                printf("\r\n<<-TKey.c->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

                                          
//#define MULTI_KEY_SHIELD_OUTPUT_MAX   //多通道屏蔽，同一轮同时触发只输出变化量最大一个
//#define MULTI_KEY_SHIELD_OUT_NONE     //多通道屏蔽，同一轮同时触发不输出

#define NO_KEY_DOWN             0xFF
#define OTHER_KEY_DOWN          0x55
#define KEY_DOWN                0x00

#define TKEY_CSDISCHARGETIME        4
#define TKEY_SW1H                   4
#define TKEY_SW1L                   4
#define TKEY_CR_CREN_SELECT         TKEY_CRSELECT_DISABLE   //通道补偿
#define TKEY_CR_VKEY_SELECT         TKEY_CR_VKEYSEL_2V5     //OPA 或 LDO 输出电压             
#define TKEY_CR_VREF_SELECT         TKEY_CR_VREFSEL_1V0     //内部基准电压 
#define TKEY_CR_SPREAD_SELECT       TKEY_CR_SPREAD_ENABLE   //扫描时钟扩频 
#define TKEY_CR_RANDM_SELECT        TKEY_SOFR_RANDM_1234    //扫描时钟抖动控制

//#define TKEY_SCAN_CLK_RC4M          //TKEY 扫描时钟为RC4M

#define TKEY_SLEEP_SCAN_SPEED       100   //睡眠时TKey扫描速度

//#define TKEY_DEBIN                  1  //按下滤波次数
//#define TKEY_DEBOUT                 1  //弹起滤波次数

#define TKEY_DEBIN                  0  //按下滤波次数
#define TKEY_DEBOUT                 0  //弹起滤波次数

#define RESET_THRESHOLD_CNT         5

#define TKEY_RELAESE_THRATIO        80    //释放比列系数，用于计算释放阈值，按下阈值DetectInTH的80%
#define TKEY_CALIBRATE_RATIO1       25    //校验比列系数，用于计算校验阈值，按下阈值DetectInTH的30%
#define TKEY_CALIBRATE_RATIO2       40    //校验比列系数，用于计算校验阈值，按下阈值DetectInTH的40%
#define TKEY_CALIBRATE_RATIO3       50    //校验比列系数，用于计算校验阈值，按下阈值DetectInTH的50%
#define TKEY_CALIBRATE_THMAX        15    //校验阈值最大值
#define TKEY_CALIBRATE_THMIN        8     //校验阈值最小值
#define TKEY_CALIBRATE_TIMES        1     //采样次数，达到该次数后保存采样结果
#define TKEY_CALIBRATE_BUFF_MAX     20    //采样结果保存次数

#define TKEY_SILENT_TIME                        100     //单位ms 静默时间

#define TKEY_LONG_DETECTING_TIME                2000    //单位ms，按键长按时间，超过该时间未释放就表示长按操作，注意该值应该小于TKEY_DETECTING_CALIBRATE_TIME

#define TKEY_DETECTING_CALIBRATE_TIME           5000   //单位ms，按键长按校准时间，超过该时间未释放就会触发快速自校准，注意该值应该大于TKEY_LONG_DETECTING_TIME

 

#define TKEY_CHANNEL_0                  (0U)       
#define TKEY_CHANNEL_1                  (1U)       
#define TKEY_CHANNEL_2                  (2U)       
#define TKEY_CHANNEL_3                  (3U)       
#define TKEY_CHANNEL_4                  (4U)       
#define TKEY_CHANNEL_5                  (5U)       
#define TKEY_CHANNEL_6                  (6U)       
#define TKEY_CHANNEL_7                  (7U)       
#define TKEY_CHANNEL_8                  (8U)       
#define TKEY_CHANNEL_9                  (9U)       
#define TKEY_CHANNEL_10                 (10U)       
#define TKEY_CHANNEL_11                 (11U)       
#define TKEY_CHANNEL_12                 (12U)       
#define TKEY_CHANNEL_13                 (13U)       
#define TKEY_CHANNEL_14                 (14U)       
#define TKEY_CHANNEL_15                 (15U)  
#define TKEY_CHANNEL_NULL               (0xFFFF) 
 
/****************  TKEY定时扫描相关配置  *****************/
#define TKEY_TIM_SCAN_ENABLE    //定时扫描TKEY使能
#ifdef  TKEY_TIM_SCAN_ENABLE
    #define TKEY_SCAN_TIMX                      TIM17 
    #define TKEY_SCAN_TIMX_IRQn                 TIM17_IRQn 
    #define TKEY_SCAN_TIMX_IRQHandler           TIM17_IRQHandler  
    #define TKEY_SCAN_TIMX_IRQn_PRIORITY        1      //中断优先级
    #define TKEY_SCAN_TIM_PERIOD                5      //TKEY扫描定时器周期(ms)
#endif
/*******************************************************/


#define TKEY_QUICK_QUICK_CALIBRATE_TIMES    10    //快速校准次数

/******************************************************************************
*  扫描等待时间:值越大,按键响应速度变慢，最大为TKEY_SCAN_WAIT_MAX_TIMES; 值过小会导致获取不到RawData值。
*******************************************************************************/
//#define TKEY_SCAN_WAIT_TIMES              10 
//#define TKEY_SCAN_WAIT_MAX_TIMES          100  
/******************************************************************************
*  扫描等待时间的自加系数:值越大,自加越小,自加速度越慢,校准时间增加;
*  值越小,自加值越大,自加速度越快，校准时间减小;
*******************************************************************************/
//#define TKEY_SCAN_WAIT_TIMES_RATIO        10  //20% 

/******************************************************************************
*  RawData上下波动范围系数:值越大,波动范围越小,BsaeLine值越接近RawData,校准越准确;
*  值过大会导致获取不到BsaeLine值,校准不过。最小值为TKEY_RAWDATA_RANGE_MIN_RATIO
*  波动范围[-RawData/TKEY_DELTA_DIVISOR, +RawData/TKEY_DELTA_DIVISOR] 
*******************************************************************************/
#define TKEY_RAWDATA_RANGE_RATIO          800
#define TKEY_RAWDATA_RANGE_MIN_RATIO      200  
/******************************************************************************
*  RawData上下波动范围系数的自减系数:值越大,自减越小,自减速度越慢,校准时间增加;
*  值越小,自减值越大,自减速度越快，校准时间减小;
*******************************************************************************/
#define TKEY_RAWDATA_RANGE_DEC_RATIO      10  //10% 


/******************************************************************************
*  TKey 16通道使能宏定义
*  需要使用哪路按键就使能该通道;未使用的按键通道不要使能,不然校准会失败
*******************************************************************************/
#define TKEY_CHANNEL_0_ENABLE        //--对应芯片管脚TVCX0, 上位机工具TK_0               
#define TKEY_CHANNEL_1_ENABLE           //--对应芯片管脚TVCX1, 上位机工具TK_1	               
#define TKEY_CHANNEL_2_ENABLE           //--对应芯片管脚TVCX2, 上位机工具TK_2                
#define TKEY_CHANNEL_3_ENABLE        //--对应芯片管脚TVCX3, 上位机工具TK_3                  
#define TKEY_CHANNEL_4_ENABLE           //--对应芯片管脚TVCX4, 上位机工具TK_4                
#define TKEY_CHANNEL_5_ENABLE           //--对应芯片管脚TVCX5, 上位机工具TK_5              
#define TKEY_CHANNEL_6_ENABLE        //--对应芯片管脚TVCX6, 上位机工具TK_6                 
#define TKEY_CHANNEL_7_ENABLE        //--对应芯片管脚TVCX7, 上位机工具TK_7                 
#define TKEY_CHANNEL_8_ENABLE           //--对应芯片管脚TVCX8, 上位机工具TK_8               
#define TKEY_CHANNEL_9_ENABLE           //--对应芯片管脚TVCX9, 上位机工具TK_9               
#define TKEY_CHANNEL_10_ENABLE          //--对应芯片管脚TVCX10, 上位机工具TK_10              
#define TKEY_CHANNEL_11_ENABLE          //--对应芯片管脚TVCX11, 上位机工具TK_11

/******************************************************************************
*  特别注意事项
*  仅当PB3做TK功能时，PB0和TK12（PB3）有冲突，PB0不能使用，需保持悬空。PB3做其它功能时，PB3和PB0无冲突。
*******************************************************************************/                  
#define TKEY_CHANNEL_12_ENABLE          //--对应芯片管脚TVCX12, 上位机工具TK_12        
#define TKEY_CHANNEL_13_ENABLE          //--对应芯片管脚TVCX13, 上位机工具TK_13              
#define TKEY_CHANNEL_14_ENABLE          //--对应芯片管脚TVCX14, 上位机工具TK_14               
#define TKEY_CHANNEL_15_ENABLE          //--对应芯片管脚TVCX15, 上位机工具TK_15            

/******************************************************************************
*  TKey 16通道工作模式灵敏度
*  值越小,按键灵敏度越高;值越大,按键灵敏度越低.未使能的按键通道灵敏度设为4095.
*******************************************************************************/
#define TKEY_0_SENSITIVITY            150    //--对应芯片管脚TVCX0, 上位机工具TK_0
#define TKEY_1_SENSITIVITY            150	  //--对应芯片管脚TVCX1, 上位机工具TK_1		
#define TKEY_2_SENSITIVITY            150      //--对应芯片管脚TVCX2, 上位机工具TK_2 
#define TKEY_3_SENSITIVITY            4095    //--对应芯片管脚TVCX3, 上位机工具TK_3 
#define TKEY_4_SENSITIVITY            150      //--对应芯片管脚TVCX4, 上位机工具TK_4  
#define TKEY_5_SENSITIVITY            150      //--对应芯片管脚TVCX5, 上位机工具TK_5
#define TKEY_6_SENSITIVITY            4095    //--对应芯片管脚TVCX6, 上位机工具TK_6 
#define TKEY_7_SENSITIVITY            150    //--对应芯片管脚TVCX7, 上位机工具TK_7
#define TKEY_8_SENSITIVITY            150      //--对应芯片管脚TVCX8, 上位机工具TK_8 
#define TKEY_9_SENSITIVITY            150      //--对应芯片管脚TVCX9, 上位机工具TK_9
#define TKEY_10_SENSITIVITY           150      //--对应芯片管脚TVCX10, 上位机工具TK_10 
#define TKEY_11_SENSITIVITY           150      //--对应芯片管脚TVCX11, 上位机工具TK_11
#define TKEY_12_SENSITIVITY           150      //--对应芯片管脚TVCX12, 上位机工具TK_12
#define TKEY_13_SENSITIVITY           150      //--对应芯片管脚TVCX13, 上位机工具TK_13
#define TKEY_14_SENSITIVITY           150      //--对应芯片管脚TVCX14, 上位机工具TK_14
#define TKEY_15_SENSITIVITY           150      //--对应芯片管脚TVCX15, 上位机工具TK_15 


/******************************************************************************
*  TKey 16通道睡眠模式灵敏度
*  值越小,按键灵敏度越高;值越大,按键灵敏度越低.未使能的按键通道灵敏度设为4095.
*******************************************************************************/
#define TKEY_0_SLEEP_SENSITIVITY           (TKEY_0_SENSITIVITY+10)    //--对应芯片管脚TVCX0, 上位机工具TK_0
#define TKEY_1_SLEEP_SENSITIVITY           (TKEY_1_SENSITIVITY+10)       //--对应芯片管脚TVCX1, 上位机工具TK_1
#define TKEY_2_SLEEP_SENSITIVITY           (TKEY_2_SENSITIVITY+10)       //--对应芯片管脚TVCX2, 上位机工具TK_2 
#define TKEY_3_SLEEP_SENSITIVITY           (TKEY_3_SENSITIVITY+10)     //--对应芯片管脚TVCX3, 上位机工具TK_3  
#define TKEY_4_SLEEP_SENSITIVITY           (TKEY_4_SENSITIVITY+10)       //--对应芯片管脚TVCX4, 上位机工具TK_4 
#define TKEY_5_SLEEP_SENSITIVITY           (TKEY_5_SENSITIVITY+10)       //--对应芯片管脚TVCX5, 上位机工具TK_5  
#define TKEY_6_SLEEP_SENSITIVITY           (TKEY_6_SENSITIVITY+10)     //--对应芯片管脚TVCX6, 上位机工具TK_6  
#define TKEY_7_SLEEP_SENSITIVITY           (TKEY_7_SENSITIVITY+10)     //--对应芯片管脚TVCX7, 上位机工具TK_7  
#define TKEY_8_SLEEP_SENSITIVITY           (TKEY_8_SENSITIVITY+10)       //--对应芯片管脚TVCX8, 上位机工具TK_8  
#define TKEY_9_SLEEP_SENSITIVITY           (TKEY_9_SENSITIVITY+10)      //--对应芯片管脚TVCX9, 上位机工具TK_9  
#define TKEY_10_SLEEP_SENSITIVITY          (TKEY_10_SENSITIVITY+10)       //--对应芯片管脚TVCX10, 上位机工具TK_10 
#define TKEY_11_SLEEP_SENSITIVITY          (TKEY_11_SENSITIVITY+10)       //--对应芯片管脚TVCX11, 上位机工具TK_11  
#define TKEY_12_SLEEP_SENSITIVITY          (TKEY_12_SENSITIVITY+10)       //--对应芯片管脚TVCX12, 上位机工具TK_12  
#define TKEY_13_SLEEP_SENSITIVITY          (TKEY_13_SENSITIVITY+10)       //--对应芯片管脚TVCX13, 上位机工具TK_13  
#define TKEY_14_SLEEP_SENSITIVITY          (TKEY_14_SENSITIVITY+10)       //--对应芯片管脚TVCX14, 上位机工具TK_14  
#define TKEY_15_SLEEP_SENSITIVITY          (TKEY_15_SENSITIVITY+10)       //--对应芯片管脚TVCX15, 上位机工具TK_15 


typedef enum{
#ifdef  TKEY_CHANNEL_0_ENABLE 
  TK_0,   //0
#endif
#ifdef  TKEY_CHANNEL_1_ENABLE   
  TK_1,   //1
#endif
#ifdef  TKEY_CHANNEL_2_ENABLE 
  TK_2,   //2
#endif
#ifdef  TKEY_CHANNEL_3_ENABLE   
  TK_3,   //3
#endif
#ifdef  TKEY_CHANNEL_4_ENABLE   
  TK_4,   //4
#endif
#ifdef  TKEY_CHANNEL_5_ENABLE   
  TK_5,   //5
#endif
#ifdef  TKEY_CHANNEL_6_ENABLE   
  TK_6,   //6
#endif
#ifdef  TKEY_CHANNEL_7_ENABLE   
  TK_7,   //7
#endif
#ifdef  TKEY_CHANNEL_8_ENABLE   
  TK_8,   //8
#endif
#ifdef  TKEY_CHANNEL_9_ENABLE   
  TK_9,   //9
#endif
#ifdef  TKEY_CHANNEL_10_ENABLE   
  TK_10,  //10
#endif  
#ifdef  TKEY_CHANNEL_11_ENABLE   
  TK_11,  //11
#endif
#ifdef  TKEY_CHANNEL_12_ENABLE  
  TK_12,  //12
#endif
#ifdef  TKEY_CHANNEL_13_ENABLE   
  TK_13,  //13
#endif
#ifdef  TKEY_CHANNEL_14_ENABLE   
  TK_14,  //14
#endif
#ifdef  TKEY_CHANNEL_15_ENABLE   
  TK_15,   //15
#endif  
  TOTAL_CHANNELS
}TK_Index_enum_T;

extern TKEY_ChannelDataDef TKEY_Config[];

void TKEY_Calibrate_ParaInit(void);

void TKEY_Timer_ScanInit(void);

void TKEY_Scan_FIFO_Clear(void);

void TKEY_Timer_Scan_Start(void);

void TKEY_Timer_Scan_Stop(void);

void TKEY_Init(void);

void TKEY_Suspend(void);

void TKEY_Resume(void);

void TKEY_Wakeup(void);

uint8_t TKEY_Quick_Calibrate(void);

void TKEY_Waveform_Output_Process(void);

void TKEY_Calibrate_Process(void);

void TKEY_RTC_Calibrate_Process(void);

uint8_t TouchKey_Init(void);

uint8_t TouchKey_TimerSacn_GetKeyVal(void);

void Bubble_Sort(uint16_t *arr, uint16_t n);

uint16_t Calculate_Sample_Average(uint16_t *arr, uint16_t n);

uint16_t Calculate_Sample_differ(uint16_t *arr, uint16_t n, uint16_t average);

uint16_t TouchKey_GetPressRelaeseFlag(void);

void TouchKey_ClearPressRelaeseFlag(void);

uint16_t TouchKey_GetLongPressFlag(void);

void TouchKey_Switch_Clock_RC4M(void);

void TouchKey_Switch_Clock_PCLK(void);

void TKEY_Press_Relaese_Process(void);

void HAL_TKEY_IRQHandler(void);

void TKEY_ScanProcess(TKEY_HandleTypeDef* htkey);

void TKEY_CSA_Init(void);

void TKEY_Suspend(void);

void TKEY_Resume(void);
void TKEY_CSA_Init_Single(void);
void read_channel_data_single_16_GND(TKEY_HandleTypeDef* htkey);
void read_channel_data_single_16(TKEY_HandleTypeDef* htkey);
void read_channel_data_single_16_compensate(TKEY_HandleTypeDef* htkey);

#endif
