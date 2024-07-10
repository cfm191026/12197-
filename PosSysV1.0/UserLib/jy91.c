//
// Created by CFM on 2023/10/28.
//
#include "jy91.h"
#include "userUart.h"
#include "cmsis_os.h"
#include "REG.h"
#include "wit_c_sdk.h"


#define ACC_UPDATE        0x01
#define GYRO_UPDATE        0x02
#define ANGLE_UPDATE    0x04
#define MAG_UPDATE        0x08
#define READ_UPDATE        0x80
static float fAcc[3], fGyro[3], fAngle[3], fYaw;
static volatile char s_cDataUpdate = 0, s_cCmd = 0xff;
const uint32_t c_uiBaud[10] = {0, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

static void AutoScanSensor(void);

static void SensorUartSend(uint8_t *p_data, uint32_t uiSize);

static void CopeSensorData(uint32_t uiReg, uint32_t uiRegNum);

uint32_t uiBuad = 115200;
uint8_t ucRxData = 0;

static void SensorUartSend(uint8_t *p_data, uint32_t uiSize)
{
    HAL_UART_Transmit(&huart5, p_data, uiSize, uiSize * 4);
}

static void CopeSensorData(uint32_t uiReg, uint32_t uiRegNum)
{
    int i;
    for (i = 0; i < uiRegNum; i++)
    {
        switch (uiReg)
        {
            case AZ:
                s_cDataUpdate |= ACC_UPDATE;
                break;
            case GZ:
                s_cDataUpdate |= GYRO_UPDATE;
                break;
            case HZ:
                s_cDataUpdate |= MAG_UPDATE;
                break;
            case Yaw:
                s_cDataUpdate |= ANGLE_UPDATE;
                break;
            default:
                s_cDataUpdate |= READ_UPDATE;
                break;
        }
        uiReg++;
    }
}

void Usart5Init(int bound)
{
    huart5.Instance = USART3;
    huart5.Init.BaudRate = bound;
    huart5.Init.WordLength = UART_WORDLENGTH_8B;
    huart5.Init.StopBits = UART_STOPBITS_1;
    huart5.Init.Parity = UART_PARITY_NONE;
    huart5.Init.Mode = UART_MODE_TX_RX;
    huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart5.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart5) != HAL_OK)
    {
        Error_Handler();
    }
}

static void AutoScanSensor(void)
{
    const uint32_t c_uiBaud[9] = {4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
    int i, iRetry;
    for (i = 0; i < 9; i++)
    {
        uiBuad = c_uiBaud[i];
//        MX_UART4_Init();
        iRetry = 2;
        do
        {
            s_cDataUpdate = 0;
            WitReadReg(AX, 3);
            HAL_Delay(100);
            if (s_cDataUpdate != 0)
            {
                Serial1_Printf("find sensor\r\n\r\n");
                return;
            }
            iRetry--;
        } while (iRetry);
    }
    Serial1_Printf("can not find sensor\r\n");
    Serial1_Printf("please check your connection\r\n");
}


static void Delayms(uint16_t ucMs)
{
    osDelay(ucMs);
}

static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum)
{
    int i;
    for (i = 0; i < uiRegNum; i++)
    {
        switch (uiReg)
        {
//            case AX:
//            case AY:
            case AZ:
                s_cDataUpdate |= ACC_UPDATE;
                break;
//            case GX:
//            case GY:
            case GZ:
                s_cDataUpdate |= GYRO_UPDATE;
                break;
//            case HX:
//            case HY:
            case HZ:
                s_cDataUpdate |= MAG_UPDATE;
                break;
//            case Roll:
//            case Pitch:
            case Yaw:
                s_cDataUpdate |= ANGLE_UPDATE;
                break;
            default:
                s_cDataUpdate |= READ_UPDATE;
                break;
        }
        uiReg++;
    }
}

static void ShowHelp(void)
{
    Serial1_Printf("\r\n************************	 WIT_SDK_DEMO	************************");
    Serial1_Printf("\r\n************************          HELP           ************************\r\n");
    Serial1_Printf("UART SEND:a\\r\\n   Acceleration calibration.\r\n");
    Serial1_Printf(
            "UART SEND:m\\r\\n   Magnetic field calibration,After calibration send:   e\\r\\n   to indicate the end\r\n");
    Serial1_Printf("UART SEND:U\\r\\n   Bandwidth increase.\r\n");
    Serial1_Printf("UART SEND:u\\r\\n   Bandwidth reduction.\r\n");
    Serial1_Printf("UART SEND:B\\r\\n   Baud rate increased to 115200.\r\n");
    Serial1_Printf("UART SEND:b\\r\\n   Baud rate reduction to 9600.\r\n");
    Serial1_Printf("UART SEND:R\\r\\n   The return rate increases to 10Hz.\r\n");
    Serial1_Printf("UART SEND:r\\r\\n   The return rate reduction to 1Hz.\r\n");
    Serial1_Printf(
            "UART SEND:C\\r\\n   Basic return content: acceleration, angular velocity, angle, magnetic field.\r\n");
    Serial1_Printf("UART SEND:c\\r\\n   Return content: acceleration.\r\n");
    Serial1_Printf("UART SEND:h\\r\\n   help.\r\n");
    Serial1_Printf("******************************************************************************\r\n");
}




