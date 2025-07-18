#include "DIO.h"
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>

/****************************************************************
 * @brief Đọc trạng thái của một kênh DIO
 * @details Đọc trạng thái hiện tại của một kênh DIO trong Port
 * @param[in] ChannelId ID của kênh DIO cần đọc
 * @return STD_LOW: trả về trạng thái đọc được
*****************************************************************/
Dio_ChannelType Dio_ReadChannel(Dio_ChannelType channeId)
{
    GPIO_TypeDef *GPIO_Port = DIO_GET_PORT(channeId);;
    uint16 GPIO_Pin = GPIO_GET_PIN_MASK(channeId);
 
    if(GPIO_Port == NULL_PIR)
    {
        return 0;
    }

    if(GPIO_ReadInputDataBit(GPIO_Port, GPIO_Pin) == Bit_SET)
    {
        return STD_HIGH;
    }
    else
    {
        return STD_LOW;
    }
}
/********************************************************************
 * @brief ghi mức điện áp cho một kênh DIO
 * @details ghi mức điện áp cho một kênh DIO trong Port
 * @param[in] ChannelId ID của kênh DIO cần đọc và Level: mức điện áp
*********************************************************************/
void  Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    GPIO_TypeDef *GPIO_Port = DIO_GET_PORT(ChannelId);;
    uint16 GPIO_Pin = GPIO_GET_PIN_MASK(ChannelId);

    if(GPIO_Port == NULL_PIR)
    {
        return 0;
    }

    if(Level == STD_HIGH)
    {
        GPIO_SetBits(GPIO_Port, GPIO_Pin);
    }
    else
    {
        GPIO_ResetBits(GPIO_Port, GPIO_Pin);
    }
}

/****************************************************************
 * @brief Đọc trạng thái của một PORT
 * @details Đọc trạng thái hiện tại của 1 Port
 * @param[in] PortId ID của PORT cần đọc
 * @return Dio_PortLevelType: trả về trạng thái của port
*****************************************************************/
Dio_PortLevelType Dio_ReadPort (Dio_PortType PortId)
{
    GPIO_TypeDef *GPIO_Port;
    GPIO_Port = GPIO_GET_PORT(PortId);
    if(GPIO_Port == NULL_PIR)
    {
        return 0;
    }
    return (Dio_PortLevelType)GPIO_ReadInputData(GPIO_Port);
}

/****************************************************************
 * @brief ghi mức điện áp cho một PORT
 * @details Cấu hình mức điện áp cho một PORT
 * @param[in] PortId ID của PORT cần đọc và Level: mức điện áp
*****************************************************************/
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    GPIO_TypeDef *GPIO_Port;
    GPIO_Port = GPIO_GET_PORT(PortId);
    if(GPIO_Port == NULL_PIR)
    {
        return 0;
    }
    GPIO_Write(GPIO_Port, Level);
}

/*******************************************************************
 * @brief Đọc trạng thái của một group
 * @details Đọc trạng thái hiện tại của 1 group
 *          - Group: là những PIN nằm cùng PORT - liền kề nhau
 *          - Mask: lấy những PIN cần đọc 
 *          - Offset: là vị trí của group so với bit LSB. để sau này
 *          lấy thông tin của Offset dùng để dịch kết quả
 * @param[in] ChannelGroupIdPtr: struct chứa thông của group cần đọc
 * @return Dio_PortLevelType: trả về trạng thái của group.
 *          Kết quả này sẽ bị dịch về bit LSB trong thanh ghi 16 bit
********************************************************************/
Dio_PortLevelType Dio_ReadChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr)
{
    GPIO_TypeDef *GPIO_Port;
    GPIO_Port = GPIO_GET_PORT(ChannelGroupIdPtr->port);

    if(GPIO_Port == NULL_PIR)
    {
        return 0;
    }

    uint16 PortData = GPIO_ReadInputData(GPIO_Port);

    return (Dio_PortLevelType) ( (PortData & ChannelGroupIdPtr->mask) >>  ChannelGroupIdPtr->offset);
}

