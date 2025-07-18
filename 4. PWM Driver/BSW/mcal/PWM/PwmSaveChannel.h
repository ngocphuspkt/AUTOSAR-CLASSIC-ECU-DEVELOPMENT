#ifndef PWMSAVECHANNEL_H
#define PWMSAVECHANNEL_H

#include "PWM.h"

static inline void PWM_SaveChannelConfig(Pwm_ConfigType *ConfigPtr)
{
    GroupPWM[ConfigPtr->Channel].Channel            = ConfigPtr->Channel;
    GroupPWM[ConfigPtr->Channel].ChannelClassType   = ConfigPtr->ChannelClassType;
    GroupPWM[ConfigPtr->Channel].IdleState          = ConfigPtr->IdleState;
    GroupPWM[ConfigPtr->Channel].Period             = ConfigPtr->Period;
    GroupPWM[ConfigPtr->Channel].Polarity           = ConfigPtr->Polarity;
    GroupPWM[ConfigPtr->Channel].Pulse              = ConfigPtr->Pulse;
    GroupPWM[ConfigPtr->Channel].initialize         = ConfigPtr->initialize;
    GroupPWM[ConfigPtr->Channel].NotificationCallback = ConfigPtr->NotificationCallback;
    GroupPWM[ConfigPtr->Channel].NotificationSaveArray = 1; // thông báo đã lưu cấu hình vào mảng
}
#endif