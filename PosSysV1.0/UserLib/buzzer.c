//
// Created by CFM on 2024/6/16.
//
#include "buzzer.h"
#include "tim.h"
#include "cmsis_os.h"

void setBuzzerFrequency(uint32_t frequency)
{
    __HAL_TIM_SET_AUTORELOAD(&htim2, 1000000 / frequency - 1);    //根据频率设置加载值
    if (frequency == 0)
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
    else
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, (1000000 / frequency - 1) / 2);   //设置占空比
}


void PassionBuzzer_Set_Freq_Duty(uint16_t freq)
{
    __HAL_TIM_SET_AUTORELOAD(&htim2, 1000000 / freq - 1);    //根据频率设置加载值
    if (freq == 0)
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
    else
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, (1000000 / freq - 1) / 2);   //设置占空比
}

void readyMusic(void)
{
    setBuzzerFrequency(262);
    osDelay(200);
    setBuzzerFrequency(523);
    osDelay(200);
    setBuzzerFrequency(1046);
    osDelay(200);
    setBuzzerFrequency(0);
}
