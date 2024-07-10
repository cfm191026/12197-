//
// Created by CFM on 2024/7/1.
//
#include "uartComm.h"
#include "pos.h"
#include "jy91.h"
#include "userUart.h"

static uint8_t SEND_POS_DATA[SE_ARRAY_LEN];
static uint8_t uart5_data[RE_ARRAY_LEN];
static uint8_t uart5_storeNum = 0;

//数据格式: 包头 x位置高位 x位置低位 y位置高位 y位置低位 yaw角度高位 yaw角度低位 x速度高位 x速度低位 y速度高位 y速度低位 yaw速度高位 yaw速度低位 bcc 包尾（yaw数据都放大了100倍）
void CalPos(void)
{
    int pos_x, pos_y, yaw_ang, speed_x, speed_y, speed_yaw;
    uint8_t store_num = 0;//存储数据下标
    pos_x = POS_GetPosX();
    pos_y = POS_GetPosY();
    yaw_ang = (int) (GetYaw() * YAW_K);//放大100倍
    speed_x = POS_GetSpeedX();
    speed_y = POS_GetSpeedY();
    speed_yaw = (int) (POS_GetYawSpeed() * YAW_K);//放大100倍

    SEND_POS_DATA[store_num++] = SE_HEAD;//包头

    uint8_t high_byte = (pos_x >> 8) & 0xFF;//x坐标
    uint8_t low_byte = pos_x & 0xFF;
    SEND_POS_DATA[store_num++] = high_byte;
    SEND_POS_DATA[store_num++] = low_byte;

    high_byte = (pos_y >> 8) & 0xFF;//y坐标
    low_byte = pos_y & 0xFF;
    SEND_POS_DATA[store_num++] = high_byte;
    SEND_POS_DATA[store_num++] = low_byte;

    high_byte = (yaw_ang >> 8) & 0xFF;//yaw角度
    low_byte = yaw_ang & 0xFF;
    SEND_POS_DATA[store_num++] = high_byte;
    SEND_POS_DATA[store_num++] = low_byte;

    high_byte = (speed_x >> 8) & 0xFF;//x速度
    low_byte = speed_x & 0xFF;
    SEND_POS_DATA[store_num++] = high_byte;
    SEND_POS_DATA[store_num++] = low_byte;

    high_byte = (speed_y >> 8) & 0xFF;//y速度
    low_byte = speed_y & 0xFF;
    SEND_POS_DATA[store_num++] = high_byte;
    SEND_POS_DATA[store_num++] = low_byte;

    high_byte = (speed_yaw >> 8) & 0xFF;//yaw角度
    low_byte = speed_yaw & 0xFF;
    SEND_POS_DATA[store_num++] = high_byte;
    SEND_POS_DATA[store_num++] = low_byte;

    SEND_POS_DATA[store_num++] = CalBCC(SEND_POS_DATA, SE_DATA_LEN - 2);
    SEND_POS_DATA[store_num] = SE_END;//包尾
}

void SendPos(void)
{
    for (int i = 0; i < SE_DATA_LEN; ++i)
    {
        Serial5_SendByte(SEND_POS_DATA[i]);
//        Serial1_SendByte(SEND_POS_DATA[i]);
    }
}

void CalAndSetPosData(uint8_t *p)
{
    uint16_t write_data[1];
    if (CalBCC(p, RE_DATA_LEN - 2) == p[RE_DATA_LEN - 2])//判断校验位是否正确
    {
        if (p[1] == SET_POS_XY_MODE)
        {
            int set_pos_x = (int16_t) High8Low8ToDECT(p[2], p[3]);
            int set_pos_y = (int16_t) High8Low8ToDECT(p[4], p[5]);

            POS_SetPosX(set_pos_x);
            POS_SetPosY(set_pos_y);
        }
        else if (p[1] == SET_POS_YAW_MODE)
        {
            float set_yaw = ((float) ((int16_t) High8Low8ToDECT(p[6], p[7]))) / YAW_K;
            POS_SetYaw(set_yaw);
        }
        else if (p[1] == SET_POS_XY_AND_YAW_MODE)
        {
            int set_pos_x = (int16_t) High8Low8ToDECT(p[2], p[3]);
            int set_pos_y = (int16_t) High8Low8ToDECT(p[4], p[5]);
            float set_yaw = ((float) ((int16_t) High8Low8ToDECT(p[6], p[7]))) / YAW_K;
            POS_SetPosX(set_pos_x);
            POS_SetPosY(set_pos_y);
            POS_SetYaw(set_yaw);
        }
    }
}

void Uart5_Interrupt(uint8_t data)
{
    if (data == RE_HEAD || uart5_storeNum > 0)
    {
        uart5_data[uart5_storeNum] = data;
        uart5_storeNum++;
    }
    else
        uart5_storeNum = 0;
    if (uart5_storeNum == 11)
    {
        if (uart5_data[10] == RE_END)
        {
            CalAndSetPosData(uart5_data);
            memset(uart5_data, 0, sizeof(uart5_data));//数组清零
            uart5_storeNum = 0;
        }
        else
        {
            uart5_storeNum = 0;///丢弃数据
            memset(uart5_data, 0, sizeof(uart5_data));//数组清零
        }
    }
}
