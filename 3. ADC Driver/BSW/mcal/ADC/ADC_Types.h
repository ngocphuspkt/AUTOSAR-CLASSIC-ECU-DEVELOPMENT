/****************************************************** 
 * @brief   Header file ADC Types
 * @details Chứa các khai báo, định nghĩa kiểu dữ liệu 
 *          struct cấu hình thông số ADC được sử dụng 
 *          trong file ADC.h và ADC.c
 * @author  Trịnh Ngọc Phú - 2411
 * @date    2025
********************************************************/
#ifndef ADC_TYPES_H
#define ADC_TYPES_H
#include "Std_Types.h"
#include "stm32f10x_adc.h"
#include "stm32f10x.h"
typedef void (*Adc_NotificationCallbackType)(void);

/*******************************************************
 * Macro lấy ra kiểu lưu trữ dữ liệu sau khi chuyển đổi
 *******************************************************/
#define GET_DATA_ALIGNMENT(ALIGNMENT_MODE) ((ALIGNMENT_MODE) == ADC_ALIGN_RIGHT) ? ADC_DataAlign_Right : ADC_DataAlign_Left

/*******************************************************
 * Macro lấy ra kiểu trigger 
 *******************************************************/
#define GET_TRIGGER_SOURCE(MODE_TRIGGER)                        \
    (((MODE_TRIGGER) == ADC_TRIGG_SRC_SW) ? ADC_ExternalTrigConv_None : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_T1_CC1) ? ADC_ExternalTrigConv_T1_CC1 : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_T1_CC2) ? ADC_ExternalTrigConv_T1_CC2 : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_T2_CC1) ? ADC_ExternalTrigConv_T2_CC2 : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_T3_TRGO) ? ADC_ExternalTrigConv_T3_TRGO : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_T4_CC4) ? ADC_ExternalTrigConv_T4_CC4 : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_Ext_IT11_TIM8_TRGO) ? ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_T1_CC3) ? ADC_ExternalTrigConv_T1_CC3 : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_T3_CC1) ? ADC_ExternalTrigConv_T3_CC1 : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_T2_CC3) ? ADC_ExternalTrigConv_T2_CC3 : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_T8_CC1) ? ADC_ExternalTrigConv_T8_CC1 : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_T8_TRGO) ? ADC_ExternalTrigConv_T8_TRGO : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_T5_CC1) ? ADC_ExternalTrigConv_T5_CC1 : \
    ((MODE_TRIGGER) == ADC_TRIGG_SRC_HW_T5_CC3) ? ADC_ExternalTrigConv_T5_CC3 : \
    ADC_ExternalTrigConv_None)  
/********************************************************
 * @typedef Adc_ChannelType
 * @brief   Chứa các ID của tất cả các kênh ADC (16 kênh)
 ********************************************************/
typedef uint8 Adc_ChannelType;

/********************************************************
 * @typedef Adc_GroupType
 * @brief   Chứa các ID của các group ADC 
 ********************************************************/
typedef enum {
    GROUP_0,
    GROUP_1,
    GROUP_2,
    GROUP_3,
    GROUP_4,
    GROUP_5,
    GROUP_6,
    GROUP_7,
    GROUP_8,
    GROUP_9,
    GROUP_10,
    GROUP_11,
    GROUP_12,
    GROUP_13,
    GROUP_14,
    GROUP_15,
    ID_GroupAdc 
} Adc_GroupType;

/********************************************************
 * @typedef Adc_ValueGroupType
 * @brief   Chứa giá trị trả về sau khi chuyển đổi xong 
 ********************************************************/
typedef uint16 Adc_ValueGroupType;

/********************************************************
 * @typedef Adc_ConversionTimeType
 * @brief   thời gian chuyển đổi ADC
 ********************************************************/
typedef uint16 Adc_ConversionTimeType;

/********************************************************
 * @typedef Adc_SamplingTimeType
 * @brief   thời gian lấy mẫu
 ********************************************************/
typedef uint16 Adc_SamplingTimeType;

/********************************************************
 * @typedef Adc_InstanceTypde
 * @brief   Số lượng module ADC được sử dụng
 ********************************************************/
typedef enum 
{
    Adc_Instance_1 = 0x01,
    Adc_Instance_2
}Adc_InstanceTypde;

/********************************************************
 * @typedef Adc_StatusType
 * @brief   Trạng thái hiện tại của ADC
 * @details ADC_IDLE: đang rãnh, không làm gì
 *          ADC_BUSY: đang bận chuyển đổi
 *          ADC_COMPLETED: đã xong quá trình chuyển đổi
 *          ADC_STREAM_COMPLETED: sử dụng chế độ DMA
 ********************************************************/
typedef enum
{
    ADC_IDLE = 0x00,
    ADC_BUSY = 0x01,
    ADC_COMPLETED = 0x02,
    ADC_STREAM_COMPLETED = 0x03
}Adc_StatusType;

/**********************************************************
 * @typedef Adc_TriggerSourceType
 * @brief   Lựa chọn nguồn kích để kích hoạt chuyển đổi ADC
 * @details ADC_TRIGG_SRC_SW: kích bằng phần mềm
 *          ADC_TRIGG_SRC_HW: kích bằng sự kiện (interrupt)
 **********************************************************/
