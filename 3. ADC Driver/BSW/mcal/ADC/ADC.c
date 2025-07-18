/****************************************************** 
 * @brief   Header source ADC_Standard
 * @details Chứa các hàm dùng để cấu hình ADC module và
 *          các Channels
 * @author  Trịnh Ngọc Phú - 2411
 * @date    2025
********************************************************/

#include "ADC.h"
#include "misc.h"
#include "stm32f10x.h"

/*Mảng chứa chứa thông tin các group*/
Adc_ConfigType GroupAdc[ID_GroupAdc];
/*****************************************************************
 * @brief Hàm Cấu hình toàn bộ ADC và Channels
 * @details Nhận vào một con trỏ ConfigPtr chứa toàn bộ cấu hình
 * cho ADC module và ADC Channel. Sau đó sẽ tiến hành cấu hình và 
 * so sánh thông qua các hàm  ADC_1_HW_Init, Adc_HW_SetupChannels, 
 * Adc_HW_Enable và ADC_DMACmd
 * Tiếp theo là gán cấu hình group vào mảng để dễ quản lí
 ******************************************************************/
void Adc_Init(const Adc_ConfigType *ConfigPtr)
{
    if(ConfigPtr == NULL_PIR) {return;}
    
    if(ConfigPtr->AdcInstance == Adc_Instance_1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
        ADC_1_HW_Init(ConfigPtr);
        if(ConfigPtr->Group.Group_DMAType == ADC_ACCESS_MODE_STREAMING)
        {
            ADC_DMACmd(ADC1, ENABLE);
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        }
        else if(ConfigPtr->Group.Group_DMAType == ADC_ACCESS_MODE_SINGLE)
        {
            ADC_DMACmd(ADC1, DISABLE);
        }
    }
    else if(ConfigPtr->AdcInstance == Adc_Instance_2)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
        ADC_2_HW_Init(ConfigPtr);
        //ADC2 không hỗ trợ sử dụng DMA
    }
    //GroupAdc[ConfigPtr->Group.GroupId] = *ConfigPtr;
    GroupAdc[ConfigPtr->Group.GroupId].AdcInstance = ConfigPtr->AdcInstance;
    GroupAdc[ConfigPtr->Group.GroupId].Alignment = ConfigPtr->Alignment;
    GroupAdc[ConfigPtr->Group.GroupId].ConversionMode = ConfigPtr->ConversionMode;
    GroupAdc[ConfigPtr->Group.GroupId].Mode = ConfigPtr->Mode;
    GroupAdc[ConfigPtr->Group.GroupId].NotificationCallback = ConfigPtr->NotificationCallback;
    GroupAdc[ConfigPtr->Group.GroupId].TriggerSource = ConfigPtr->TriggerSource;

    GroupAdc[ConfigPtr->Group.GroupId].Group.Group_DMAType = ConfigPtr->Group.Group_DMAType;
    GroupAdc[ConfigPtr->Group.GroupId].Group.GroupId = ConfigPtr->Group.GroupId;
    GroupAdc[ConfigPtr->Group.GroupId].Group.NotificationEnabled = ConfigPtr->Group.NotificationEnabled;
    GroupAdc[ConfigPtr->Group.GroupId].Group.NumberChannels = ConfigPtr->Group.NumberChannels;
    GroupAdc[ConfigPtr->Group.GroupId].Group.ResultBuffer = ConfigPtr->Group.ResultBuffer;
    GroupAdc[ConfigPtr->Group.GroupId].Group.ScanModeConversion = ConfigPtr->Group.ScanModeConversion;
    GroupAdc[ConfigPtr->Group.GroupId].Group.Status = ConfigPtr->Group.Status;
    GroupAdc[ConfigPtr->Group.GroupId].Group.StreamBufferMode = ConfigPtr->Group.StreamBufferMode;

    for(int i=0; i<16; i++)
    {
        if(ConfigPtr->Channels[i].Channel && ConfigPtr->Channels[i].Rank && ConfigPtr->Channels[i].SamplingTime)
        {
            //GroupAdc[i].Channels[i] = ConfigPtr->Channels[i];
            GroupAdc[ConfigPtr->Group.GroupId].Channels[i].Channel = ConfigPtr->Channels[i].Channel;
            GroupAdc[ConfigPtr->Group.GroupId].Channels[i].Rank = ConfigPtr->Channels[i].Rank;
            GroupAdc[ConfigPtr->Group.GroupId].Channels[i].SamplingTime = ConfigPtr->Channels[i].SamplingTime;
        }
    }
    Adc_HW_SetupChannels(ConfigPtr);

    Adc_HW_Enable(ConfigPtr);
}
/*****************************************************************
 * @brief Reset cấu hình về trạng thái mặc định
 ******************************************************************/
