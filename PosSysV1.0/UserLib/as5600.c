//
// Created by CFM on 2024/6/16.
//
#include "as5600.h"
#include "math.h"


float angle_prev = 0;
int full_rotations = 0; // full rotation tracking;
float angle_d;                //GetAngle_Without_Track()的返回值
float angle_cd;                //GetAngle()的返回值

//角度相关全局变量
short relativeAngle = 0;//相对旋转角度
short totalAngle = 0;//总旋转角度
short numberofTurns = 0;//旋转圈数

static short previousquadrantNumber = 0;//上次象限
static short quadrantNumber = 0;//当前象限



#include "AS5600.h"
#include "i2c.h"


unsigned char write_reg(uint8_t which, unsigned char reg, unsigned char value)
{
    if (which == 1)
        return HAL_I2C_Mem_Write(&hi2c1, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 50);
    else
        return HAL_I2C_Mem_Write(&hi2c2, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 50);

}


unsigned char read_reg(uint8_t which, unsigned char reg, unsigned char *buf, unsigned short len)
{
    if (which == 1)
        return HAL_I2C_Mem_Read(&hi2c1, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 50);
    else
        return HAL_I2C_Mem_Read(&hi2c2, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 50);
}


void RelativeAngleCorrect(short startAngle, short realAngle)
{
    //相对角度获取，顺时针为正
    relativeAngle = realAngle - startAngle; //没变动则为0

    if (relativeAngle < 0) //把相对角度转换为0-360度表示
    {
        relativeAngle = relativeAngle + 360; //
    }

    //把0-360角度转换为-180到+180表示
    if (relativeAngle > 180)
    {
        relativeAngle = relativeAngle - 360;
    }
}


void checkQuadrant(void)
{
    //用象限来计算圈数，顺时针转为正
    /*
    //Quadrants:
    4  |  1
    ---|---
    3  |  2
    */
    //Quadrant 1
    if (relativeAngle > 0 && relativeAngle <= 90)
    {
        quadrantNumber = 1;
    }
    //Quadrant 2
    if (relativeAngle > 90 && relativeAngle <= 180)
    {
        quadrantNumber = 2;
    }
    //Quadrant 3
    if (relativeAngle > -180 && relativeAngle <= -90)
    {
        quadrantNumber = 3;
    }
    //Quadrant 4
    if (relativeAngle > -90 && relativeAngle < 0)
    {
        quadrantNumber = 4;
    }
    if (quadrantNumber != previousquadrantNumber) //如果象限改变
    {
        if (quadrantNumber == 1 && previousquadrantNumber == 4)
        {
            numberofTurns++; // 现在象限为1而上次为4，则顺时针旋转过0一次，圈数加1
        }
        if (quadrantNumber == 4 && previousquadrantNumber == 1)
        {
            numberofTurns--; // 现在象限为4而上次为1，则逆时针旋转过0一次，圈数减1
        }
        previousquadrantNumber = quadrantNumber;  //更新旧象限
    }
}


/*********************返回绝对编码器值(0-4096)*************************/
int AS5600_getEnc(uint8_t which)
{
    int16_t enc;
    uint8_t temp[2] = {0, 0};
    read_reg(which, Angle_Hight_Register_Addr, temp, 2);
    enc = ((int16_t) temp[0] << 8) | (temp[1]);
    return enc;
}


/*********************返回相对角度(弧度制)*************************/
float AS5600_getRadinAng(uint8_t which)
{
    float angle_d;
    int16_t in_angle;
    uint8_t temp[2] = {0, 0};
    read_reg(which, Angle_Hight_Register_Addr, temp, 2);
    //printf("%d  %d\n",temp[0],temp[1]);
    in_angle = ((int16_t) temp[0] << 8) | (temp[1]);
    angle_d = (float) in_angle * cpr / 4096;
    //printf("angle=%.2f\n",angle_d);

    return angle_d;
    //angle_d为弧度制，范围在0-6.28
}

/*********************返回绝对角度(角度制)*************************/
float AS5600_getAng(uint8_t which)
{
    float angle_d, angle;
    int16_t in_angle;
    uint8_t temp[2] = {0, 0};
    read_reg(which, Angle_Hight_Register_Addr, temp, 2);
    //printf("%d  %d\n",temp[0],temp[1]);
    in_angle = ((int16_t) temp[0] << 8) | (temp[1]);
    angle_d = (float) in_angle * cpr / 4096;
    //printf("angle=%.2f\n",angle_d);
    angle = (180.0 / PI) * angle_d;
    return angle;
}

/*********************返回角度差值(角度制)*************************/
float AS5600_getRelativeAng(uint8_t which)
{
    static uint8_t start_flag = 0;
    static float last_ang[2];
    float ang_diff;
    if (start_flag == 0)
    {
        start_flag = 1;
        last_ang[0] = AS5600_getAng(1);
        last_ang[1] = AS5600_getAng(2);
    }
    float ang_tem = AS5600_getAng(which);
    ang_diff = ang_tem - last_ang[which - 1];

    if (ang_diff > 180)//解决0°和360°交界问题
        ang_diff = ang_diff - 360;
    else if (ang_diff < -180)
        ang_diff = ang_diff + 360;

    if (ang_diff > 360 | ang_diff < -360)//as5600数据有误不更新上次角度 并不输出角度差
        ang_diff = 0;
    else
        last_ang[which - 1] = ang_tem;
    return ang_diff;
}

/*********************返回编码器差值(0-4096)*************************/
int AS5600_getRelativeEnc(uint8_t which)
{
    static uint8_t start_flag = 0;
    static int last_enc[2];
    int enc_diff;
    if (start_flag == 0)
    {
        start_flag = 1;
        last_enc[0] = AS5600_getEnc(1);
        last_enc[1] = AS5600_getEnc(2);
    }
    int enc_tem = AS5600_getEnc(which);
    enc_diff = enc_tem - last_enc[which - 1];

    if (enc_diff > 2048)//解决0°和360°交界问题
        enc_diff = enc_diff - 4096;
    else if (enc_diff < -2048)
        enc_diff = enc_diff + 4096;

    if (enc_diff > 4096 | enc_diff < -4096)//as5600数据有误不更新上次角度 并不输出角度差
        enc_diff = 0;
    else
        last_enc[which - 1] = enc_tem;
    return enc_diff;
}
