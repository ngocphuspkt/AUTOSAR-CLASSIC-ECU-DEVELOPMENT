#include "ADC_Standard.h"
#include "Port_driver.h"
#include "DIO.h"


volatile uint16 value[10];

void ADC1_Irq(void)
{
    Adc_ReadGroup(GROUP_0, GroupAdc[GROUP_0].Group.ResultBuffer);
    if(value[0] > 2000)
    {
        Dio_WriteChannel(PortC_Pin_13, STD_LOW);
    }
    else 
    {
        Dio_WriteChannel(PortC_Pin_13, STD_HIGH);
    }
}

void DIO_Config()
{
    Port_ConfigType ConfigPinA;
    ConfigPinA.Mode = PORT_PIN_MODE_ADC;
    ConfigPinA.Port = PORT_A;
    ConfigPinA.Pin = PortA_Pin_0;
    Port_Init(&ConfigPinA);

    Port_ConfigType ConfigPinC;
    ConfigPinC.ClockDirection = DISABLE;
    ConfigPinC.Mode = PORT_PIN_MODE_DIO;
    ConfigPinC.Pin = PortC_Pin_13;
    ConfigPinC.PinDirection = PORT_PIN_OUT;
    ConfigPinC.Port = PORT_C;
    ConfigPinC.Pull = PORT_PIN_MODE_OUT_PP;
    ConfigPinC.Speed = PIN_SPEED_MODE_50HZ;
    Port_Init(&ConfigPinC);
}

void ADC_Config()
{
    Adc_ConfigType ConfigPtr;
    ConfigPtr.AdcInstance = Adc_Instance_1;
    ConfigPtr.ConversionMode = ADC_CONV_MODE_CONTINUOUS;
    ConfigPtr.TriggerSource = ADC_TRIGG_SRC_SW;
    ConfigPtr.Alignment = ADC_ALIGN_RIGHT;
    ConfigPtr.Mode = ADC_Mode_Independent;
    ConfigPtr.Channels[0].Channel = ADC_Channel_0;
    ConfigPtr.Channels[0].Rank = 0;
    ConfigPtr.Channels[0].SamplingTime = ADC_SampleTime_1Cycles5;

    //Config Group ADC
    ConfigPtr.Group.GroupId = GROUP_0;
    ConfigPtr.Group.NumberChannels = 1;
    ConfigPtr.Group.ResultBuffer = value;
    ConfigPtr.Group.ScanModeConversion = DISABLE;
    ConfigPtr.Group.Group_DMAType = ADC_ACCESS_MODE_SINGLE; // không sử dụng DMA
    
    //Callback Function: khi có ngắt thì sẽ gọi hàm này
    ConfigPtr.NotificationCallback = ADC1_Irq;
    Adc_Init(&ConfigPtr);
}

int main ()
{
    
    DIO_Config(); // Cấu hình Port - Pin
    Dio_WriteChannel(PortC_Pin_13, STD_HIGH); // Turn off the led before operation project
    ADC_Config(); //cấu hình ADC
    //cho phép sử dụng ngắt
    Adc_EnableGroupNotification(GROUP_0); // cho phép thông báo khi có ngắt cho Group 1
    Adc_StartGroupConversion(GROUP_0); // Bắt đầu chuyển đổi
    while(1)
    {
        
    }
}
