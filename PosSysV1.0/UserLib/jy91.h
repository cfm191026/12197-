//
// Created by CFM on 2023/10/28.
//

#ifndef SUMOROBOT_JY60_H
#define SUMOROBOT_JY60_H
#include "main.h"
static float START_ANG=0;
void CopeCmdData(unsigned char ucData);
void JyMain(void);
float *GetAngle(void);
float *GetAcc(void);
float *GetGyro(void);
float GetYaw(void);
void JyInit(void);
void JYSetOrigin(void);
void SetYaw(float yaw);
#endif //SUMOROBOT_JY60_H
