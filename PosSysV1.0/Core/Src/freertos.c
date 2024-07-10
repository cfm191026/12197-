/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "buzzer.h"
#include "music.h"
#include "userUart.h"
#include "as5600.h"
#include "pos.h"
#include "jy91.h"
#include "uartComm.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for BuzzerTask */
osThreadId_t BuzzerTaskHandle;
const osThreadAttr_t BuzzerTask_attributes = {
  .name = "BuzzerTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for JYTASK */
osThreadId_t JYTASKHandle;
const osThreadAttr_t JYTASK_attributes = {
  .name = "JYTASK",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartBuzzerTask(void *argument);
void StartJYTASK(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of BuzzerTask */
  BuzzerTaskHandle = osThreadNew(StartBuzzerTask, NULL, &BuzzerTask_attributes);

  /* creation of JYTASK */
  JYTASKHandle = osThreadNew(StartJYTASK, NULL, &JYTASK_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;)
    {
        Serial1_Printf("%d,%d,%f\n", POS_GetPosX(),POS_GetPosY(),GetYaw());
        SendPos();
        osDelay(10);
    }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartBuzzerTask */
/**
* @brief Function implementing the BuzzerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBuzzerTask */
void StartBuzzerTask(void *argument)
{
  /* USER CODE BEGIN StartBuzzerTask */
    /* Infinite loop */
    readyMusic();
    for (;;)
    {
        POS_CalPosEncSpeedAndPos();
        osDelay(5);
    }
  /* USER CODE END StartBuzzerTask */
}

/* USER CODE BEGIN Header_StartJYTASK */
/**
* @brief Function implementing the JYTASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartJYTASK */
void StartJYTASK(void *argument)
{
  /* USER CODE BEGIN StartJYTASK */
    /* Infinite loop */
    JyInit();
    for (;;)
    {
        JyMain();
//        Serial1_Printf("YAW=%f\n", GetYaw());
        osDelay(5);

    }
  /* USER CODE END StartJYTASK */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

