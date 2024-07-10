//
// Created by CFM on 2024/6/17.
//

#include "pos.h"
#include "jy91.h"
#include <math.h>

#define WHEEL_CONST 331.6125 //轮子周长/360*1000
#define L 34.5 //轮子到中心的距离 单位为mm
#define sin45 0.707106
#define cos45 0.707106
static float L_SPEED, R_SPEED;
static double POS_X = 0, POS_Y = 0;
static double POS_X_mm = 0, POS_Y_mm = 0;
static double SPEED_X = 0, SPEED_Y = 0;
static double SPEED_YAW = 0;


void POS_CalPosEncSpeedAndPos(void)
{
    static float last_yaw;
    float dis_l_d = (double) AS5600_getRelativeEnc(1) * WHEEL_CONST * 360 / 4096;
    float dis_r_d = (double) AS5600_getRelativeEnc(2) * WHEEL_CONST * 360 / 4096;
    float yaw_gyro = GetYaw();
//    float yaw_gyro = 0;
    float yaw_gyro_diff = yaw_gyro - last_yaw;
    last_yaw = yaw_gyro;
//    float yaw_gyro_diff = 0;
//    dis_l_d = dis_l_d - (PI / 180.0) * yaw_gyro_diff * L;
//    dis_r_d = dis_r_d - (PI / 180.0) * yaw_gyro_diff * L;
    double robot_dx = -sin45 * dis_l_d - sin45 * dis_r_d;
    double robot_dy = -cos45 * dis_l_d + cos45 * dis_r_d;
    SPEED_X = robot_dx / INTERVAL_TIME / 1000;
    SPEED_Y = robot_dy / INTERVAL_TIME / 1000;
    SPEED_YAW = yaw_gyro_diff / INTERVAL_TIME;

    if (yaw_gyro > 0 && yaw_gyro < 90)
    {
        yaw_gyro = yaw_gyro * PI / 180;
        POS_Y += robot_dy * cosf(yaw_gyro) + robot_dx * sinf(yaw_gyro);
        POS_X += robot_dx * cosf(yaw_gyro) - robot_dy * sinf(yaw_gyro);
    }
    else if (yaw_gyro > 90 && yaw_gyro < 180)
    {
        yaw_gyro = 180 - yaw_gyro;
        yaw_gyro = yaw_gyro * PI / 180;
        POS_Y += -robot_dy * cosf(yaw_gyro) + robot_dx * sinf(yaw_gyro);
        POS_X += -robot_dx * cosf(yaw_gyro) - robot_dy * sinf(yaw_gyro);
    }
    else if (yaw_gyro > 180 && yaw_gyro < 270)
    {
        yaw_gyro = yaw_gyro - 180;
        yaw_gyro = yaw_gyro * PI / 180;
        POS_Y += -robot_dy * cosf(yaw_gyro) - robot_dx * sinf(yaw_gyro);
        POS_X += -robot_dx * cosf(yaw_gyro) + robot_dy * sinf(yaw_gyro);
    }
    else if (yaw_gyro > 270 && yaw_gyro < 360)
    {
        yaw_gyro = 360 - yaw_gyro;
        yaw_gyro = yaw_gyro * PI / 180;
        POS_Y += +robot_dy * cosf(yaw_gyro) - robot_dx * sinf(yaw_gyro);
        POS_X += +robot_dx * cosf(yaw_gyro) + robot_dy * sinf(yaw_gyro);
    }
    else if (yaw_gyro == 0)
    {
        POS_Y += robot_dy;
        POS_X += robot_dx;
    }
    else if (yaw_gyro == 90)
    {
        POS_Y += robot_dx;
        POS_X += -robot_dy;
    }
    else if (yaw_gyro == 180)
    {
        POS_Y += -robot_dy;
        POS_X += -robot_dx;
    }
    else if (yaw_gyro == 270)
    {
        POS_Y += -robot_dx;
        POS_X += robot_dy;
    }
    else if (yaw_gyro == 360)
    {
        POS_Y += robot_dy;
        POS_X += robot_dx;
    }
    POS_X_mm = POS_X / 1000;
    POS_Y_mm = POS_Y / 1000;
//    Serial1_Printf("%f,%f,%f,%f\n", GetYaw(), yaw, POS_X, POS_Y);
}


int POS_GetPosX(void)
{
    int pos_x = (int) POS_X_mm;
    return pos_x;
}

int POS_GetSpeedX(void)
{
    return (int) SPEED_X;
}

int POS_GetPosY(void)
{
    int pos_y = (int) POS_Y_mm;
    return pos_y;
}

int POS_GetSpeedY(void)
{
    return (int) SPEED_Y;
}

//单位为mm
void POS_SetPosX(int x)
{
    POS_X = (float) x;
}

//单位为mm
void POS_SetPosY(int y)
{
    POS_Y = (float) y;
}

float POS_GetYawAng(void)
{
    return GetYaw();
}

void POS_SetYawAng(float yaw)
{
    SetYaw(yaw);
}

float POS_GetYawSpeed(void)
{
    return SPEED_YAW;
}