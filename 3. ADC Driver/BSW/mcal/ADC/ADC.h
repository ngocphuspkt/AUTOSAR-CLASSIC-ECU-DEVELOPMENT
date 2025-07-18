#ifndef ADC_H
#define ADC_H
#include "ADC_HW_Config.h"
#include "ADC_Types.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

void Adc_Init(const Adc_ConfigType *ConfigPtr);
void Adc_DeInit (const Adc_ConfigType *ConfigPtr);
void Adc_StartGroupConversion (Adc_GroupType GroupId);
void Adc_StopGroupConversion(Adc_GroupType GroupId);
void Adc_EnableHardwareTrigger (Adc_GroupType GroupId, Adc_TriggerSourceType TriggerType);
void Adc_DisableHardwareTrigger (Adc_GroupType GroupId);
Std_ReturnType Adc_SetupResultBuffer (Adc_GroupType GroupId, Adc_ValueGroupType* DataBufferPtr);
Std_ReturnType Adc_ReadGroup (Adc_GroupType GroupId, volatile Adc_ValueGroupType* DataBufferPtr);
void Adc_EnableGroupNotification(Adc_GroupType GroupId);
void Adc_DisableGroupNotification(Adc_GroupType GroupId);
void ADC1_2_IRQHandler(void);
Adc_StatusType Adc_GetGroupStatus (Adc_GroupType GroupId);
Adc_StreamNumSampleType Adc_GetStreamLastPointer (Adc_GroupType Group,Adc_ValueGroupType** PtrToSamplePtr);

#endif