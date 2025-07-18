#include "Port_driver.h"
#include "stm32f10x_gpio.h"
void delay(uint64 time)
{
    while (time--);
}

int main ()
{
    Port_ConfigType ChannelConfig;
    ChannelConfig.ClockDirection = DISABLE;
    ChannelConfig.PinDirection = PORT_PIN_OUT;
    ChannelConfig.Pin = PortC_Pin_13;
    ChannelConfig.Port = PORT_C;
    ChannelConfig.Mode = PORT_PIN_MODE_DIO;
    ChannelConfig.Pull = PORT_PIN_MODE_OUT_PP;
    ChannelConfig.Speed = PIN_SPEED_MODE_50HZ;

    Port_Init(&ChannelConfig);
    while(1)
    {
        GPIO_InitTypeDef GPIO;
        GPIO.GPIO_Mode = GPIO_Mode_Out_OD;
        GPIO.GPIO_Pin = GPIO_Pin_11;
        GPIO.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO);

        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        delay(100000);
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        delay(100000);
    }
    return 0;
}