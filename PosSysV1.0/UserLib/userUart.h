//
// Created by CFM on 2023/9/20.
//

#ifndef SUMOROBOT_USERUART_H
#define SUMOROBOT_USERUART_H

#include "usart.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include <assert.h>



void Serial1_SendByte(uint8_t Byte);
void Serial4_SendByte(uint8_t Byte);
void Serial5_SendByte(uint8_t Byte);
void Serial1_Printf(char *format, ...);
void Serial4_Printf(char *format, ...);
void Serial5_Printf(char *format, ...);
void Serial1_SendArray(uint8_t *Array, uint16_t Length);
void Serial4_SendArray(uint8_t *Array, uint16_t Length);
void Serial5_SendArray(uint8_t *Array, uint16_t Length);
uint16_t High8Low8ToDECT(uint8_t high, uint8_t low);
uint8_t CalBCC(uint8_t *P, int data_len);
#endif //SUMOROBOT_USERUART_H
