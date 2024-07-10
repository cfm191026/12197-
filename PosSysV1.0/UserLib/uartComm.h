//
// Created by CFM on 2024/7/1.
//

#ifndef POSSYSV1_0_UARTCOMM_H
#define POSSYSV1_0_UARTCOMM_H


//发送的数据为 包头 包尾 功能码 x坐标(mm) y坐标(mm) yaw角度(ang) x速度(mm/s) y速度(mm/s) yaw速度(ang/s)
#define SE_HEAD 0xFD//发送包头
#define SE_END 0xFE//发送包尾

#define SE_DATA_LEN 15//发送数据长度
#define SE_ARRAY_LEN 20//放松数组长度

#define RE_HEAD 0xFB//接收包头
#define RE_END 0xFC//接收包尾

#define RE_DATA_LEN 10//接收数据长度
#define RE_ARRAY_LEN 20//接收数组长度

#define SET_POS_XY_MODE 1
#define SET_POS_YAW_MODE 2
#define SET_POS_XY_AND_YAW_MODE 3

#define YAW_K 10.0//yaw转换倍数

#include "main.h"
void CalPos(void);
void SendPos(void);


#endif //POSSYSV1_0_UARTCOMM_H
