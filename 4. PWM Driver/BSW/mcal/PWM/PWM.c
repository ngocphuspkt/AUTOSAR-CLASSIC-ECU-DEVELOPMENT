#include "PWM.h"
#include "PwmSaveChannel.h"

Pwm_ConfigType GroupPWM[12];

volatile uint8 Pwm_Initialized = 0;


void Pwm_Init ( Pwm_ConfigType *ConfigPtr)
{
    if(ConfigPtr == NULL_PIR)
    {
        return;
    }

    TIM_TypeDef *TIMx = GET_MODULE_TIMER(ConfigPtr->Channel);
    GET_RCC_TIMER(TIMx); // bật clock cho timer tương ứng
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_Prescaler = 8 - 1; // Chỉ lấy xung lock từ thạch anh bên ngoài
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up; // Upcounting
    TIM_InitStruct.TIM_Period = (uint16)ConfigPtr->Period - 1;

    TIM_TimeBaseInit(TIMx, &TIM_InitStruct);

    TIM_OCInitTypeDef TIM_OC;
    TIM_OC.TIM_OCMode = TIM_OCMode_PWM1; // PWM mode 1
    TIM_OC.TIM_OCPolarity = ConfigPtr->Polarity;
    TIM_OC.TIM_OutputState = TIM_OutputState_Enable; // enable output PWM
    TIM_OC.TIM_Pulse = ConfigPtr->Pulse; 
    
    switch ((ConfigPtr->Channel) % 4)     
    {                             
        case 0: TIM_OC1Init(TIMx, &TIM_OC); break; 
        case 1: TIM_OC2Init(TIMx, &TIM_OC); break; 
        case 2: TIM_OC3Init(TIMx, &TIM_OC); break; 
        case 3: TIM_OC4Init(TIMx, &TIM_OC); break; 
        default: break;
    }  
    //Bật chức năng cập nhật giá trị sau mỗi lần có sự kiện Update (tránh tình trạng làm gián đoạn trong quá trình chạy)
    switch ((ConfigPtr->Channel) % 4)
    {
        case 0: TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable); break;
        case 1: TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable); break;
        case 2: TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable); break;
        case 3: TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable); break;
    }

    //cho phép Timer hoạt động và bắt đầu hoạt động
    TIM_Cmd(TIMx, ENABLE);

    //PIN đã được khởi tạo chức năng PWM thành công. Tham số này dùng để xét khi thay đổi Period và DutyCycle
    ConfigPtr->initialize = INIT_INITIALIZED;

    //lưu giá trị vào mảng
    PWM_SaveChannelConfig(ConfigPtr);

    //sau khi cấu hình tất cả các PIN thì sẽ set biến Pwm_Initialized - thông báo đã hoàn thành việc cấu hình
    Pwm_Initialized = 1;
}

void Pwm_DeInit(void) // giải quyết bằng mảng - sử dụng cái nào thì Set the default configuration
{
    if(!Pwm_Initialized) // Trước đó chưa khởi tạo chức năng PWM thành công => thoát ra
    { return; } 

    //cấu hình về trạng thái mặc đinh
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);

    //tắt timer
    TIM_Cmd(TIM2, DISABLE);
    TIM_Cmd(TIM3, DISABLE);
    TIM_Cmd(TIM4, DISABLE);

    for(int i=0; i<4; i++) // các kênh PWM đều trở về trạng thái mặc định kể cả không sử dụng và cấu hình
    {
        switch (i % 4)     
        {                             
            case 0: 
                {
                    TIM_OC1Init(TIM2, &TIM_OCInitStruct);
                    TIM_OC1Init(TIM3, &TIM_OCInitStruct);
                    TIM_OC1Init(TIM4, &TIM_OCInitStruct);
                } break; 
            case 1: 
                {
                    TIM_OC2Init(TIM2, &TIM_OCInitStruct);
                    TIM_OC2Init(TIM3, &TIM_OCInitStruct);
                    TIM_OC2Init(TIM4, &TIM_OCInitStruct);
                }break; 
            case 2: 
                {
                    TIM_OC3Init(TIM2, &TIM_OCInitStruct);
                    TIM_OC3Init(TIM3, &TIM_OCInitStruct);
                    TIM_OC3Init(TIM4, &TIM_OCInitStruct);
                } break;
            case 3: 
                {
                    TIM_OC4Init(TIM2, &TIM_OCInitStruct);
                    TIM_OC4Init(TIM3, &TIM_OCInitStruct);
                    TIM_OC4Init(TIM4, &TIM_OCInitStruct);
                } break;
            default: break;
        } 
    }
    Pwm_Initialized = 0;
}

