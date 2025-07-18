/****************************************************** 
 * @brief   Header file PWM Types
 * @details Chứa các khai báo, định nghĩa kiểu dữ liệu 
 *          struct cấu hình thông số PWM được sử dụng 
 *          trong file PWM.c
 * @author  Trịnh Ngọc Phú - 2411
 * @date    2025
********************************************************/
#ifndef PWM_H
#define PWM_H

#include "Std_Types.h"
#include "stm32f10x_tim.h"
#include "DIO.h"
#include "misc.h"
#include "stm32f10x_rcc.h"

typedef void (*PWM_NotificationCallbackType)(void);
/********************************************************
 * @brief Macro lấy ra Module Timer thông qua Id
 ********************************************************/

#define GET_MODULE_TIMER(Id) ((Id <= TIME2_CHANNEL_4) ? TIM2 : \
                                (Id > TIME2_CHANNEL_4 && Id <= TIME3_CHANNEL_4) ? TIM3 : \
                                TIM4)

#define GET_RCC_TIMER(TIMx) ((TIMx == TIM2) ? RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE) : \
                             (TIMx == TIM3) ? RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE): \
                                              RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE))
/*************************************************************
 * @brief Macro lấy ra chân PIN DIO tương ứng với channel PWM
 *************************************************************/                                
#define GetDioPwmMode(Id) ((Id == TIME2_CHANNEL_1) ? PortA_Pin_0 : \
                         (Id == TIME2_CHANNEL_2) ? PortA_Pin_1 : \
                         (Id == TIME2_CHANNEL_3) ? PortA_Pin_2 : \
                         (Id == TIME2_CHANNEL_4) ? PortA_Pin_3 : \
                         (Id == TIME3_CHANNEL_1) ? PortA_Pin_6 : \
                         (Id == TIME3_CHANNEL_2) ? PortA_Pin_7 : \
                         (Id == TIME3_CHANNEL_3) ? PortB_Pin_0 : \
                         (Id == TIME3_CHANNEL_4) ? PortB_Pin_1 : \
                         (Id == TIME4_CHANNEL_1) ? PortB_Pin_6 : \
                         (Id == TIME4_CHANNEL_2) ? PortB_Pin_7 : \
                         (Id == TIME4_CHANNEL_3) ? PortB_Pin_8 : \
                                                   PortB_Pin_9)
/********************************************************
 * @typedef Pwm_ChannelType
 * @brief kiểu dữ liệu định nghĩa kênh Time cho phép xuất
 *        xung PWM
 * @details các kênh ở chế độ mặc định No Remap, sẽ ánh xạ
 *          tới các PIN tương ứng (được giải thích)
 ********************************************************/

typedef enum 
{
    TIME2_CHANNEL_1 = 0x00, //PA0
    TIME2_CHANNEL_2,        //PA1
    TIME2_CHANNEL_3,        //PA2
    TIME2_CHANNEL_4,        //PA3
    TIME3_CHANNEL_1,        //PA6
    TIME3_CHANNEL_2,        //PA7
    TIME3_CHANNEL_3,        //PB0
    TIME3_CHANNEL_4,        //PB1
    TIME4_CHANNEL_1,        //PB6
    TIME4_CHANNEL_2,        //PB7
    TIME4_CHANNEL_3,        //PB8
    TIME4_CHANNEL_4         //PB9
}Pwm_ChannelType;

/********************************************************
 * @typedef Pwm_PeriodType
 * @brief   kiểu dữ liệu định nghĩa chu kỳ của xung PWM
 ********************************************************/
typedef uint32 Pwm_PeriodType;

/********************************************************
 * @typedef Pwm_OutputStateType
 * @brief   Kiểu dữ liệu định nghĩa mức điện áp đầu ra
 ********************************************************/
typedef enum
{
    PWM_LOW  = 0x00,
    PWM_HIGH = 0x01
}Pwm_OutputStateType;

