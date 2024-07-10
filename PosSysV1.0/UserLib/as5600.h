//
// Created by CFM on 2024/6/16.
//

#ifndef POSSYSV1_0_AS5600_H
#define POSSYSV1_0_AS5600_H
#include "main.h"

extern uint8_t data[2];

extern float angle_d;				//GetAngle_Without_Track()的返回值
extern float angle_cd;				//GetAngle()的返回值


#ifndef __AS5600_H__
#define __AS5600_H__

#include "main.h"
#include <stdio.h>
#define PI					3.14159265358979f
#define cpr (float)(2.0f*PI)
#define Slave_Addr                0x36<<1//设备从地址
#define Write_Bit                 0	   //写标记
#define Read_Bit                  1    //读标记
#define Angle_Hight_Register_Addr 0x0C //寄存器高位地址
#define Angle_Low_Register_Addr   0x0D //寄存器低位地址



//角度相关的全项目全局变量
extern short relativeAngle;//相对旋转角度
extern short totalAngle;//总旋转角度
extern short numberofTurns;//旋转圈数

unsigned char read_reg(uint8_t which,unsigned char reg, unsigned char* buf, unsigned short len);
unsigned char write_reg(uint8_t which,unsigned char reg, unsigned char value);

int AS5600_getEnc(uint8_t which);
float AS5600_getRadinAng(uint8_t which);
float AS5600_getAng(uint8_t which);
float AS5600_getRelativeAng(uint8_t which);
int AS5600_getRelativeEnc(uint8_t which);

#endif




#endif //POSSYSV1_0_AS5600_H