void Pwm_SetDutyCycle (Pwm_ChannelType ChannelNumber,uint16 DutyCycle)
{
    /******************************************************************************
    *  Kiểm tra xem trước đó khởi tạo thành công hay chưa
    *  sau đó kiểm tra xem PIN đó đã được cấu hình PWM hay chưa - nếu chứ thoát ra
    *******************************************************************************/
    if(!Pwm_Initialized && GroupPWM[ChannelNumber].initialize == INIT_UNINITIALIZED)
    { return; }
    //Tính toán giá trị để cấp cho thanh ghi Compare/Capture register (CCR) thông qua DutyCycle
    if (DutyCycle > 100) DutyCycle = 100;
    uint16 Pulse = ((uint16)DutyCycle * GroupPWM[ChannelNumber].Period) / 100;

    //Lấy Timer Module tương ứng
    TIM_TypeDef *TIMx = GET_MODULE_TIMER(GroupPWM[ChannelNumber].Channel); 

    if      (TIMx == TIM2) { TIM_SetCompare2(TIM2, Pulse); } // thay đổi giá trị trong thanh ghi CCR cho Timer_2
    else if (TIMx == TIM3) { TIM_SetCompare3(TIM3, Pulse); } // thay đổi giá trị trong thanh ghi CCR cho Timer_3
    else if (TIMx == TIM4) { TIM_SetCompare4(TIM4, Pulse); } // thay đổi giá trị trong thanh ghi CCR cho Timer_4
}

void Pwm_SetPeriodAndDuty (Pwm_ChannelType ChannelNumber, Pwm_PeriodType Period, uint16 DutyCycle)
{
    // cấu hình chu kỳ và DutyCycle: có tính công thức để gán giá trị cho các ARR - CPP
    // sau đó gọi Pwm_Init
    // cần kiểm tra kiểu Pwm_ChannelClassType: có cho chỉnh sửa Period không

    if(!Pwm_Initialized && GroupPWM[ChannelNumber].initialize == INIT_UNINITIALIZED && 
        GroupPWM[ChannelNumber].ChannelClassType == PWM_FIXED_PERIOD)
        { return; }

    TIM_TypeDef *TIMx = GET_MODULE_TIMER(GroupPWM[ChannelNumber].Channel);
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_Prescaler = 8 - 1; // Chỉ lấy xung lock từ thạch anh bên ngoài
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up; // Upcounting
    TIM_InitStruct.TIM_Period = Period - 1;

    TIM_TimeBaseInit(TIMx, &TIM_InitStruct);

    Pwm_SetDutyCycle(ChannelNumber, DutyCycle);
}

void Pwm_SetOutputToIdle (Pwm_ChannelType ChannelNumber)
{
    // đưa về trạng thái rãnh - DIO(Low)
    Dio_WriteChannel(ChannelNumber, STD_LOW);   
}

// đọc giá trị trạng thái hiện tại của PIN PWM (LOW - HIGH)
// Có thể sử dụng 2 cách
/*********************************************************************
 * Cách 1: So sánh giá trị trên thanh ghi CCR và CNT. Đồng thời kiểm tra
 *         TIM_OCPolarity 
 * Cách 2: Đọc giá trị trên chân PIN cấu hình chức năng PWM bằng hàm 
 *         Dio_ReadChannel trong DIO.h
 *********************************************************************/ 
Pwm_OutputStateType Pwm_GetOutputState (Pwm_ChannelType ChannelNumber)
{
    uint8 Channel = GetDioPwmMode(ChannelNumber);
    if(Dio_ReadChannel(Channel) == STD_HIGH)
    {
        return PWM_HIGH;
    }
    else 
    {
        return PWM_LOW;
    }
}