void Adc_DeInit (const Adc_ConfigType *ConfigPtr)
{
    ADC_InitTypeDef ADC_InitStruct;
    ADC_StructInit(&ADC_InitStruct);
    if(ConfigPtr->AdcInstance == Adc_Instance_1)
    {
        ADC_Init(ADC1, &ADC_InitStruct);
        ADC_DeInit(ADC1);
    }
    else if(ConfigPtr->AdcInstance == Adc_Instance_2)
    {
        ADC_Init(ADC2, &ADC_InitStruct);
        ADC_DeInit(ADC2);
    }
}
/*****************************************************************
 * @brief Hàm cho phép và bắt đầu quá trình chuyển đổi ADC
 * @details sau khi được đánh thức và gọi hàm này, quá trình chuyển 
 * đổi ADC sẽ được bắt đầu và đồng thời cũng cho phép module ADC sử
 * dụng chế độ DNA (nếu có)
 ******************************************************************/
void Adc_StartGroupConversion (Adc_GroupType GroupId)
{
    if (GroupId >= ID_GroupAdc) return;
    if(GroupAdc[GroupId].Group.Group_DMAType == ADC_ACCESS_MODE_STREAMING)
    {
        DMA_Cmd(DMA1_Channel1, ENABLE);
    }
    if(GroupAdc[GroupId].AdcInstance == Adc_Instance_1)
    {
        ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
    }
    else if (GroupAdc[GroupId].AdcInstance == Adc_Instance_2)
    {
        ADC_SoftwareStartConvCmd(ADC2, ENABLE);
    }
    GroupAdc[GroupId].Group.Status = ADC_BUSY;
}
/*****************************************************************
 * @brief Hàm ngưng quá trình chuyển đổi ADC (Stop conversion)
 * @details Ngừng lại quá trình chuyển đổi và DMA
 ******************************************************************/
void Adc_StopGroupConversion(Adc_GroupType GroupId)
{
    if (GroupId >= ID_GroupAdc) return;
    if(GroupAdc[GroupId].AdcInstance == Adc_Instance_1)
    {
        ADC_SoftwareStartConvCmd(ADC1, DISABLE);
        DMA_Cmd(DMA1_Channel1, DISABLE);
    }
    else if (GroupAdc[GroupId].AdcInstance == Adc_Instance_2)
    {
        ADC_SoftwareStartConvCmd(ADC2, DISABLE);
    }
    GroupAdc[GroupId].Group.Status = ADC_IDLE;
}

/*****************************************************************
 * @brief Chuyển đổi xung kích chuyển đổi ADC
 * @details Trong quá trình sử dụng, nếu muốn thay đổi xung kích từ
 * software (lệnh) bằng hardware (Timer,...) để ra lệnh chuyển đổi 
 * ADC thì sử dụng hàm này. Tham số truyền vào là Id của group muốn 
 * thay đổi và nguồn kích muốn thay đổi
 ******************************************************************/