/*******************************************************************
 * @brief ghi mức điện áp cho một group
 * @details Cấu hình mức điện áp cho một group
 *          - Group: là những PIN nằm cùng PORT - liền kề nhau
 *          - Mask: lấy những PIN cần cấu hình 
 *          - Offset: là vị trí của group so với bit LSB. để sau này
 *          lấy thông tin của Offset dùng để dịch
 * @param[in] ChannelGroupIdPtr: struct chứa thông của group cần đọc
 *            Level: mức điện áp
********************************************************************/
void Dio_WriteChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level)
{
    GPIO_TypeDef *GPIO_Port;
    GPIO_Port = GPIO_GET_PORT(ChannelGroupIdPtr->port);

    if(GPIO_Port == NULL_PIR)
    {
        return 0;
    }

    uint16 PortData = GPIO_ReadInputData(GPIO_Port);

    PortData &= ~(ChannelGroupIdPtr->mask); // xóa trạng thái các bit cần cấu hình mức điện áp
    PortData |= ((Level << ChannelGroupIdPtr->offset) & (ChannelGroupIdPtr->mask)); // gán mức điện áp cần thay đổi (ghi)

    GPIO_Write(GPIO_Port, PortData);
}

/****************************************************************
 * @brief Hàm đảo giá trị logic của một PIN
 * @details Ban đầu sẽ đọc trạng thái hiện tại của PIN. sau đó đảo
 *          lại trạng thái (ngược với trạng thái hiện tại)
 * @param[in] ChannelId ID của kênh DIO cần đảo trạng thái
 * @return Dio_LevelType: trạng thái logic mới của PIN sau khi đảo
*****************************************************************/
Dio_LevelType Dio_FlipChannel (Dio_ChannelType ChannelId)
{
    Dio_ChannelType currentLevel = Dio_ReadChannel(ChannelId); 

    if(currentLevel == STD_HIGH)
    {
        Dio_WriteChannel(ChannelId, STD_LOW);
        return STD_LOW;
    }
    else
    {
        Dio_WriteChannel(ChannelId, STD_HIGH);
        return STD_HIGH;    
    }
}
/****************************************************************
 * @brief Hàm cấu hình các PIN không nằm liền kề (No Group)
 * @details Ban đầu sẽ đọc trạng thái hiện tại của các PIN cần cấu 
 *          hình. Sau đó gán mức điện áp mong muốn vào các PIN đó.
 *          Lưu ý không làm thay đổi trạng thái của các PIN khác.
 * @param[in] PortId: Port muốn cấu hình
 * @param[in] Level: Mức điện áp cấu hình cho các PIN. thông số này
 *                   truyền vào dưới dạng 16 bit.
 * @param[in] Mask: các PIN muốn cấu hình
*****************************************************************/
void Dio_MaskedWritePort(Dio_PortType PortId, Dio_PortLevelType Level, Dio_PortLevelType Mask)
{
    GPIO_TypeDef *GPIO_Port = GPIO_GET_PORT(PortId);
    if(GPIO_Port == NULL_PIR)
    {
        return;
    }
    uint16_t current = GPIO_Port->ODR; // đọc trạng thái trước khi thay đổi
    // giữ nguyên các bit không cần cấu hình, cập nhật bit trong mask
    uint16_t new_value = (current & ~Mask) | (Level & Mask);  
    //GPIO_Port->ODR = new_value;
    GPIO_Write(GPIO_Port, new_value);
}

/******************************************************************
 * @brief PortDriver_GetVersionInfo
 * @details Hàm chứa version của phần mềm. Mỗi khi cập nhật đều phải
 *          thay đổi version, tùy theo sự thay lớn hay nhỏ
 ******************************************************************/
void Dio_GetVersionInfo(Std_VersionInfoType *VersionInfo)
{
    //establish: 19 - 6 - 2025 (day - month - year)
    VersionInfo->moduleID = 2025;
    VersionInfo->sw_major_version = 1;
    VersionInfo->sw_minor_version = 0;
    VersionInfo->sw_patch_version = 0;
}