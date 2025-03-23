/**
  ******************************************************************************
  * File Name          : app_display.c
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

/* Includes ------------------------------------------------------------------*/
#include "app_display.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "lcd_io.h"
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

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
/* USER CODE BEGIN PV */

uint16_t red = 0b1111100000000000;
uint16_t green = 0b0000011111100000;
uint16_t blue = 0b0000000000011111;
uint16_t cyan = 0b0000011111111111;
uint16_t magenta = 0b11111000000111111;

uint8_t rect[2048];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef huart3;

/* USER CODE END 0 */

/**
 * Initialize DISPLAY application
 */
void MX_DISPLAY_Init(void)
{
  /* USER CODE BEGIN MX_DISPLAY_Init 0 */
	int32_t ret;
	uint32_t val;
	uint32_t x_size;
	uint32_t y_size;

	char msg_out[64];

  /* USER CODE END MX_DISPLAY_Init 0 */
  if(BSP_LCD_Init(0, LCD_ORIENTATION_PORTRAIT) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN MX_DISPLAY_Init 1 */
  sprintf(msg_out, "LCD Initialized.\r\n");
	HAL_UART_Transmit(&huart3, (uint8_t *)msg_out, strlen(msg_out)+1, 0xff);

  ret = BSP_LCD_SetOrientation(0, LCD_ORIENTATION_LANDSCAPE);
  sprintf(msg_out, "Set Orientation result: %ld\r\n", ret);
	HAL_UART_Transmit(&huart3, (uint8_t *)msg_out, strlen(msg_out)+1, 0xff);

  ret = BSP_LCD_DisplayOn(0);
  sprintf(msg_out, "Display On result: %ld\r\n", ret);
	HAL_UART_Transmit(&huart3, (uint8_t *)msg_out, strlen(msg_out)+1, 0xff);
/*
  ret = BSP_LCD_SetDisplayWindow(0, 120, 120, 64, 64);
  sprintf(msg_out, "Set Window result: %ld\r\n", ret);
	HAL_UART_Transmit(&huart3, (uint8_t *)msg_out, strlen(msg_out)+1, 0xff);
*/


  /* USER CODE END MX_DISPLAY_Init 1 */
}

/**
 * DISPLAY application entry function
 */
void MX_DISPLAY_Process(void)
{
  /* USER CODE BEGIN MX_DISPLAY_Process */
	int32_t ret;
	uint32_t val;
	uint32_t x_size;
	uint32_t y_size;

	char msg_out[64];

	ret = BSP_LCD_GetXSize(0, &x_size);
  sprintf(msg_out, "GetXSize value: %lu, result: %ld\r\n", x_size, ret);
	HAL_UART_Transmit(&huart3, (uint8_t *)msg_out, strlen(msg_out)+1, 0xff);

	ret = BSP_LCD_GetYSize(0, &y_size);
  sprintf(msg_out, "GetYSize value: %lu, result: %ld\r\n", y_size, ret);
	HAL_UART_Transmit(&huart3, (uint8_t *)msg_out, strlen(msg_out)+1, 0xff);

	HAL_Delay(100);
	uint16_t color[5] = {red, green, blue, cyan, magenta};

	for (int c = 0; c < 5; c++)
	{
		// fill in 32*32px(*2bytes) test piece
		for (int i = 0; i < 2048; i+=2)
		{
			memcpy(rect + i, &color[c], 2);
		}

		for (int x = 0; x < x_size; x+=32)
		{
			for (int y = 0; y < y_size; y+=32)
			{
				ret = BSP_LCD_FillRGBRect(0, 0, rect, x, y, 32, 32);
				BSP_LCD_WaitForTransferToBeDone(0);
				  sprintf(msg_out, "Fill RGB at x:%ld, y:%ld, result: %ld\r\n", x, y, ret);
				HAL_UART_Transmit(&huart3, (uint8_t *)msg_out, strlen(msg_out)+1, 0xff);
			}
		}
	}

  /* USER CODE END MX_DISPLAY_Process */
}

void BSP_LCD_SignalTearingEffectEvent(uint32_t Instance, uint8_t state, uint16_t Line)
{
  if(Instance == 0)
  {
    /* USER CODE BEGIN BSP_LCD_SignalTearingEffectEvent */
    if(state)
    {
      /* Line '0' is the Vsync event */
      if(Line == 0)
      {
        /* TE event is received : allow display refresh */
      }
    }
    else
    {
      /* TE event is done : de-allow display refresh */
    }
    /* USER CODE END BSP_LCD_SignalTearingEffectEvent */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