void Adc_EnableHardwareTrigger (Adc_GroupType GroupId, Adc_TriggerSourceType TriggerType)
{
    if (GroupId >= ID_GroupAdc) return;
    GroupAdc[GroupId].TriggerSource = TriggerType;
    if(GroupAdc[GroupId].AdcInstance ==  Adc_Instance_1)
    {
        ADC_1_HW_Init(&GroupAdc[GroupId]);
    }
    else if (GroupAdc[GroupId].AdcInstance ==  Adc_Instance_2)
    {
        ADC_2_HW_Init(&GroupAdc[GroupId]);
    }
} 

/*****************************************************************
 * @brief Chuyển đổi ADC bằng software (lệnh)
 * @details Trong quá trình sử dụng, nếu muốn thay đổi xung kích từ
 * hardware về software (Trả về trigger kích bằng software)
 ******************************************************************/
void Adc_DisableHardwareTrigger (Adc_GroupType GroupId) 
{
    if (GroupId >= ID_GroupAdc) return;
    GroupAdc[GroupId].TriggerSource = ADC_TRIGG_SRC_SW;
    if(GroupAdc[GroupId].AdcInstance ==  Adc_Instance_1)
    {
        ADC_1_HW_Init(&GroupAdc[GroupId]);
    }
    else if (GroupAdc[GroupId].AdcInstance ==  Adc_Instance_2)
    {
        ADC_2_HW_Init(&GroupAdc[GroupId]);
    }
}
/*****************************************************************
 * @brief Setup nơi mà DMA sẽ trả về kết quả
 * @details Hàm này dùng để setup nơi mà DMA sẽ trả về kết quả đọc 
 * được từ thanh ghi DR của một group nào đó. Hàm này cần được thiết
 *  lập trước khi sử dụng DMA, để cho DMA biết rằng nó đã có nơi lưu 
 * vào (tránh trường hợp cấu hình hàm này sau khi bật DMA)
 ******************************************************************/
Std_ReturnType Adc_SetupResultBuffer (Adc_GroupType GroupId, Adc_ValueGroupType* DataBufferPtr)
{
    if(GroupId >= ID_GroupAdc || DataBufferPtr == NULL_PIR)
    {
        return E_NOT_OK;
    }
    if(GroupAdc[GroupId].Group.Group_DMAType == ADC_ACCESS_MODE_STREAMING)
    {
    DMA_InitTypeDef DMA_InitStruct;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32)&ADC1->DR;
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32)(DataBufferPtr);
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize = GroupAdc[GroupId].Group.NumberChannels;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
    DMA_InitStruct.DMA_Mode = (GroupAdc[GroupId].Group.StreamBufferMode == ADC_STREAM_BUFFER_LINEAR) ? DMA_Mode_Normal : DMA_Mode_Circular;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA1_Channel1, &DMA_InitStruct);
    }
    return E_OK;
} 
/*******************************************************************
 * @brief Setup nơi lưu trữ kết quả từ DR
 * @details Hàm này dùng để setup nơi lưu trữ dữ liệu trả về từ thanh
 * ghi DR. Hàm này được sử dụng bằng cách đọc thủ công (không sử dụng
 * DMA).
 ********************************************************************/
Std_ReturnType Adc_ReadGroup (Adc_GroupType GroupId, volatile Adc_ValueGroupType *DataBufferPtr) 
{
    if (GroupId >= ID_GroupAdc || DataBufferPtr == NULL_PIR)
        return E_NOT_OK;

    for (uint8 i = 0; i < GroupAdc[GroupId].Group.NumberChannels; i++)
    {
        while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
        DataBufferPtr[i] = ADC_GetConversionValue((GroupAdc[GroupId].AdcInstance == Adc_Instance_1) ? ADC1 : ADC2);
    }
    return E_OK;
}
/*******************************************************************
 * @brief bật thông báo cho group sau khi chuyển đổi xong
 * @details Hàm này dùng để bật thông báo cho một group sau khi chuyển
 * đổi xong một channel trong group. Trong hàm này sử dụng thông báo 
 * bằng cách gọi hàm ngắt sau khi chuyển đổi xong. Xong hàm ngắt sẽ
 * có vai trò kiểm tra xem hàm ngắt đến từ ADC nào (ADC1 hay ADC2).
 ********************************************************************/
