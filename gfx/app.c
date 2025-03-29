/*
 * app.c
 *
 *  Created on: Mar 29, 2025
 *      Author: mickey
 */

#include "app.h"

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

const uint16_t half_step_delay = 32;
const uint16_t step_delay = 64;
const uint16_t second_delay = 1000;

Color565_t color;
uint8_t color_loop[32];

Color565_t hello_color;
Color565_t world_color;
Color565_t alien_color;
BinarySprite_t *hello_sprite;
BinarySprite_t *world_sprite;
BinarySprite_t *alien_sprite;

bool gfx_dirty = false;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (gfx_dirty)
	{
		gfx_dirty = false;
		gfx_push_to_screen();
	}
}

void app_init(void)
{
	int32_t ret;
	char msg_out[64];

  if(BSP_LCD_Init(0, LCD_ORIENTATION_PORTRAIT) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  sprintf(msg_out, "LCD Initialized.\r\n");
  HAL_UART_Transmit(&huart3, (uint8_t *)msg_out, strlen(msg_out)+1, 0xff);

  ret = BSP_LCD_SetOrientation(0, LCD_ORIENTATION_LANDSCAPE);
  sprintf(msg_out, "Set Orientation result: %ld\r\n", ret);
	HAL_UART_Transmit(&huart3, (uint8_t *)msg_out, strlen(msg_out)+1, 0xff);

  ret = BSP_LCD_DisplayOn(0);
  sprintf(msg_out, "Display On result: %ld\r\n", ret);
	HAL_UART_Transmit(&huart3, (uint8_t *)msg_out, strlen(msg_out)+1, 0xff);

	// init color loop
	for (int i = 0; i < 32; i+=2)
	{
		gfx_rgb_to_565_nonalloc(color, 100 / 31 * i, 10 - i / 10, 10 - i / 10);
		color_loop[i] = color[0];
		color_loop[i+1] = color[1];
	}

	// init sprites and some colors
	gfx_rgb_to_565_nonalloc(hello_color, 100, 95, 100);
	gfx_rgb_to_565_nonalloc(world_color, 95, 100, 100);
	gfx_rgb_to_565_nonalloc(alien_color, 0, 100, 0);
	hello_sprite = gfx_bytes_to_binary_sprite(5, 5, hello_bytes);
	world_sprite = gfx_bytes_to_binary_sprite(5, 5, world_bytes);
	alien_sprite = gfx_bytes_to_binary_sprite(8, 2, alien_bytes);

	HAL_TIM_Base_Start_IT(&htim2);
}

void app_loop(void)
{
	gfx_fill_screen(color_white);
	gfx_dirty = true;
	HAL_Delay(half_step_delay);
	gfx_fill_screen(color_red);
	gfx_dirty = true;
	HAL_Delay(half_step_delay);
	gfx_fill_screen(color_green);
	gfx_dirty = true;
	HAL_Delay(half_step_delay);
	gfx_fill_screen(color_blue);
	gfx_dirty = true;
	HAL_Delay(half_step_delay);
	gfx_fill_screen(color_cyan);
	gfx_dirty = true;
	HAL_Delay(half_step_delay);
	gfx_fill_screen(color_yellow);
	gfx_dirty = true;
	HAL_Delay(half_step_delay);
	gfx_fill_screen(color_magenta);
	gfx_dirty = true;
	HAL_Delay(half_step_delay);
	gfx_fill_screen(color_black);
	gfx_dirty = true;
	HAL_Delay(half_step_delay);

	gfx_rgb_to_565_nonalloc(color, 75, 25, 0);
	gfx_fill_rect_single_color(0, 0, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 50, 50, 0);
	gfx_fill_rect_single_color(80, 0, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 25, 75, 0);
	gfx_fill_rect_single_color(160, 0, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 0, 100, 0);
	gfx_fill_rect_single_color(240, 0, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 0, 75, 25);
	gfx_fill_rect_single_color(240, 60, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 0, 50, 50);
	gfx_fill_rect_single_color(160, 60, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 0, 25, 75);
	gfx_fill_rect_single_color(80, 60, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 0, 0, 100);
	gfx_fill_rect_single_color(0, 60, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 75, 25, 0);
	gfx_fill_rect_single_color(0, 120, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 50, 50, 0);
	gfx_fill_rect_single_color(80, 120, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 25, 75, 0);
	gfx_fill_rect_single_color(160, 120, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 0, 100, 0);
	gfx_fill_rect_single_color(240, 120, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 0, 75, 25);
	gfx_fill_rect_single_color(240, 180, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 0, 50, 50);
	gfx_fill_rect_single_color(160, 180, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 0, 25, 75);
	gfx_fill_rect_single_color(80, 180, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);

	gfx_rgb_to_565_nonalloc(color, 0, 0, 100);
	gfx_fill_rect_single_color(0, 180, 80, 60, color);
	gfx_dirty = true;
	HAL_Delay(step_delay);
	HAL_Delay(step_delay);

	float scale = 0.00f;

	while (scale < 1.0f)
	{
		scale += 0.05f;
		gfx_fill_rect_loop(color_loop, 32, 160 - (160 * scale), 120 - (120 * scale), 320 * scale, 240 * scale);
		gfx_dirty = true;
		HAL_Delay(half_step_delay);
	}

	HAL_Delay(step_delay);

	gfx_draw_binary_sprite(hello_sprite, 160-20+2, 120-15, hello_color, 1);
	gfx_draw_binary_sprite(world_sprite, 160-20+2, 120+10, world_color, 1);
	gfx_draw_binary_sprite(alien_sprite, 16, 130, alien_color, 11);
	gfx_dirty = true;
	HAL_Delay(step_delay);
	HAL_Delay(step_delay);
	gfx_fill_rect_loop(color_loop, 32, 0, 0, 320, 240);
	gfx_draw_binary_sprite(hello_sprite, 160-40+4, 120-20, hello_color, 2);
	gfx_draw_binary_sprite(world_sprite, 160-40+4, 120+10, world_color, 2);
	gfx_draw_binary_sprite(alien_sprite, 32, 130, alien_color, 10);
	gfx_dirty = true;
	HAL_Delay(step_delay);
	gfx_fill_rect_loop(color_loop, 32, 0, 0, 320, 240);
	gfx_draw_binary_sprite(hello_sprite, 160-80+8, 120-30, hello_color, 4);
	gfx_draw_binary_sprite(world_sprite, 160-80+8, 120+10, world_color, 4);
	gfx_draw_binary_sprite(alien_sprite, 48, 130, alien_color, 9);
	gfx_dirty = true;
	HAL_Delay(step_delay);
	gfx_fill_rect_loop(color_loop, 32, 0, 0, 320, 240);
	gfx_draw_binary_sprite(alien_sprite, 64, 130, alien_color, 8);
	gfx_draw_binary_sprite(hello_sprite, 24, 120-42, color_black, 8);
	gfx_draw_binary_sprite(world_sprite, 24, 120+18, color_black, 8);
	gfx_draw_binary_sprite(hello_sprite, 16, 120-50, hello_color, 8);
	gfx_draw_binary_sprite(world_sprite, 16, 120+10, world_color, 8);
	gfx_dirty = true;
	HAL_Delay(step_delay);
	HAL_Delay(step_delay);
	gfx_draw_binary_sprite(alien_sprite, 100, 130, alien_color, 8);
	gfx_draw_binary_sprite(hello_sprite, 24, 120-42, color_black, 8);
	gfx_draw_binary_sprite(world_sprite, 24, 120+18, color_black, 8);
	gfx_draw_binary_sprite(hello_sprite, 16, 120-50, hello_color, 8);
	gfx_draw_binary_sprite(world_sprite, 16, 120+10, world_color, 8);
	gfx_dirty = true;
	HAL_Delay(step_delay);
	HAL_Delay(step_delay);
	gfx_draw_binary_sprite(alien_sprite, 32, 48, alien_color, 8);
	gfx_draw_binary_sprite(hello_sprite, 24, 120-42, color_black, 8);
	gfx_draw_binary_sprite(world_sprite, 24, 120+18, color_black, 8);
	gfx_draw_binary_sprite(hello_sprite, 16, 120-50, hello_color, 8);
	gfx_draw_binary_sprite(world_sprite, 16, 120+10, world_color, 8);
	gfx_dirty = true;
	HAL_Delay(step_delay);
	HAL_Delay(step_delay);
	gfx_draw_binary_sprite(alien_sprite, 200, 64, alien_color, 8);
	gfx_draw_binary_sprite(hello_sprite, 24, 120-42, color_black, 8);
	gfx_draw_binary_sprite(world_sprite, 24, 120+18, color_black, 8);
	gfx_draw_binary_sprite(hello_sprite, 16, 120-50, hello_color, 8);
	gfx_draw_binary_sprite(world_sprite, 16, 120+10, world_color, 8);
	gfx_dirty = true;
	HAL_Delay(second_delay);
	gfx_fill_rect_loop(color_loop, 32, 0, 0, 320, 240);
	gfx_dirty = true;
	HAL_Delay(second_delay);
}
