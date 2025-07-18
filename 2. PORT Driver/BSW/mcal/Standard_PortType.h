#ifndef STANDARD_PORTTYPE_H
#define STANDARD_PORTTYPE_H

/****************************************************
 * @typedef Dio_ChannelType
 * @brief Kiểu dữ liệu cho một kênh DIO
 * @details Đây là kiểu định danh cho một PIN cụ thể
 *          numberic ID of a DIO channel
 *****************************************************/
typedef enum
{
    // Port A
    PortA_Pin_0  = 0,   PortA_Pin_1  = 1,   PortA_Pin_2  = 2,   PortA_Pin_3  = 3,
    PortA_Pin_4  = 4,   PortA_Pin_5  = 5,   PortA_Pin_6  = 6,   PortA_Pin_7  = 7,
    PortA_Pin_8  = 8,   PortA_Pin_9  = 9,   PortA_Pin_10 = 10,  PortA_Pin_11 = 11,
    PortA_Pin_12 = 12,  PortA_Pin_13 = 13,  PortA_Pin_14 = 14,  PortA_Pin_15 = 15,

    // Port B
    PortB_Pin_0  = 16,  PortB_Pin_1  = 17,  PortB_Pin_2  = 18,  PortB_Pin_3  = 19,
    PortB_Pin_4  = 20,  PortB_Pin_5  = 21,  PortB_Pin_6  = 22,  PortB_Pin_7  = 23,
    PortB_Pin_8  = 24,  PortB_Pin_9  = 25,  PortB_Pin_10 = 26,  PortB_Pin_11 = 27,
    PortB_Pin_12 = 28,  PortB_Pin_13 = 29,  PortB_Pin_14 = 30,  PortB_Pin_15 = 31,

    // Port C
    PortC_Pin_0  = 32,  PortC_Pin_1  = 33,  PortC_Pin_2  = 34,  PortC_Pin_3  = 35,
    PortC_Pin_4  = 36,  PortC_Pin_5  = 37,  PortC_Pin_6  = 38,  PortC_Pin_7  = 39,
    PortC_Pin_8  = 40,  PortC_Pin_9  = 41,  PortC_Pin_10 = 42,  PortC_Pin_11 = 43,
    PortC_Pin_12 = 44,  PortC_Pin_13 = 45,  PortC_Pin_14 = 46,  PortC_Pin_15 = 47,

    // Port D
    PortD_Pin_0  = 48,  PortD_Pin_1  = 49,  PortD_Pin_2  = 50,  PortD_Pin_3  = 51,
    PortD_Pin_4  = 52,  PortD_Pin_5  = 53,  PortD_Pin_6  = 54,  PortD_Pin_7  = 55,
    PortD_Pin_8  = 56,  PortD_Pin_9  = 57,  PortD_Pin_10 = 58,  PortD_Pin_11 = 59,
    PortD_Pin_12 = 60,  PortD_Pin_13 = 61,  PortD_Pin_14 = 62,  PortD_Pin_15 = 63
} Dio_ChannelType;
/********************************************************************
 * @brief   Macro đưa ra một PIN tương ứng 
 * @details macro sẽ nhận vào một tham số ChannelId sau đó trả về mặt
 *          nạ của Pin tương ứng
 ********************************************************************/
#define GPIO_GET_PIN_MASK(ChannelId) (1 << (ChannelId % 16))

/********************************************************************
 * @brief   Macro đưa ra một port tương ứng
 * @details macro sẽ nhận vào một tham số sau đó trả về Port tương ứng
 *          nếu không đúng thì sẽ trả về con trỏ NULL_PIR 
 *          (NULL_PIR được định nghĩa trong file Std_Types.h)
 ********************************************************************/
#define GPIO_GET_PORT(PortId) ( (PortId == PORT_A) ? GPIOA :\
                                (PortId == PORT_B) ? GPIOB :\
                                (PortId == PORT_C) ? GPIOC :\
                                (PortId == PORT_D) ? GPIOD : NULL_PIR)

/****************************************************
 * @typedef Dio_PortType
 * @brief Kiểu dữ liệu cho một kênh DIO
 * @details Đây là kiểu định danh cho một PORT cụ thể
 *          numeric ID of a DIO Port
 *****************************************************/
typedef enum
{
    PORT_A = 0x00,
    PORT_B = 0x01,
    PORT_C = 0x02,
    PORT_D = 0x03
}Dio_PortType;

#endif