void Adc_EnableGroupNotification(Adc_GroupType GroupId)
{
     
    if (GroupId >= ID_GroupAdc) return;
    
    GroupAdc[GroupId].Group.NotificationEnabled = ENABLE;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    if (GroupAdc[GroupId].AdcInstance == Adc_Instance_1)
    {
        ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE); // Cho phép ngắt khi hoàn tất ADC
    }
    else if (GroupAdc[GroupId].AdcInstance == Adc_Instance_2)
    {
        ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);
    }
}
/*******************************************************************
 * @brief tắt thông báo cho group
 ********************************************************************/
void Adc_DisableGroupNotification(Adc_GroupType GroupId)
{
    if (GroupId >= ID_GroupAdc) return;

    GroupAdc[GroupId].Group.NotificationEnabled = DISABLE;

    if (GroupAdc[GroupId].AdcInstance == Adc_Instance_1)
    {
        ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
    }
    else if (GroupAdc[GroupId].AdcInstance == Adc_Instance_2)
    {
        ADC_ITConfig(ADC2, ADC_IT_EOC, DISABLE);
    }
}
/*******************************************************************
 * @brief Hàm ngắt ADC
 * @details Hàm này sẽ được thực thi khi có ngắt xảy ra (bằng cách cho
 * phép thông báo bằng hàm Adc_EnableGroupNotification). Trong hàm này
 * sẽ gọi lại hàm NotificationCallback mà trong quá trình cấu hình cho 
 * group mình sẽ gán thông qua con trỏ hàm NotificationCallback trong 
 * struct Adc_ConfigType. Trong hàm callback đó mình có thể đọc giá trị
 * hay kiểm tra giá trị.
 ********************************************************************/
void ADC1_2_IRQHandler(void)
{
    // Kiểm tra ngắt từ ADC1
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET)
    {
        
        for (Adc_GroupType g = 0; g < ID_GroupAdc; g++)
        {
            if (GroupAdc[g].AdcInstance == Adc_Instance_1 &&
                GroupAdc[g].Group.NotificationEnabled == ENABLE &&
                GroupAdc[g].NotificationCallback != NULL_PIR)
                {
                    GroupAdc[g].Group.Status = ADC_COMPLETED;
                    GroupAdc[g].NotificationCallback();  // GỌI LẠI CALLBACK
                }
        }
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }
    // Kiểm tra ngắt từ ADC2
    else if (ADC_GetITStatus(ADC2, ADC_IT_EOC) != RESET)
    {
        
        for (Adc_GroupType g = 0; g < ID_GroupAdc; g++)
        {
            if (GroupAdc[g].AdcInstance == Adc_Instance_2 &&
                GroupAdc[g].Group.NotificationEnabled == ENABLE &&
                GroupAdc[g].NotificationCallback != NULL_PIR)
                {
                    GroupAdc[g].NotificationCallback();  // GỌI LẠI CALLBACK
                }
        }
        ADC_ClearITPendingBit(ADC2, ADC_IT_EOC);
    }
}
/**************************************************************
 * @brief hàm trả về trạng thái của group
 * @details - ADC_IDLE: ADC đang rãnh
 *          - ADC_BUSY: ADC đang bận
 *          - ADC_COMPLETED: ADC đã chuyển đổi xong
 *          - ADC_STREAM_COMPLETED: DMA đã sao chép xong dữ liệu
 **************************************************************/
Adc_StatusType Adc_GetGroupStatus (Adc_GroupType GroupId) 
{
    return (Adc_StatusType)GroupAdc[GroupId].Group.Status;
}
// Em chưa hoàn thành: Do mấy ngày nay đúi quá - em xin phép bổ sung sau
Adc_StreamNumSampleType Adc_GetStreamLastPointer (Adc_GroupType Group,Adc_ValueGroupType** PtrToSamplePtr) 
{
    return (Adc_StreamNumSampleType)0;
}
