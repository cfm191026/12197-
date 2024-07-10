//
// Created by CFM on 2023/9/20.
//

#include "userUart.h"


void Serial1_SendByte(uint8_t Byte)
{
    static uint8_t byte;
    byte = Byte;
    uint8_t *p = &byte;
    HAL_UART_Transmit(&huart1, p, 1, 2);
}



void Serial4_SendByte(uint8_t Byte)
{
    static uint8_t byte;
    byte = Byte;
    uint8_t *p = &byte;
    HAL_UART_Transmit(&huart4, p, 1, 2);
}

void Serial5_SendByte(uint8_t Byte)
{
    static uint8_t byte;
    byte = Byte;
    uint8_t *p = &byte;
    HAL_UART_Transmit(&huart5, p, 1, 2);
}

void Serial1_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++)
    {
        Serial1_SendByte(Array[i]);
    }
}

void Serial4_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++)
    {
        Serial4_SendByte(Array[i]);
    }
}

void Serial5_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++)
    {
        Serial5_SendByte(Array[i]);
    }
}

void Serial1_SendString(char *String)
{
    uint16_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        Serial1_SendByte(String[i]);
    }
}

void Serial4_SendString(char *String)
{
    uint16_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        Serial4_SendByte(String[i]);
    }
}

void Serial5_SendString(char *String)
{
    uint16_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        Serial5_SendByte(String[i]);
    }
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}


void Serial1_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial1_SendString(String);
}

void Serial4_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial4_SendString(String);
}

void Serial5_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial5_SendString(String);
}

int count_strlen(const char *p)
{
    assert(p != NULL);
    int count = 0;
    while (*p != '\0')
    {
        count++;
        p++;
    }
    return count;
}


uint16_t High8Low8ToDECT(uint8_t high, uint8_t low)
{
    int dec;
    dec = high;
    dec <<= 8;
    dec = dec | low;
    dec = dec & 0XFFFF;
    unsigned char t = high & 0X80;
    if (t == 0x80)
    {
        dec -= 1;
        dec = ~dec;
        dec = dec & 0xFFFF;
        dec = -dec;
    }
    return dec;
}

uint8_t CalBCC(uint8_t *P, int data_len)
{
    uint8_t BCC_data = 0x00;
    for (int i = 0; i < data_len; i++)
    {
        BCC_data = BCC_data ^ P[i];
    }
    return BCC_data;
}