#include "Port_driver.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

static uint8 Port_Initialized = 0; /*biến trạng thái xác định pin đã được cấu hình hay chưa*/
Port_ConfigType gpioMap[NUM_PORTS][NUM_PINS];
void Port_PinConfig (Port_ConfigType *ConfigPtr)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_TypeDef *GPIO_Port = GPIO_GET_PORT(ConfigPtr->Port);
    if(GPIO_Port == NULL_PIR) return;

    uint16_t pin = GPIO_GET_PIN_MASK(ConfigPtr->Pin);

    // 1. Cấp clock cho port
    RCC_APB2PeriphClockCmd(CLOCK_GET_PORT(ConfigPtr->Port), ENABLE);

    // 2. Cấu hình GPIO
    GPIO_InitStruct.GPIO_Pin = pin;
    GPIO_InitStruct.GPIO_Speed = SPEED_GET_PIN(ConfigPtr->Speed);

    if(ConfigPtr->Mode == PORT_PIN_MODE_DIO)
    {
        if(ConfigPtr->PinDirection == PORT_PIN_IN)
        {
            GPIO_InitStruct.GPIO_Mode = (ConfigPtr->Pull == PORT_PIN_MODE_INPUT_FLOATING) ? GPIO_Mode_IN_FLOATING :
                                        (ConfigPtr->Pull == PORT_PIN_MODE_INPUT_PULL_UP) ? GPIO_Mode_IPU :
                                                       GPIO_Mode_IPD;
        }
        else if (ConfigPtr->PinDirection == PORT_PIN_OUT)
        {
            GPIO_InitStruct.GPIO_Mode = (ConfigPtr->Pull == PORT_PIN_MODE_OUT_OD) ? GPIO_Mode_Out_OD : \
                                        (ConfigPtr->Pull == PORT_PIN_MODE_OUT_PP) ? GPIO_Mode_Out_PP : \
                                        (ConfigPtr->Pull == PORT_PIN_MODE_AF_OD)  ? GPIO_Mode_AF_OD : \
                                                                                    GPIO_Mode_AF_PP;
        }
    }
    else if(ConfigPtr->Mode == PORT_PIN_MODE_ADC)
    {
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    }

    GPIO_Init(GPIO_Port, &GPIO_InitStruct);

    if(ConfigPtr->PinDirection == PORT_PIN_OUT)
        GPIO_ResetBits(GPIO_Port, pin); // Mặc định LED sáng nếu LED PC13 active low
}

/******************************************************************
 * @brief Port_Init
 * @details Hàm nhận vào một con trỏ chứa thông số cấu hình của Pin
 ******************************************************************/
void Port_Init(Port_ConfigType *ConfigPin)
{
    if(ConfigPin == NULL_PIR) return;

    if(ConfigPin->Mode == PORT_PIN_MODE_ADC)
    {
        ConfigPin->Speed = PIN_SPEED_MODE_2HZ;
        ConfigPin->Pull = PORT_PIN_MODE_INPUT_ANALOG;
        ConfigPin->PinDirection = PORT_PIN_IN;
        ConfigPin->ClockDirection = DISABLE;
    }

    Port_PinConfig(ConfigPin);

    uint8 index = (uint8)(ConfigPin->Pin % 16);

    gpioMap[ConfigPin->Port][index].ClockDirection = ConfigPin->ClockDirection;
    gpioMap[ConfigPin->Port][index].Mode = ConfigPin->Mode;
    gpioMap[ConfigPin->Port][index].Pin = ConfigPin->Pin;
    gpioMap[ConfigPin->Port][index].PinDirection = ConfigPin->PinDirection;
    gpioMap[ConfigPin->Port][index].Port = ConfigPin->Port;
    gpioMap[ConfigPin->Port][index].Pull = ConfigPin->Pull;
    gpioMap[ConfigPin->Port][index].Speed = ConfigPin->Speed;

    Port_Initialized = 1; /*Pin đã cấu hình thành công*/
}

/******************************************************************
 * @brief Port_RefreshPortDirection
 * @details Hàm cấu hình chức năng mặc định cho tất cả các PIN
 ******************************************************************/
void Port_RefreshPortDirection(void)
{
    for (uint8 port = 0; port < NUM_PORTS; port++)
    {
        for (uint8 pin = 0; pin < NUM_PINS; pin++)
        {
            Port_ConfigType defaultCfg;
            defaultCfg.Port = (Dio_PortType)port;
            defaultCfg.Pin = (Dio_ChannelType)pin;
            defaultCfg.PinDirection = PORT_PIN_IN;
            defaultCfg.Mode = PORT_PIN_MODE_DIO;
            defaultCfg.Speed = PIN_SPEED_MODE_2HZ;
            defaultCfg.ClockDirection = ENABLE;
            defaultCfg.Pull = PORT_PIN_MODE_INPUT_FLOATING;

            Port_PinConfig(&defaultCfg);
            gpioMap[port][pin] = defaultCfg;
        }
    } 
}
/********************************************************************
 * @brief Port_SetPinDirection
 * @details Hàm thay đổi chiều của PIN: Output -> Input và ngược lại
 ********************************************************************/
void Port_SetPinDirection (Dio_PortType Port, Dio_ChannelType Pin, PortChange_ConfigType ChangeCofig)
{
    if(!Port_Initialized) return; // hàm Port_Init không khởi tạo đúng
    if(gpioMap[Port][Pin].ClockDirection != DISABLE) // cho phép thay đổi chiều
    {
        //kiểm tra xem trạng thái hiện tại có giống trạng thái trước hay không. Giống return. không giống thì cập nhật
        if(gpioMap[Port][Pin].PinDirection != ChangeCofig.DirectionChange) 
        {
            gpioMap[Port][Pin].PinDirection = ChangeCofig.DirectionChange;
            gpioMap[Port][Pin].ClockDirection = ChangeCofig.ClockDirection;
            if(ChangeCofig.DirectionChange == PORT_PIN_IN)
            {
                gpioMap[Port][Pin].Speed = PIN_SPEED_MODE_2HZ;
            }
            else 
            {
                gpioMap[Port][Pin].Speed = ChangeCofig.Speed;
            }
            gpioMap[Port][Pin].Pull = ChangeCofig.Pull;
        }
        else
        {
            return;
        }
    }
    else // không cho phép Disable
    {
        return;
    }
    Port_PinConfig(&gpioMap[Port][Pin]);
}

/******************************************************************
 * @brief PortDriver_GetVersionInfo
 * @details Hàm chứa version của phần mềm. Mỗi khi cập nhật đều phải
 *          thay đổi version, tùy theo sự thay lớn hay nhỏ
 ******************************************************************/
void PortDriver_GetVersionInfo(Std_VersionInfoType *VersionInfo)
{
    //establish: 19 - 6 - 2025 (day - month - year)
    VersionInfo->moduleID = 2025;
    VersionInfo->sw_major_version = 1;
    VersionInfo->sw_minor_version = 0;
    VersionInfo->sw_patch_version = 0;
}