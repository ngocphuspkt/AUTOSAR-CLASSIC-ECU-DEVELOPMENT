#include "PWM.h"
#include "Port_driver.h"
#include "stm32f10x_gpio.h"


volatile uint16 DutyCycle = 0;
volatile int8_t flag = 1;

void PWM_NotificationCallback(void)
{
    Dio_FlipChannel(PortC_Pin_13);
    DutyCycle += (5*flag);
}

void Config_Port(void)
{
    Port_ConfigType GPIOChannelConfig;
    GPIOChannelConfig.Port = PORT_B;
    GPIOChannelConfig.Pin = PortB_Pin_9;
    GPIOChannelConfig.ClockDirection = DISABLE;
    GPIOChannelConfig.Mode = PORT_PIN_MODE_DIO;
    GPIOChannelConfig.Pull = PORT_PIN_MODE_AF_PP;
    GPIOChannelConfig.Speed = PIN_SPEED_MODE_50HZ;
    GPIOChannelConfig.PinDirection = PORT_PIN_OUT;

    Port_Init(&GPIOChannelConfig);

    Port_ConfigType LedNotification;
    LedNotification.Port = PORT_C;
    LedNotification.Pin = PortC_Pin_13;
    LedNotification.ClockDirection = DISABLE;
    LedNotification.Mode = PORT_PIN_MODE_DIO;
    LedNotification.Pull = PORT_PIN_MODE_OUT_PP;
    LedNotification.Speed = PIN_SPEED_MODE_50HZ;
    LedNotification.PinDirection = PORT_PIN_OUT;

    Port_Init(&LedNotification);
}

void Config_PWMChannel(void)
{
    Pwm_ConfigType PWM_ChannelConfig;
    PWM_ChannelConfig.Channel = TIME4_CHANNEL_4;
    //PWM_ChannelConfig.ChannelClassType = PWM_VARIABLE_PERIOD;
    PWM_ChannelConfig.ChannelClassType = PWM_FIXED_PERIOD;
    PWM_ChannelConfig.IdleState = PWM_LOW;
    PWM_ChannelConfig.Period = 65000; // 20ms
    PWM_ChannelConfig.Polarity = TIM_OCPolarity_High;
    PWM_ChannelConfig.Pulse = 0; // 1ms
    PWM_ChannelConfig.NotificationCallback = PWM_NotificationCallback;
    Pwm_Init(&PWM_ChannelConfig);
}

int main ()
{
    uint16 value;
    Config_Port();
    Config_PWMChannel();
    Pwm_EnableNotification(TIME4_CHANNEL_4, PWM_FALLING_EDGE);
    
    while(1)
    {
        //value = TIM4->CCR4;
       
        if(DutyCycle >= 100)
        {
            flag = -1;
        }
        else if (DutyCycle <= 0)
        {
            flag = 1;
        }
        Pwm_SetDutyCycle(TIME4_CHANNEL_4,DutyCycle);
    } 
}

