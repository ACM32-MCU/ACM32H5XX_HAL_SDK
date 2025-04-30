#ifndef CAN_LIB_H_
#define CAN_LIB_H_

/*
#define Platform_16Bit
#define Platform_32Bit
*/

#ifdef Platform_16Bit
    #define canlib_uint8_t  unsigned char
    #define canlib_uint16_t unsigned int
    #define canlib_uint32_t unsigned long
#else
    #define canlib_uint8_t  unsigned char
    #define canlib_uint16_t unsigned short
    #define canlib_uint32_t unsigned int
    #define canlib_uint64_t unsigned long long
#endif

#define CAN_RX_MSG_Block (( canlib_uint8_t )(24U / 4u))
#define CAN_TX_MSG_Block (( canlib_uint8_t )(32U / 4u))

typedef void (*MSG_CKB_ON_RECEIVE)(canlib_uint8_t CopyData []);
typedef void (*MSG_CKB_ON_PROCESS)(canlib_uint8_t CopyData []);
typedef void (*MSG_CKB_ON_LOST)(canlib_uint8_t CopyData []);
typedef void (*pfnMsgBufferSet)(canlib_uint8_t CopyData []);
typedef void (*pfnMsgSuccess)(void);

/**@enum CAN_FUNC_ERR_t
 * @brief 定义驱动上报应用消息类型 \n
 */
typedef enum
{
    CAN_ERR_OK,       /**< 模块初始化返回值,参数正常 */
    CAN_ERR_ParaInit, /**< 模块初始化返回值,初始化错误 */
    CAN_ERR_OUTRANGE, /**< 模块初始化返回值,参数传入超限制 */
} CAN_FUNC_ERR_t;

/**@enum CAN_TX_Status_t
 * @brief 定义CAN模块发送状态 \n
 */
typedef enum
{
    CAN_N_TX_Disable = 0x00U, /**< CAN模块禁止发送 */
    CAN_N_TX_Enable  = 0x55U, /**< CAN模块允许发送 */
} CAN_TX_Status_t;

/**@enum CAN_RX_Status_t
 * @brief 定义CAN模块接收状态 \n
 */
typedef enum
{
    CAN_N_RX_Enable  = 0x00U, /**< CAN模块禁止接收 */
    CAN_N_RX_Disable = 0x55U, /**< CAN模块允许接收 */
} CAN_RX_Status_t;

/**@struct st_CANMsgAttribute
 * @brief 报文接收属性结构体 \n
 * 定义报接收文属性
 */
typedef struct
{
    canlib_uint8_t     Init_Aarry [ 8u ];    /**< 报文初始化值.默认加载,未收到报文时,buffer内为该值 */
    canlib_uint8_t     TimeOut_Array [ 8u ]; /**< 报文掉线值.掉线后加载 */
    canlib_uint8_t     Mask_Array [ 8u ];    /**< 报文掉线掩码.哪一位要是用掉线值就置位 */
    canlib_uint32_t    MSG_TimeOut_Val;      /**< 报文掉线时间,单位ms */
    canlib_uint32_t    MSG_Identifier;       /**< 报文ID值 */
    MSG_CKB_ON_RECEIVE Msg_ReceiveCBK;       /**< 报文中断接收回调函数,默认NULL,该回调函数处于接收中断内. */
    MSG_CKB_ON_PROCESS Msg_ProcessCBK;       /**< 报文逻辑接收回调函数,默认NULL,该回调函数处逻辑函数内 */
    MSG_CKB_ON_LOST    Msg_LostCBK;          /**< 报文逻辑掉线回调函数,默认NULL,该回调函掉线后会被执行一次 */
} st_CANMsgAttribute;

/**@struct st_CAN_SendAttribute
 * @brief 报文发送属性结构体 \n
 * 定义发送报文属性
 */