typedef enum
{
    ADC_TRIGG_SRC_SW = 0x00,
    ADC_TRIGG_SRC_HW_T1_CC1,
    ADC_TRIGG_SRC_HW_T1_CC2,
    ADC_TRIGG_SRC_HW_T2_CC1,
    ADC_TRIGG_SRC_HW_T3_TRGO,
    ADC_TRIGG_SRC_HW_T4_CC4,
    ADC_TRIGG_SRC_HW_Ext_IT11_TIM8_TRGO,
    ADC_TRIGG_SRC_HW_T1_CC3,
    ADC_TRIGG_SRC_HW_T3_CC1,
    ADC_TRIGG_SRC_HW_T2_CC3,
    ADC_TRIGG_SRC_HW_T8_CC1,
    ADC_TRIGG_SRC_HW_T8_TRGO,
    ADC_TRIGG_SRC_HW_T5_CC1,
    ADC_TRIGG_SRC_HW_T5_CC3
}Adc_TriggerSourceType;

/**********************************************************
 * @typedef Adc_GroupConvModeType
 * @brief   Chế độ hoạt của ADC
 * @details ADC_CONV_MODE_ONESHOT: sự chuyển đổi được thực 
 *          khi có một xung kích
 *          ADC_CONV_MODE_CONTINUOUS: việc chuyển đổi được
 *          thực hiện liên tục sau khi được kích lần đầu
 **********************************************************/
typedef enum 
{
    ADC_CONV_MODE_ONESHOT = 0x00,
    ADC_CONV_MODE_CONTINUOUS = 0x01
}Adc_GroupConvModeType;

/********************************************************
 * @typedef Adc_GroupPriorityType
 * @brief   Độ ưu tiên chuyển đổi của group. Phục vụ cho 
 *          kiểu Groupdef
 ********************************************************/
typedef uint8 Adc_GroupPriorityType;

/********************************************************
 * @typedef Adc_StreamNumSampleType
 * @brief   Số lượng group sử dụng chế độ DMA
 ********************************************************/
typedef uint8 Adc_StreamNumSampleType;

/**********************************************************
 * @typedef Adc_StreamBufferModeType
 * @brief   Chế độ lưu trữ của DMA
 * @details ADC_STREAM_BUFFER_LINEAR: DMA sẽ lưu đầy bộ nhớ
 *          xong rồi dừng. không lưu nữa
 *          ADC_STREAM_BUFFER_CIRCULAR: DMA sẽ lưu khi đầy 
 *          bộ nhớ và quay lại vị trí đầu tiên ghi đè lên 
 *          data cũ
 **********************************************************/
typedef enum 
{
    ADC_STREAM_BUFFER_LINEAR = 0x01,
    ADC_STREAM_BUFFER_CIRCULAR
}Adc_StreamBufferModeType;

/**********************************************************
 * @typedef Adc_GroupAccessModeType
 * @brief   kiểu truy cập
 * @details ADC_ACCESS_MODE_SINGLE: không sử dụng DMA
 *          ADC_ACCESS_MODE_STREAMING: sử dụng DMA
 **********************************************************/
typedef enum
{   
    ADC_ACCESS_MODE_SINGLE = 0x00, // không sử dụng DMA
    ADC_ACCESS_MODE_STREAMING // mode DMA
}Adc_GroupAccessModeType;

/**********************************************************
 * @typedef Adc_ResultAlignmentType
 * @brief   Kiểu lưu trữ data nhận được sau khi chuyển đổi
 * @details ADC_ALIGN_LEFT: data sẽ dịch về bên trái - dư
 *          4 bit có trọng số thấp
 *          ADC_ALIGN_RIGHT: data sẽ dịch về bên trái - dư
 *          4 bit có trọng số cao (sử dụng phổ biến)
 **********************************************************/
typedef enum 
{
    ADC_ALIGN_LEFT = 0x00,
    ADC_ALIGN_RIGHT
}Adc_ResultAlignmentType;

/**********************************************************
 * @typedef Adc_ChannelConfigType
 * @brief   Kiểu cấu trúc cấu hình cho từng Channel
 * @details Mỗi channel cần các thông số cơ bản như: 
 *          Channel: thuộc ADC channel nào
 *          SamplingTime: thời gian lấy mẫu
 *          Rank: độ ưu tiên
 **********************************************************/
typedef struct {
    Adc_ChannelType Channel;
    uint8_t SamplingTime;
    uint8_t Rank;
} Adc_ChannelConfigType;

/**********************************************************
 * @typedef Adc_GroupDesType
 * @brief   Kiểu dữ liệu xác định cấu hình các nhóm ADC. các
 *          kênh có cùng tính chất được lưu vào cùng một nhóm
 **********************************************************/
typedef struct {
    Adc_GroupType GroupId;
    uint8 NumberChannels;
    Adc_StatusType Status;
    volatile Adc_ValueGroupType *ResultBuffer;
    FunctionalState ScanModeConversion;
    Adc_GroupAccessModeType Group_DMAType;
    Adc_StreamBufferModeType StreamBufferMode;
    FunctionalState NotificationEnabled;
} Adc_GroupDefType;

/*************************************************************
 * @typedef Adc_ConfigType
 * @brief   Kiểu cấu trúc cấu hình các thông số cho ADC
 * @details Các thông số cơ bản cấu hình một kênh ADC như 
 *          kiểu chuyển đổi, thời gian lấy mẫu, mức độ ưu tiên
 *************************************************************/
typedef struct {
    Adc_InstanceTypde AdcInstance;
    Adc_GroupConvModeType ConversionMode;
    Adc_TriggerSourceType TriggerSource;
    Adc_ResultAlignmentType Alignment;
    Adc_ChannelConfigType Channels[16];
    Adc_GroupDefType Group;
    uint32 Mode;
    Adc_NotificationCallbackType NotificationCallback; // kiểu con trỏ hàm
} Adc_ConfigType;

/*Mảng chứa cấu hình các Channels trong ADC. mảng này sẽ được khai báo lại trong file ADC_Standard*/
extern Adc_ConfigType GroupAdc[ID_GroupAdc];
#endif