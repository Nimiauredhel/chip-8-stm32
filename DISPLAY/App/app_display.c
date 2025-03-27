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

	const uint8_t hello_bytes[25] =
	{
			0b10001000, // H
			0b10001000,
			0b11111000,
			0b10001000,
			0b10001000,
			0b11111000, // E
			0b10000000,
			0b11111000,
			0b10000000,
			0b11111000,
			0b10000000, // L
			0b10000000,
			0b10000000,
			0b10000000,
			0b11111000,
			0b10000000, // L
			0b10000000,
			0b10000000,
			0b10000000,
			0b11111000,
			0b11111000, // O
			0b10001000,
			0b10001000,
			0b10001000,
			0b11111000,
	};

	const uint8_t world_bytes[25] =
	{
			0b10001000, // W
			0b10101000,
			0b10101000,
			0b10101000,
			0b11111000,
			0b11111000, // O
			0b10001000,
			0b10001000,
			0b10001000,
			0b11111000,
			0b11110000, // R
			0b10001000,
			0b11110000,
			0b10010000,
			0b10001000,
			0b10000000, // L
			0b10000000,
			0b10000000,
			0b10000000,
			0b11111000,
			0b11110000, // D
			0b10001000,
			0b10001000,
			0b10001000,
			0b11110000,
	};

	const uint8_t alien_bytes[25] =
	{
			0b00001000, // W
			0b00000100,
			0b00001111,
			0b00011011,
			0b00111111,
			0b00101111, // O
			0b00101000,
			0b00001100,

			0b00010000,
			0b00100000,
			0b11110000,
			0b11011000,
			0b11111100,
			0b11110100,
			0b00010100,
			0b00110000,
	};

	const uint16_t step_delay = 50;

	Color565_t color;
	uint8_t color_loop[32];

	Color565_t hello_color;
	Color565_t world_color;
	Color565_t alien_color;
	BinarySprite_t *hello_sprite;
	BinarySprite_t *world_sprite;
	BinarySprite_t *alien_sprite;

/* USER CODE END 0 */

/**
 * Initialize DISPLAY application
 */
void MX_DISPLAY_Init(void)
{
  /* USER CODE BEGIN MX_DISPLAY_Init 0 */
	int32_t ret;
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

	// init color loop
	for (int i = 0; i < 32; i++)
	{
		gfx_rgb_to_565_nonalloc(&color, 100 / 31 * i, 10 - i / 10, 10 - i / 10);
        memcpy(color_loop+i, &color + i % 2, 1);
	}

	// init sprites
	gfx_rgb_to_565_nonalloc(&hello_color, 100, 95, 100);
	gfx_rgb_to_565_nonalloc(&world_color, 95, 100, 100);
	gfx_rgb_to_565_nonalloc(&alien_color, 0, 100, 0);
	hello_sprite = gfx_bytes_to_binary_sprite(5, 5, hello_bytes);
	world_sprite = gfx_bytes_to_binary_sprite(5, 5, world_bytes);
	alien_sprite = gfx_bytes_to_binary_sprite(8, 2, alien_bytes);

  /* USER CODE END MX_DISPLAY_Init 1 */
}

/**
 * DISPLAY application entry function
 */