typedef struct
{
    canlib_uint32_t u32MsgID;          /**< 报文发送ID */
    canlib_uint32_t u32MsgCycle;       /**< 报文发送周期,单位是us */
    canlib_uint32_t u32MsgCycleOffset; /**< 报文发送周期偏移量,定时器计时初值 */
    canlib_uint32_t u32InitSendCnt;    /**< 报文发送初始次数 */
    canlib_uint8_t  u8MsgSTD_EXT;      /**< 报文类型是标准帧还是扩展帧  标准帧0 扩展帧1 */
    canlib_uint8_t  u8MsgPro;          /**< 报文发送优先级,使用哪个buffer发送 */
    canlib_uint8_t  u8MsgDLC;          /**< 报文发送的长度,默认8 */
    canlib_uint8_t  u8TXMsgType;       /**< 报文发送的类型,周期帧,事件帧,周期事件帧 */
    pfnMsgBufferSet MsgBufferSet;      /**< 报文发送buffer填充函数,报文发送前会调用此函数从外部加载发送值 */
    pfnMsgSuccess   MsgSuccess;        /**< 报文发送成功回调函数 */
} st_CAN_SendAttribute;
/**@struct st_CAN_Msg
 * @brief 报文发送结构体 \n
 * 定义报文发送结构的信息
 */
typedef struct
{
    canlib_uint32_t MsgID;     /**< 报文发送ID */
    canlib_uint8_t  MsgStd;    /**< 报文发送标准帧/扩展帧 */
    canlib_uint8_t  MsgRTR;    /**< 报文发送数据帧/远程帧 默认0 */
    canlib_uint8_t  MsgDLC;    /**< 报文发送长度 */
    canlib_uint8_t  MsgPro;    /**< 报文发送优先级,对应使用buffer */
    canlib_uint8_t  Msg [ 8 ]; /**< 报文发送数据buffer */
} st_CAN_Msg;

typedef canlib_uint8_t (*HAL_CAN_Send)(st_CAN_Msg *Msg);
/**@struct st_CANMsgStruct
 * @brief 内部使用 \n
 * 内容不明
 */
typedef struct
{
    canlib_uint8_t          AnalyzeMsg [ 8u ];
    canlib_uint8_t          ReceiveBuf [ 8u ];
    volatile canlib_uint8_t CAN_Busy;
    volatile canlib_uint8_t CAN_Received;
    canlib_uint8_t          CAN_Status;
    canlib_uint8_t          CAN_Reverse;
    canlib_uint32_t         CAN_Lost;
} st_CANMsgStruct;
/**@struct st_CanMsgOp
 * @brief 内部使用,不做讲解 \n
 * 内部使用,不做讲解
 */
typedef struct
{
    st_CANMsgStruct          *CAN_MSG_Analysis;
    const st_CANMsgAttribute *pAttrubute;
    canlib_uint16_t           Total_Msg;
    canlib_uint8_t            u8CAN_RX_ENABLE;
} st_CanMsgOp;
/**@struct st_CAN_SendOperation
 * @brief 内部使用,不做讲解 \n
 * 内部使用,不做讲解
 */
typedef struct
{
    canlib_uint32_t u32MsgID;
    canlib_uint32_t u32Timer;
    canlib_uint32_t u32MsgCycle;
    canlib_uint32_t u32SendCnt;
    canlib_uint8_t  u8MsgSTD_EXT;
    canlib_uint8_t  u8MsgPro;
    canlib_uint8_t  u8TXMsgType;
    canlib_uint8_t  u8MsgRet;
    canlib_uint8_t  u8MsgBuff [ 8u ];
    canlib_uint16_t u16MsgDLC;
    canlib_uint16_t u16TXMsgDIMcounter;
} st_CAN_SendOperation;
/**@struct st_CanMsgTxOp
 * @brief 内部使用,不做讲解 \n
 * 内部使用,不做讲解
 */
