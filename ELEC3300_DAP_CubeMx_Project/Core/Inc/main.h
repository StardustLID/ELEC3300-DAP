/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

extern I2S_HandleTypeDef hi2s3;
extern DMA_HandleTypeDef hdma_spi3_tx;

extern SD_HandleTypeDef hsd;
extern DMA_HandleTypeDef hdma_sdio_rx;
extern DMA_HandleTypeDef hdma_sdio_tx;

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern SRAM_HandleTypeDef hsram1;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern uint8_t play_wav;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED0_Pin GPIO_PIN_2
#define LED0_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_3
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_4
#define LED2_GPIO_Port GPIOE
#define Buzzer_Pin GPIO_PIN_5
#define Buzzer_GPIO_Port GPIOE
#define Rot_Enc_But_Pin GPIO_PIN_6
#define Rot_Enc_But_GPIO_Port GPIOE
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOC
#define Button_0_Pin GPIO_PIN_0
#define Button_0_GPIO_Port GPIOB
#define Button_1_Pin GPIO_PIN_1
#define Button_1_GPIO_Port GPIOB
#define EEPROM_WP_Pin GPIO_PIN_12
#define EEPROM_WP_GPIO_Port GPIOB
#define LCD_BLK_Pin GPIO_PIN_12
#define LCD_BLK_GPIO_Port GPIOD
#define LCD__PREIRQ_Pin GPIO_PIN_6
#define LCD__PREIRQ_GPIO_Port GPIOC
#define Jack_detect_Pin GPIO_PIN_6
#define Jack_detect_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */
#define LED_ON 0
#define LED_OFF 1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