void MX_DISPLAY_Process(void)
{
  /* USER CODE BEGIN MX_DISPLAY_Process */
	gfx_rgb_to_565_nonalloc(&color, 100, 0, 0);
	gfx_fill_screen(&color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 75, 25, 0);
	gfx_fill_rect_single_color(0, 0, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 50, 50, 0);
	gfx_fill_rect_single_color(80, 0, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 25, 75, 0);
	gfx_fill_rect_single_color(160, 0, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 0, 100, 0);
	gfx_fill_rect_single_color(240, 0, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 0, 75, 25);
	gfx_fill_rect_single_color(240, 60, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 0, 50, 50);
	gfx_fill_rect_single_color(160, 60, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 0, 25, 75);
	gfx_fill_rect_single_color(80, 60, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 0, 0, 100);
	gfx_fill_rect_single_color(0, 60, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 75, 25, 0);
	gfx_fill_rect_single_color(0, 120, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 50, 50, 0);
	gfx_fill_rect_single_color(80, 120, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 25, 75, 0);
	gfx_fill_rect_single_color(160, 120, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 0, 100, 0);
	gfx_fill_rect_single_color(240, 120, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 0, 75, 25);
	gfx_fill_rect_single_color(240, 180, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 0, 50, 50);
	gfx_fill_rect_single_color(160, 180, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 0, 25, 75);
	gfx_fill_rect_single_color(80, 180, 80, 60, color);

	HAL_Delay(step_delay);
	gfx_rgb_to_565_nonalloc(&color, 0, 0, 100);
	gfx_fill_rect_single_color(0, 180, 80, 60, color);

	HAL_Delay(step_delay);
	HAL_Delay(step_delay);

	float scale = 0.00f;

	while (scale < 1.0f)
	{
		scale += 0.05f;
		screen_fill_rect_loop(color_loop, 32, 160 - (160 * scale), 120 - (120 * scale), 320 * scale, 240 * scale);
		//HAL_Delay(step_delay);
		//screen_await_transfer();
	}

	HAL_Delay(step_delay);

	gfx_draw_binary_sprite(hello_sprite, 160-20+2, 120-15, hello_color, 1);
	gfx_draw_binary_sprite(world_sprite, 160-20+2, 120+10, world_color, 1);
	gfx_draw_binary_sprite(alien_sprite, 16, 130, alien_color, 11);
	HAL_Delay(step_delay);
	HAL_Delay(step_delay);
	screen_fill_rect_loop(color_loop, 32, 0, 0, 320, 240);
	gfx_draw_binary_sprite(hello_sprite, 160-40+4, 120-20, hello_color, 2);
	gfx_draw_binary_sprite(world_sprite, 160-40+4, 120+10, world_color, 2);
	gfx_draw_binary_sprite(alien_sprite, 32, 130, alien_color, 10);
	HAL_Delay(step_delay);
	screen_fill_rect_loop(color_loop, 32, 0, 0, 320, 240);
	gfx_draw_binary_sprite(hello_sprite, 160-80+8, 120-30, hello_color, 4);
	gfx_draw_binary_sprite(world_sprite, 160-80+8, 120+10, world_color, 4);
	gfx_draw_binary_sprite(alien_sprite, 48, 130, alien_color, 9);
	HAL_Delay(step_delay);
	screen_fill_rect_loop(color_loop, 32, 0, 0, 320, 240);
	gfx_draw_binary_sprite(alien_sprite, 64, 130, alien_color, 8);
	gfx_draw_binary_sprite(hello_sprite, 12, 120-50, 0x0000, 9);
	gfx_draw_binary_sprite(world_sprite, 12, 120+10, 0x0000, 9);
	gfx_draw_binary_sprite(hello_sprite, 16, 120-50, hello_color, 8);
	gfx_draw_binary_sprite(world_sprite, 16, 120+10, world_color, 8);
	HAL_Delay(2000);
	gfx_draw_binary_sprite(alien_sprite, 100, 130, alien_color, 8);
	gfx_draw_binary_sprite(hello_sprite, 12, 120-50, 0x0000, 9);
	gfx_draw_binary_sprite(world_sprite, 12, 120+10, 0x0000, 9);
	gfx_draw_binary_sprite(hello_sprite, 16, 120-50, hello_color, 8);
	gfx_draw_binary_sprite(world_sprite, 16, 120+10, world_color, 8);
	HAL_Delay(1000);
	gfx_draw_binary_sprite(alien_sprite, 32, 48, alien_color, 8);
	gfx_draw_binary_sprite(hello_sprite, 12, 120-50, 0x0000, 9);
	gfx_draw_binary_sprite(world_sprite, 12, 120+10, 0x0000, 9);
	gfx_draw_binary_sprite(hello_sprite, 16, 120-50, hello_color, 8);
	gfx_draw_binary_sprite(world_sprite, 16, 120+10, world_color, 8);
	HAL_Delay(1000);
	gfx_draw_binary_sprite(alien_sprite, 200, 64, alien_color, 8);
	gfx_draw_binary_sprite(hello_sprite, 12, 120-50, 0x0000, 9);
	gfx_draw_binary_sprite(world_sprite, 12, 120+10, 0x0000, 9);
	gfx_draw_binary_sprite(hello_sprite, 16, 120-50, hello_color, 8);
	gfx_draw_binary_sprite(world_sprite, 16, 120+10, world_color, 8);
	HAL_Delay(2000);
	screen_fill_rect_loop(color_loop, 32, 0, 0, 320, 240);

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
