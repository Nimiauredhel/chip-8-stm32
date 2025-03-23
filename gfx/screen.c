/*
 * screen.c
 *
 *  Created on: Mar 23, 2025
 *      Author: mickey
 */

#include "screen.h"

void screen_fill_rect(uint8_t *data, uint16_t x_origin, uint16_t y_origin, uint16_t width, uint16_t height)
{
	BSP_LCD_FillRGBRect(0, 0, data, x_origin, y_origin, width, height);
}

uint32_t screen_get_x_size(void)
{
	uint32_t value;
	BSP_LCD_GetXSize(0, &value);
	return value;
}

uint32_t screen_get_y_size(void)
{
	uint32_t value;
	BSP_LCD_GetYSize(0, &value);
	return value;
}
