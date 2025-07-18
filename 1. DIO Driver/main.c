#include "Port_driver.h"
#include "DIO.h"

volatile uint16 count = 0;
void delay(uint64 time)
{
    while(time--);
}
int main ()
{
    Port_ConfigType ConfigPin;
    ConfigPin.ClockDirection = DISABLE;
    ConfigPin.Mode = PORT_PIN_MODE_DIO;
    ConfigPin.Pin = PortC_Pin_13;
    ConfigPin.PinDirection = PORT_PIN_OUT;
    ConfigPin.Port = PORT_C;
    ConfigPin.Pull = PORT_PIN_MODE_OUT_PP;
    ConfigPin.Speed = PIN_SPEED_MODE_50HZ;
    Port_Init(&ConfigPin);

    Dio_WriteChannel(PortC_Pin_13, STD_HIGH);
    while(1)
    {
        Dio_FlipChannel(PortC_Pin_13);
        delay(100000);
        count++;
        if(count == 1000)
        {
            count = 0;
        }
    }
}