/************************************************************
 * @typedef Pwm_EdgeNotificationType
 * @brief   Kiểu dữ liệu lựa chọn kiểu thông báo 
 * @details - PWM_RISING_EDGE: Thông báo khi xuất hiện cạnh 
 *          lên
 *          - PWM_FALLING_EDGE: Thông báo khi xuất hiện cạnh 
 *          xuống
 *          - PWM_BOTH_EDGES: kết hợp cả 2 cạnh
 * watch out: Tương tự như ngắt trong timer
 **********************************************************/
typedef enum 
{
    PWM_RISING_EDGE = 0x00, 
    PWM_FALLING_EDGE = 0x01,
    PWM_BOTH_EDGES
}Pwm_EdgeNotificationType;

/************************************************************
 * @typedef Pwm_ChannelClassType
 * @brief   Kiểu dữ liệu lựa chọn sự thay đổi chu kỳ 
 * @details - PWM_VARIABLE_PERIOD: Chu kỳ có thể thay đổi
 *            sau khi đã cấu hình
 *          - PWM_FIXED_PERIOD: Chu kỳ không thể thay đổi
 *            sau khi đã cấu hình
 *          - PWM_FIXED_PERIOD_SHIFTED: 
 **********************************************************/
typedef enum 
{
    PWM_VARIABLE_PERIOD = 0x00, // Tương tự như hàm TIM_SetCampare trong SPL
    PWM_FIXED_PERIOD = 0x01, 
    PWM_FIXED_PERIOD_SHIFTED
}Pwm_ChannelClassType;

/***********************************************************
 * @typedef Initialized
 * @brief   Kiểu dữ liệu thông báo chân PWM đã được khởi tạo
 ***********************************************************/
typedef enum 
{
    INIT_UNINITIALIZED = 0x00,
    INIT_INITIALIZED
}Initialized;

/********************************************************
 * @typedef Pwm_ChannelConfig
 * @brief   Kiểu cấu trúc chứa định nghĩa và khai báo của 
 *          một PIN được cài đặc chức năng PWM
 * @details Bao gồm các cấu hình cơ bản của PWM - Các thông
 *          tin còn lại sẽ được mặc định
 ********************************************************/
typedef struct 
{
    Pwm_ChannelType Channel; /*<Kênh PWM sử dụng>*/
    Pwm_PeriodType Period; /*<Chu kỳ của xung PWM>*/
    Pwm_OutputStateType IdleState; /*<Trạng thái khi Idle>*/
    uint16 Pulse; /*<Thời gian mức HIGH tồn tại>*/
    Pwm_ChannelClassType ChannelClassType; /*<Cho phép thay đổi period hay không>*/
    uint16 Polarity; /*<Cực tính xác định mức logic>*/
    Initialized initialize; // khi khởi tạo sẽ tự gán
    PWM_NotificationCallbackType NotificationCallback; // kiểu con trỏ hàm
    uint8 NotificationSaveArray;
}Pwm_ConfigType;

/*********************************************************
 * Mảng chứa cấu hình các chân PWM
 *********************************************************/
extern Pwm_ConfigType GroupPWM[12];

void Pwm_Init ( Pwm_ConfigType *ConfigPtr);
void Pwm_DeInit(void);
void Pwm_SetDutyCycle (Pwm_ChannelType ChannelNumber,uint16 DutyCycle);
void Pwm_SetPeriodAndDuty (Pwm_ChannelType ChannelNumber, Pwm_PeriodType Period, uint16 DutyCycle);
void Pwm_SetOutputToIdle (Pwm_ChannelType ChannelNumber);
Pwm_OutputStateType Pwm_GetOutputState (Pwm_ChannelType ChannelNumber);
void Pwm_DisableNotification (Pwm_ChannelType ChannelNumber);
void Pwm_EnableNotification (Pwm_ChannelType ChannelNumber, Pwm_EdgeNotificationType Notification);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void PWM_GetVersionInfo(Std_VersionInfoType *VersionInfo);
#endif