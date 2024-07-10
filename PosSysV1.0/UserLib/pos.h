//
// Created by CFM on 2024/6/17.
//

#ifndef POSSYSV1_0_POS_H
#define POSSYSV1_0_POS_H

#include "main.h"
#include "as5600.h"

#define AUTO_RELOAD_REGISTER_POS 20000
#define  CAL_SPEED_TIME_POS 10 //ms
#define  CAL_SPEED_FRE_POS 100 //hz
#define ENCODER_L_DIR 1
#define ENCODER_R_DIR 1
#define ENCODER_L_HTIM htim3
#define ENCODER_R_HTIM htim4
#define PI 3.14159
#define INTERVAL_TIME 0.01 //运行间隔(s)

void POS_CalPosEncSpeedAndPos(void);
int POS_GetPosX(void);
int POS_GetPosY(void);
void POS_SetPosX(int x);
void POS_SetPosY(int y);
void POS_SetYaw(float yaw);
float POS_GetYaw(void);
int POS_GetSpeedX(void);
int POS_GetSpeedY(void);
float POS_GetYawSpeed(void);


#endif //POSSYSV1_0_POS_H