void Pwm_DisableNotification (Pwm_ChannelType ChannelNumber)
{
    // tắt chức ngắt với từng kênh PWM
    //kiểm tra xem kênh PWM muốn tắt chức năng thông báo đã được khởi tạo trước đó hay chưa
    if(GroupPWM[ChannelNumber].initialize != INIT_INITIALIZED) { return; }

    TIM_TypeDef *TIMx = GET_MODULE_TIMER(ChannelNumber);
    switch (ChannelNumber % 4)
    {
    case 0:
        TIM_ITConfig(TIMx, TIM_IT_CC1 | TIM_IT_Update, DISABLE); 
        break;
    case 1:
        TIM_ITConfig(TIMx, TIM_IT_CC1 | TIM_IT_Update, DISABLE); 
        break;
    case 2:
        TIM_ITConfig(TIMx, TIM_IT_CC1 | TIM_IT_Update, DISABLE); 
        break;
    case 3:
        TIM_ITConfig(TIMx, TIM_IT_CC1 | TIM_IT_Update, DISABLE); 
        break;
    default:
        break;
    }
}
/* 
Cho phép timer 2 ngắt toàn cục - cục bộ
Bật ngắt CNT == CCRx (tra datasheet kỹ)
Code bằng thanh ghi
*/
void Pwm_EnableNotification (Pwm_ChannelType ChannelNumber, Pwm_EdgeNotificationType Notification)
{
    // bật tất cả các ngắt - cho phép ngắt xảy ra
    // cách 1: Nhưng trong tiêu chuẩn autosar thì không sử dụng tên ngắt của timer thay vào đó là NotificationPWM
    // cách 2: sử dụng con trỏ hàm - khi ngắt thì gọi hàm đó 

    //kiểm tra xem kênh PWM muốn tắt chức năng thông báo đã được khởi tạo trước đó hay chưa
    if(GroupPWM[ChannelNumber].initialize != INIT_INITIALIZED) { return; }

    TIM_TypeDef *TIMx = GET_MODULE_TIMER(ChannelNumber);

    NVIC_InitTypeDef NVIC_InitStruct;
     // cho phép ngắt Timer 2
    if(TIMx == TIM2)      { NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn; }
    else if (TIMx == TIM3) { NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn; }
    else if (TIMx == TIM4) { NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn; }
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    switch (Notification)
    {
        case PWM_RISING_EDGE:
            TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE); // CNT = 0 ~ cạnh lên
            break;

        case PWM_FALLING_EDGE:
            TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE); // CNT == CCR1 ~ cạnh xuống
            break;

        case PWM_BOTH_EDGES:
            TIM_ITConfig(TIMx, TIM_IT_Update | TIM_IT_CC1, ENABLE);
            break;
        default:
            break;
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // GỌI CALLBACK CẠNH LÊN
        for(int i=0; i<12; i++)
        {
            if(GroupPWM[i].NotificationCallback != NULL_PIR && GroupPWM[i].initialize != INIT_UNINITIALIZED)
            {
                GroupPWM[i].NotificationCallback();
            }
        }
        //xóa cờ 
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {
        // GỌI CALLBACK CẠNH XUỐNG
        for(int i=0; i<12; i++)
        {
            if(GroupPWM[i].NotificationCallback != NULL_PIR && GroupPWM[i].initialize != INIT_UNINITIALIZED)
            {
                GroupPWM[i].NotificationCallback();
            }
        }
        //xóa cờ
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    }
}
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        // GỌI CALLBACK CẠNH LÊN
        for(int i=0; i<12; i++)
        {
            if(GroupPWM[i].NotificationCallback != NULL_PIR && GroupPWM[i].initialize != INIT_UNINITIALIZED)
            {
                GroupPWM[i].NotificationCallback();
            }
        }
        //xóa cờ 
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
    {
        // GỌI CALLBACK CẠNH XUỐNG
        for(int i=0; i<12; i++)
        {
            if(GroupPWM[i].NotificationCallback != NULL_PIR && GroupPWM[i].initialize != INIT_UNINITIALIZED)
            {
                GroupPWM[i].NotificationCallback();
            }
        }
        //xóa cờ
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    }
}
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        // GỌI CALLBACK CẠNH LÊN
        for(int i=0; i<12; i++)
        {
            if(GroupPWM[i].NotificationCallback != NULL_PIR && GroupPWM[i].initialize != INIT_UNINITIALIZED)
            {
                GroupPWM[i].NotificationCallback();
            }
        }
        //xóa cờ 
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }

    if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
    {
        // GỌI CALLBACK CẠNH XUỐNG
        for(int i=0; i<12; i++)
        {
            if(GroupPWM[i].NotificationCallback != NULL_PIR && GroupPWM[i].initialize != INIT_UNINITIALIZED)
            {
                GroupPWM[i].NotificationCallback();
            }
        }
        //xóa cờ
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
    }
}
/******************************************************************
 * @brief PortDriver_GetVersionInfo
 * @details Hàm chứa version của phần mềm. Mỗi khi cập nhật đều phải
 *          thay đổi version, tùy theo sự thay lớn hay nhỏ
 ******************************************************************/
void PWM_GetVersionInfo(Std_VersionInfoType *VersionInfo)
{
    //establish: 19 - 6 - 2025 (day - month - year)
    VersionInfo->moduleID = 2025;
    VersionInfo->sw_major_version = 1;
    VersionInfo->sw_minor_version = 0;
    VersionInfo->sw_patch_version = 0;
}