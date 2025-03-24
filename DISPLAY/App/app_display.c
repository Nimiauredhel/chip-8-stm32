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
#include "gfx.h"

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
extern UART_HandleTypeDef huart3;

uint16_t red = 0b1111100000000000;
uint16_t green = 0b0000011111100000;
uint16_t blue = 0b0000000000011111;
uint16_t cyan = 0b0000011111111111;
uint16_t magenta = 0b1111100000011111;

uint8_t rect[2048];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void three_color_wipe(void)
{
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
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
	const uint16_t step_delay = 50;
	Color565_t color = {0};
	Color565_t color_loop[16] = {0};

	gfx_rgb_to_565_nonalloc(color_loop,    50, 50, 0);
	gfx_rgb_to_565_nonalloc(color_loop+1,  50, 50, 5);
	gfx_rgb_to_565_nonalloc(color_loop+2,  50, 50, 10);
	gfx_rgb_to_565_nonalloc(color_loop+3,  50, 50, 15);
	gfx_rgb_to_565_nonalloc(color_loop+4,  50, 50, 20);
	gfx_rgb_to_565_nonalloc(color_loop+5,  50, 50, 25);
	gfx_rgb_to_565_nonalloc(color_loop+6,  50, 50, 30);
	gfx_rgb_to_565_nonalloc(color_loop+7,  50, 50, 35);
    gfx_rgb_to_565_nonalloc(color_loop+8,  50, 50, 35);
    gfx_rgb_to_565_nonalloc(color_loop+9,  50, 50, 30);
    gfx_rgb_to_565_nonalloc(color_loop+10, 50, 50, 25);
    gfx_rgb_to_565_nonalloc(color_loop+11, 50, 50, 20);
    gfx_rgb_to_565_nonalloc(color_loop+12, 50, 50, 15);
    gfx_rgb_to_565_nonalloc(color_loop+13, 50, 50, 10);
    gfx_rgb_to_565_nonalloc(color_loop+14, 50, 50, 5);
	gfx_rgb_to_565_nonalloc(color_loop+15, 50, 50, 0);

	HAL_Delay(1000);

	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 100, 0, 0);
	gfx_fill_screen(&color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 75, 25, 0);
	gfx_fill_rect_single_color(0, 0, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 50, 50, 0);
	gfx_fill_rect_single_color(80, 0, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 25, 75, 0);
	gfx_fill_rect_single_color(160, 0, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 0, 100, 0);
	gfx_fill_rect_single_color(240, 0, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 0, 75, 25);
	gfx_fill_rect_single_color(240, 60, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 0, 50, 50);
	gfx_fill_rect_single_color(160, 60, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 0, 25, 75);
	gfx_fill_rect_single_color(80, 60, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 0, 0, 100);
	gfx_fill_rect_single_color(0, 60, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 75, 25, 0);
	gfx_fill_rect_single_color(0, 120, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 50, 50, 0);
	gfx_fill_rect_single_color(80, 120, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 25, 75, 0);
	gfx_fill_rect_single_color(160, 120, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 0, 100, 0);
	gfx_fill_rect_single_color(240, 120, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 0, 75, 25);
	gfx_fill_rect_single_color(240, 180, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 0, 50, 50);
	gfx_fill_rect_single_color(160, 180, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 0, 25, 75);
	gfx_fill_rect_single_color(80, 180, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_rgb_to_565_nonalloc(&color, 0, 0, 100);
	gfx_fill_rect_single_color(0, 180, 80, 60, color);

	HAL_Delay(step_delay);
	screen_await_transfer();
	gfx_fill_rect_color_loop(80, 60, 160, 120, color_loop, 16);

	uint8_t hello_bytes[25] =
	{
			0b10010000,
			0b10010000,
			0b11110000,
			0b10010000,
			0b10010000,
			0b11110000,
			0b10000000,
			0b11110000,
			0b10000000,
			0b11110000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b11110000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b11110000,
			0b11110000,
			0b10010000,
			0b10010000,
			0b10010000,
			0b11110000,
	};

	uint8_t world_bytes[25] =
	{
			0b10001000,
			0b10101000,
			0b10101000,
			0b10101000,
			0b11111000,
			0b11110000,
			0b10010000,
			0b10010000,
			0b10010000,
			0b11110000,
			0b11100000,
			0b10010000,
			0b11100000,
			0b10100000,
			0b10010000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b11110000,
			0b11100000,
			0b10010000,
			0b10010000,
			0b10010000,
			0b11100000,
	};

	Color565_t hello_color = {0};
	Color565_t world_color = {0};
	gfx_rgb_to_565_nonalloc(&hello_color, 100, 95, 100);
	gfx_rgb_to_565_nonalloc(&world_color, 95, 100, 100);
	BinarySprite_t *hello_sprite = gfx_bytes_to_binary_sprite(5, 5, hello_bytes);
	BinarySprite_t *world_sprite = gfx_bytes_to_binary_sprite(5, 5, world_bytes);
	gfx_draw_binary_sprite_scaled(hello_sprite, 88, 75, hello_color, 4);
	gfx_draw_binary_sprite_scaled(world_sprite, 88, 135, world_color, 4);
	free(hello_sprite);
	free(world_sprite);

	HAL_Delay(2000);

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