typedef struct
{
    const st_CAN_SendAttribute *pCAN_SendAttribute;
    st_CAN_SendOperation       *CanMsg;
    canlib_uint16_t             Total_Msg;
    HAL_CAN_Send                Can_Write;
    canlib_uint8_t              u8CAN_TX_ENABLE;
} st_CanMsgTxOp;

extern void            CAN_RX_SetEnable(st_CanMsgOp *CanRXMsgOp, CAN_RX_Status_t RX_enable);
extern CAN_RX_Status_t CAN_RX_ReadEnableSts(st_CanMsgOp *CanRXMsgOp);

extern void            CAN_TX_SetEnable(st_CanMsgTxOp *CanMsgTxOp, CAN_TX_Status_t TX_enable);
extern CAN_TX_Status_t CAN_TX_ReadEnableSts(st_CanMsgTxOp *CanMsgTxOp);

extern void           Can_Write_Fun(st_CanMsgTxOp *CanMsgTxOp, canlib_uint32_t u32CbkCycle);
extern CAN_FUNC_ERR_t Can_Msg_Tx_Overwrite(st_CanMsgTxOp *CanMsgTxOp, canlib_uint32_t u32TxMsgIdx, canlib_uint16_t u16TxMSgDIMcounter, canlib_uint32_t u32TxMsgCycle);
extern CAN_FUNC_ERR_t Can_TX_BuffInit(st_CanMsgTxOp *CanMsgTxOp, const st_CAN_SendAttribute *Array, canlib_uint16_t length, HAL_CAN_Send pfunc);
extern void           Can_Msg_TX_SUCCESS(st_CanMsgTxOp *CanMsgTxOp, canlib_uint32_t u32TxMsgIdx);
extern CAN_FUNC_ERR_t Can_Msg_TX_FastInitCycle(st_CanMsgTxOp *CanMsgTxOp, canlib_uint32_t u32TxMsgIdx, canlib_uint32_t CurTimerCnt);

extern void            Can_RX_BuffInit(st_CanMsgOp *CanRXMsgOp, const st_CANMsgAttribute *array, canlib_uint16_t length);
extern void            CanMSg_XMS_Analysis(st_CanMsgOp *CanRXMsgOp, canlib_uint16_t CbkCycle);
extern void            Co_Can_Buff_Set(st_CanMsgOp *CanRXMsgOp, canlib_uint16_t MsgID, canlib_uint8_t m_DLC, canlib_uint8_t Msg []);
extern canlib_uint8_t  CAN_MSG_Status(st_CanMsgOp *CanRXMsgOp, canlib_uint16_t MsgID);
extern canlib_uint8_t  CAN_MSG_Read(st_CanMsgOp *CanRXMsgOp, canlib_uint16_t MsgIdx, canlib_uint8_t BuffIdx);
extern void            Can_RX_ResetAllTimer(st_CanMsgOp *CanRXMsgOp);
extern void            Can_RX_ResetTimer(st_CanMsgOp *CanRXMsgOp, canlib_uint16_t idx);
extern canlib_uint32_t CAN_RX_Get_TimeStamp(st_CanMsgOp *CanRXMsgOp, canlib_uint16_t MsgIdx);
extern canlib_uint8_t  CAN_MSG_ChangeStatusDefault(st_CanMsgOp *CanRXMsgOp, canlib_uint16_t MsgID);

extern canlib_uint32_t Read_CAN_RX_Version(void);
extern canlib_uint32_t Read_CAN_TX_Version(void);

extern void CAN_TX_DiagCtrlEnable(st_CanMsgTxOp *CanMsgTxOp, CAN_TX_Status_t TX_enable);
extern void CAN_TX_DiagCtrlRelease(st_CanMsgTxOp *CanMsgTxOp);

extern void CAN_RX_DiagCtrlEnable(st_CanMsgOp *CanRXMsgOp, CAN_RX_Status_t RX_enable);
extern void CAN_RX_DiagCtrlRelease(st_CanMsgOp *CanRXMsgOp);

#endif
