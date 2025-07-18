/****************************************************** 
 * @brief   Header file ADC_HW_Config
 * @details Chứa các hàm dùng để cấu hình ADC module và
 *          các Channels
 * @author  Trịnh Ngọc Phú - 2411
 * @date    2025
********************************************************/

#ifndef ADC_HW_CONFIG_H
#define ADC_HW_CONFIG_H

#include "ADC_Types.h"
#include "stm32f10x_adc.h"

/************************************************************************
 * @brief Hàm cấu hình ADC_1
 * @details Nhận vào con trỏ ConfigPtr kiểu Adc_ConfigType chứa các cấu 
 * hình của một module ADC
 ************************************************************************/
static inline void ADC_1_HW_Init(const Adc_ConfigType *ConfigPtr)
{
    ADC_InitTypeDef ADC_InitStruct;

    ADC_InitStruct.ADC_Mode =  ConfigPtr->Mode;
    ADC_InitStruct.ADC_ContinuousConvMode = (ConfigPtr->ConversionMode == ADC_CONV_MODE_ONESHOT) ? DISABLE : ENABLE;
    ADC_InitStruct.ADC_ScanConvMode = ConfigPtr->Group.ScanModeConversion;
    ADC_InitStruct.ADC_ExternalTrigConv = GET_TRIGGER_SOURCE(ConfigPtr->TriggerSource);
    ADC_InitStruct.ADC_DataAlign = GET_DATA_ALIGNMENT(ConfigPtr->Alignment);
    ADC_InitStruct.ADC_NbrOfChannel = ConfigPtr->Group.NumberChannels;

    ADC_Init(ADC1, &ADC_InitStruct);
}
/************************************************************************
 * @brief Hàm cấu hình ADC_2
 * @details Nhận vào con trỏ ConfigPtr kiểu Adc_ConfigType chứa các cấu 
 * hình của một module ADC
 ************************************************************************/
static inline void ADC_2_HW_Init(const Adc_ConfigType *ConfigPtr)
{
    ADC_InitTypeDef ADC_InitStruct;

    ADC_InitStruct.ADC_Mode =  ADC_Mode_Independent;
    ADC_InitStruct.ADC_ContinuousConvMode = (ConfigPtr->ConversionMode == ADC_CONV_MODE_ONESHOT) ? DISABLE : ENABLE;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = (ConfigPtr->TriggerSource == ADC_TRIGG_SRC_SW) ? ADC_ExternalTrigConv_None : ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfChannel = ConfigPtr->Group.NumberChannels;

    ADC_Init(ADC2, &ADC_InitStruct);
}

/************************************************************************
 * @brief Hàm cấu hình từng kênh Channel
 * @details Nhận vào con trỏ ConfigPtr kiểu Adc_ConfigType chứa các cấu 
 * hình của một Channle ADC cụ thể
 ************************************************************************/
static inline void Adc_HW_SetupChannels(const Adc_ConfigType *ConfigPtr)
{
    for(int i=0; i<ConfigPtr->Group.NumberChannels; i++)
    {
        Adc_ChannelType channel = ConfigPtr->Channels[i].Channel;
        uint32 SamplingTime = ConfigPtr->Channels[i].SamplingTime;
        uint8 Rank = ConfigPtr->Channels[i].Rank;

        if(ConfigPtr->AdcInstance == Adc_Instance_1)
        {
            ADC_RegularChannelConfig(ADC1, channel, Rank, SamplingTime);
        }
        else
        {
            ADC_RegularChannelConfig(ADC2, channel, Rank, SamplingTime);
        }
    }
}
/************************************************************************
 * @brief Hàm đánh thức VDK để bắt đầu quá trình chuyển đổi
 * @details Nhận vào con trỏ ConfigPtr kiểu Adc_ConfigType sau đó so sánh
 * nếu là ADC1 hoặc ADC2 sẽ tiến hành đánh thức từng module
 ************************************************************************/
static inline void Adc_HW_Enable(const Adc_ConfigType *ConfigPtr)
{
    if(ConfigPtr->AdcInstance == Adc_Instance_1)
    {
        ADC_Cmd(ADC1, ENABLE);
    }
    else 
    {
        ADC_Cmd(ADC2, ENABLE);
    }
}
#endif