/****************************************************** 
 * @brief   Header file Port Driver
 * @details Chứa các thông tin cần thiết để cấu hình cho
 *          Port - Pin 
 * @author  Trịnh Ngọc Phú
 * @date    2025
********************************************************/
#ifndef PORT_DRIVER_H
#define PORT_DRIVER_H

#include "Std_Types.h"
#include "stm32f10x.h"
#include "Standard_PortType.h"

/*******************************************************************
 * @brief số lượng Port được sử dụng trong module STM32F1xx
********************************************************************/
#define NUM_PORTS  4
/*******************************************************************
 * @brief số lượng PIN trong một Port module STM32F1xx
********************************************************************/
#define NUM_PINS   16

/********************************************************************
 * @brief   Macro Cấp clock cho từng port
 * @details macro sẽ nhận vào một tham số port. sau đó sẽ tiến hành
 *          cấp xung tương ứng
 ********************************************************************/
#define CLOCK_GET_PORT(PortId) \
        ( ( (PortId) == PORT_A ) ? RCC_APB2Periph_GPIOA:\
          ( (PortId) == PORT_B ) ? RCC_APB2Periph_GPIOB:\
          ( (PortId) == PORT_C ) ? RCC_APB2Periph_GPIOC:\
          ( (PortId) == PORT_D) ? RCC_APB2Periph_GPIOD:\
          0)

/********************************************************************
 * @brief   Macro Cấp speed cho từng Pin
 * @details macro sẽ nhận vào một tham số speed sau đó sẽ cấp speed cho
 *          pin cấu hình
 ********************************************************************/
#define SPEED_GET_PIN(SpeedMode) \
        ( ( (SpeedMode) == PIN_SPEED_MODE_10HZ ) ? GPIO_Speed_10MHz:\
          ( (SpeedMode) == PIN_SPEED_MODE_2HZ  ) ? GPIO_Speed_2MHz: \
          ( (SpeedMode) == PIN_SPEED_MODE_50HZ ) ? GPIO_Speed_50MHz:\
          0)

/*********************************************************************
* @typedef define
* @brief Định nghĩa các kiểu chức năng cho PIN. Dùng để hỗ trợ cho kiểu
        dữ liệu Port_PinModeType
**********************************************************************/
#define PORT_PIN_MODE_DIO 0
#define PORT_PIN_MODE_ADC 1
#define PORT_PIN_MODE_PWM 2
#define PORT_PIN_MODE_I2C 3
#define PORT_PIN_MODE_SPI 4
#define PORT_PIN_MODE_CAN 5
#define PORT_PIN_MODE_LIN 6

/*********************************************************************
 * @brief nghĩa các chế độ INPUT (floating, pull-up và pull-down)
**********************************************************************/
#define PORT_PIN_MODE_INPUT_FLOATING  0
#define PORT_PIN_MODE_INPUT_PULL_UP   1
#define PORT_PIN_MODE_INPUT_PULL_DOWN 2
#define PORT_PIN_MODE_INPUT_ANALOG    3
/*********************************************************************
 * @brief Định nghĩa các chế độ OUTPUT (open-drain và push-pull)
**********************************************************************/
#define PORT_PIN_MODE_OUT_OD    0
#define PORT_PIN_MODE_OUT_PP    1
#define PORT_PIN_MODE_AF_OD     2
#define PORT_PIN_MODE_AF_PP     3

/*********************************************************************
 * @enum Port_PinDirectionType
 * @brief   định nghĩa chiều cho PIN - Ouput hay Input
 *********************************************************************/
typedef enum
{
    PORT_PIN_IN = 0x00,
    PORT_PIN_OUT
}Port_PinDirectionType;

/*********************************************************************
 * @typedef Port_PinModeType
 * @brief   Định nghĩa chức năng cho PIN
 * @details Trạng thái Output, Input như thế nào: PUll-Up, PUll-Down
 *********************************************************************/
typedef uint8 Port_PinModeType;

/*********************************************************************
 * @enum    Port_PinDirectionType
 * @brief   Định nghĩa chiều cho một Pin trong Port. Có thể là input
 *          output.
 *********************************************************************/
typedef enum
{
    PIN_SPEED_MODE_10HZ = 0, 
    PIN_SPEED_MODE_2HZ,
    PIN_SPEED_MODE_50HZ
}Pin_Speed_OutputType;

/***********************************************************************
 * @typedef Port_ConfigType
 * @brief   Kiểu cấu trúc chứa thông tin cấu hình cho PIN. Những thông tin
 *          cơ bản để cấu hình cho một PIN được chứa trong đây
 ************************************************************************/
typedef struct 
{
    Dio_PortType Port;
    Dio_ChannelType Pin;
    Port_PinDirectionType PinDirection;
    Port_PinModeType Mode;
    Pin_Speed_OutputType Speed;
    FunctionalState ClockDirection;
    uint8 Pull;
}Port_ConfigType;

/***********************************************************************
 * @typedef PortChange_ConfigType
 * @brief   Kiểu cấu trúc chứa thông tin cần thay đổi khi đổi chiều PIN
 *          khi đổi chiều, bắt buộc chứa các thông tin cần thiết để cấu
 *          hình cho PIN
 ************************************************************************/
typedef struct 
{
    Port_PinDirectionType DirectionChange;
    Pin_Speed_OutputType Speed;
    FunctionalState ClockDirection;
    uint8 Pull;
}PortChange_ConfigType;

/***********************************************************************
 * @typedef Port_ConfigType
 * @brief   Mảng có các phần tử là tất cả PIN của 4 Port (A,B,C,D). Đồng
 *          thời lưu trữ cấu hình của PIN
 ************************************************************************/
extern Port_ConfigType gpioMap[NUM_PORTS][NUM_PINS];

void Port_PinConfig (Port_ConfigType *ConfigPtr);
void Port_Init( Port_ConfigType* ConfigPin);
void Port_SetPinDirection (Dio_PortType Port, Dio_ChannelType Pin, PortChange_ConfigType ChangeCofig);
void Port_RefreshPortDirection(void);
void PortDriver_GetVersionInfo(Std_VersionInfoType *VersionInfo);
#endif