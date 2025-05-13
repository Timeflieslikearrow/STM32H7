/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CP_ADC_Pin GPIO_PIN_4
#define CP_ADC_GPIO_Port GPIOA
#define TEMP_ADC_Pin GPIO_PIN_6
#define TEMP_ADC_GPIO_Port GPIOA
#define E_RS485_RX_Pin GPIO_PIN_7
#define E_RS485_RX_GPIO_Port GPIOE
#define E_RS485_TX_Pin GPIO_PIN_8
#define E_RS485_TX_GPIO_Port GPIOE
#define E_RS485_DE_Pin GPIO_PIN_9
#define E_RS485_DE_GPIO_Port GPIOE
#define DIP_SW1_Pin GPIO_PIN_10
#define DIP_SW1_GPIO_Port GPIOE
#define DIP_SW2_Pin GPIO_PIN_11
#define DIP_SW2_GPIO_Port GPIOE
#define DIP_SW3_Pin GPIO_PIN_12
#define DIP_SW3_GPIO_Port GPIOE
#define State_E_Pin GPIO_PIN_14
#define State_E_GPIO_Port GPIOE
#define K_RS485_TX_Pin GPIO_PIN_14
#define K_RS485_TX_GPIO_Port GPIOB
#define K_RS485_RX_Pin GPIO_PIN_15
#define K_RS485_RX_GPIO_Port GPIOB
#define RS232A_TX_Pin GPIO_PIN_8
#define RS232A_TX_GPIO_Port GPIOD
#define RS232A_RX_Pin GPIO_PIN_9
#define RS232A_RX_GPIO_Port GPIOD
#define K_RS485_EN_Pin GPIO_PIN_10
#define K_RS485_EN_GPIO_Port GPIOD
#define Relay_EN_Pin GPIO_PIN_15
#define Relay_EN_GPIO_Port GPIOD
#define CP_PWM_Pin GPIO_PIN_8
#define CP_PWM_GPIO_Port GPIOA
#define STATE_LED_Pin GPIO_PIN_3
#define STATE_LED_GPIO_Port GPIOD
#define Relay_Welding_Pin GPIO_PIN_4
#define Relay_Welding_GPIO_Port GPIOD
#define RS232D_TX_Pin GPIO_PIN_5
#define RS232D_TX_GPIO_Port GPIOD
#define RS232D_RX_Pin GPIO_PIN_6
#define RS232D_RX_GPIO_Port GPIOD
#define CP_PWM_IN_Pin GPIO_PIN_5
#define CP_PWM_IN_GPIO_Port GPIOB
#define RCD_Fault_Pin GPIO_PIN_1
#define RCD_Fault_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
#include <stdio.h>
#include <string.h>
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
