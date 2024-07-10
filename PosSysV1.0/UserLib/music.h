//
// Created by CFM on 2024/6/16.
//

#ifndef POSSYSV1_0_MUSIC_H
#define POSSYSV1_0_MUSIC_H

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Music.h
  * @brief          : Header for Music.c file.
  *                   This file provides code for the configuration
  *                   of the Music instances
  * @author         : 千樊
  *	@time			      : 2021-10-12
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "main.h"
#include "buzzer.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PT */
#define Bass		0
#define	Alto		1
#define	Teble		2

#define	One_Beat				1
#define	One_TWO_Beat		2
#define	One_FOUR_Beat		4

/* USER CODE END PT */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */

void MUSIC_Analysis(void);

/* USER CODE END EFP */

#endif
