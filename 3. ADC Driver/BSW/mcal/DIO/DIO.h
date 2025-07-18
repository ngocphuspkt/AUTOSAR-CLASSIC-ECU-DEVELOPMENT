/****************************************************** 
 * @brief   Header file DIO
 * @details Chứa các hàm dùng để đọc, ghi các PIN, PORT
 * @author  Trịnh Ngọc Phú - 2411
 * @date    2025
********************************************************/
#ifndef DIO_H
#define DIO_H
#include "Std_Types.h"
#include "Standard_PortType.h"

/**********************************************************
 * macro xác định Port tương ứng dựa trên ChannelId
***********************************************************/
#define DIO_GET_PORT(ChannelId) \
    (((ChannelId) < 16) ? GPIOA : \
     ((ChannelId) < 32) ? GPIOB : \
     ((ChannelId) < 48) ? GPIOC : \
     ((ChannelId) < 64) ? GPIOD : \
     NULL_PIR)

/*******************************************************
 * @typedef Dio_ChannelGroupType
 * @brief Kiểu cấu trúc chứa thông tin các nhóm PIN cần
 *        làm việc
 * @details Nhóm PIN là những PIN nằm liền kề nhau trong
 *          cùng một Port.
 *******************************************************/
typedef struct 
{
    uint16 mask; // các bit cần làm việc
    uint8  offset; // vị trí của nhóm so với LSB
    Dio_PortType port; // Port nào cần làm việc
} Dio_ChannelGroupType;
/*******************************************************
 * @typedef Dio_LevelType
 * @brief   Mức điện áp của PIN
 * @details PIN chỉ có 2 mức điện áp: Low và High. Hai kiểu
 *          này được định nghĩa trong file Std_Types.h
 *******************************************************/
typedef uint8 Dio_LevelType;
/*******************************************************
 * @typedef Dio_PortLevelType
 * @brief   Đọc mức điện áp các PIN trong Port
 * @details kiểu dữ liệu sẽ chứa các mức điện áp của PIN
 *          trong Port. Mỗi bit sẽ là mức điện áp của một
 *          PIN (16 PIN = 16 bit)
 *******************************************************/
typedef uint16 Dio_PortLevelType;

Dio_ChannelType Dio_ReadChannel(Dio_ChannelType channeId);
void  Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);
Dio_PortLevelType Dio_ReadPort (Dio_PortType PortId);
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);
Dio_PortLevelType Dio_ReadChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr);
void Dio_WriteChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level);
Dio_LevelType Dio_FlipChannel (Dio_ChannelType ChannelId);
void Dio_MaskedWritePort (Dio_PortType PortId, Dio_PortLevelType Level, Dio_PortLevelType Mask);
void Dio_GetVersionInfo(Std_VersionInfoType *VersionInfo);

#endif