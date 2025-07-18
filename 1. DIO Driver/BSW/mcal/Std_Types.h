#ifndef STD_TYPES_H
#define STD_TYPES_H
/**************************************************
 * @file    std_Types.h
 * @brief   định nghĩa các kiểu dữ liệu chuẩn cho AUTOSAR
 * @details file này cung cấp các kiểu dữ liệu nền tảng độc lập
 *          cho phần cứng và trình biên dịch cho các module phần 
 *          mềm cơ bản trong AUTOSAR
 */
//voltage value definition 
#define STD_LOW  0x00U
#define STD_HIGH 0x01U 

#ifndef NULL_PTR
#define NULL_PIR ((void *)0)
#endif
//integer type data definition
typedef unsigned char       uint8;
typedef signed char         sint8;
typedef unsigned short      uint16;
typedef signed short        sint16;
typedef unsigned int        uint32;
typedef signed int          sint32;
typedef unsigned long long  uint64;
typedef signed long long    sint64;
//float type date definition
typedef float   float32;
typedef double  float64;

//version infor type
typedef struct 
{
    uint16 vendorID;
    uint16 moduleID;
    uint8 sw_major_version;
    uint8 sw_minor_version;
    uint8 sw_patch_version;
}Std_VersionInfoType;

#endif