static void CmdProcess(void)
{
    switch (s_cCmd)
    {
        case 'a':
            if (WitStartAccCali() != WIT_HAL_OK)
                Serial1_Printf("\r\nSet AccCali Error\r\n");
            break;
        case 'm':
            if (WitStartMagCali() != WIT_HAL_OK)
                Serial1_Printf("\r\nSet MagCali Error\r\n");
            break;
        case 'e':
            if (WitStopMagCali() != WIT_HAL_OK)
                Serial1_Printf("\r\nSet MagCali Error\r\n");
            break;
        case 'u':
            if (WitSetBandwidth(BANDWIDTH_5HZ) != WIT_HAL_OK)
                Serial1_Printf("\r\nSet Bandwidth Error\r\n");
            break;
        case 'U':
            if (WitSetBandwidth(BANDWIDTH_256HZ) != WIT_HAL_OK)
                Serial1_Printf("\r\nSet Bandwidth Error\r\n");
            break;
        case 'B':
            if (WitSetUartBaud(WIT_BAUD_115200) != WIT_HAL_OK)
                Serial1_Printf("\r\nSet Baud Error\r\n");
            else
                Usart5Init(c_uiBaud[WIT_BAUD_115200]);
            break;
        case 'b':
            if (WitSetUartBaud(WIT_BAUD_9600) != WIT_HAL_OK)
                Serial1_Printf("\r\nSet Baud Error\r\n");
            else
                Usart5Init(c_uiBaud[WIT_BAUD_9600]);
            break;
        case 'R':
            if (WitSetOutputRate(RRATE_10HZ) != WIT_HAL_OK)
                Serial1_Printf("\r\nSet Rate Error\r\n");
            break;
        case 'r':
            if (WitSetOutputRate(RRATE_1HZ) != WIT_HAL_OK)
                Serial1_Printf("\r\nSet Rate Error\r\n");
            break;
        case 'C':
            if (WitSetContent(RSW_ACC | RSW_GYRO | RSW_ANGLE | RSW_MAG) != WIT_HAL_OK)
                Serial1_Printf("\r\nSet RSW Error\r\n");
            break;
        case 'c':
            if (WitSetContent(RSW_ACC) != WIT_HAL_OK)
                Serial1_Printf("\r\nSet RSW Error\r\n");
            break;
        case 'h':
            ShowHelp();
            break;
    }
    s_cCmd = 0xff;
}

void CopeCmdData(unsigned char ucData)
{
    static unsigned char s_ucData[50], s_ucRxCnt = 0;

    s_ucData[s_ucRxCnt++] = ucData;
    if (s_ucRxCnt < 3)return;                                        //Less than three data returned
    if (s_ucRxCnt >= 50) s_ucRxCnt = 0;
    if (s_ucRxCnt >= 3)
    {
        if ((s_ucData[1] == '\r') && (s_ucData[2] == '\n'))
        {
            s_cCmd = s_ucData[0];
            memset(s_ucData, 0, 50);//
            s_ucRxCnt = 0;
        }
        else
        {
            s_ucData[0] = s_ucData[1];
            s_ucData[1] = s_ucData[2];
            s_ucRxCnt = 2;

        }
    }
}


void JyInit(void)
{
    WitInit(WIT_PROTOCOL_NORMAL, 0x50);
    WitSerialWriteRegister(SensorUartSend);
    WitRegisterCallBack(SensorDataUpdata);
    WitDelayMsRegister(Delayms);
//    Serial1_Printf("\r\n********************** wit-motion normal example  ************************\r\n");
    AutoScanSensor();
    JyMain();
    JYSetOrigin();
}

void JyMain(void)
{
    CmdProcess();
    if (s_cDataUpdate)
    {
        for (int i = 0; i < 3; i++)
        {
            fAcc[i] = sReg[AX + i] / 32768.0f * 16.0f;
            fGyro[i] = sReg[GX + i] / 32768.0f * 2000.0f;
            fAngle[i] = sReg[Roll + i] / 32768.0f * 180.0f;
        }
        if (s_cDataUpdate & ACC_UPDATE)
        {
//            Serial1_Printf("acc:%.3f %.3f %.3f\r\n", fAcc[0], fAcc[1], fAcc[2]);
            s_cDataUpdate &= ~ACC_UPDATE;
        }
        if (s_cDataUpdate & GYRO_UPDATE)
        {
//            Serial1_Printf("gyro:%.3f %.3f %.3f\r\n", fGyro[0], fGyro[1], fGyro[2]);
            s_cDataUpdate &= ~GYRO_UPDATE;
        }
        if (s_cDataUpdate & ANGLE_UPDATE)
        {
//            Serial1_Printf("angle:%.3f %.3f %.3f\r\n", fAngle[0], fAngle[1], fAngle[2]);
            fYaw = (float) ((unsigned short) sReg[Yaw]) / 32768 * 180.0;
            s_cDataUpdate &= ~ANGLE_UPDATE;
        }
        if (s_cDataUpdate & MAG_UPDATE)
        {
//            Serial1_Printf("mag:%d %d %d\r\n", sReg[HX], sReg[HY], sReg[HZ]);
            s_cDataUpdate &= ~MAG_UPDATE;
        }
    }
}

void JYSetOrigin(void)
{
    START_ANG = GetYaw();
}

float *GetAngle(void)
{
    return fAngle;
}

float *GetAcc(void)
{
    return fAcc;
}

float *GetGyro(void)
{
    return fGyro;
}

float GetYaw(void)
{
    float cal_yaw = fYaw - START_ANG;
    if (cal_yaw > 360)
        cal_yaw -= 360;
    if (cal_yaw < 0)
        cal_yaw += 360;
    return cal_yaw;
}

void SetYaw(float yaw)
{
    if (yaw > GetYaw())
    {
        START_ANG = START_ANG - yaw + GetYaw();
    }
    else
    {
        START_ANG = START_ANG + yaw - GetYaw();
    }
}