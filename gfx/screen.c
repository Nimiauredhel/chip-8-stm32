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

void screen_fill_rect_loop(uint8_t *data, uint16_t data_length, uint16_t x_origin, uint16_t y_origin, uint16_t width, uint16_t height)
{
	if (width * height < 0) return;

    uint16_t divisor = width * height * 2 / data_length;

    if (divisor < 1) divisor = 1;

    BSP_LCD_SetDisplayWindow(0, x_origin, y_origin, width, height);

    for (uint16_t idx = 0; idx < divisor; idx++)
    {
    	// TODO: figure out enabling DMA
    	BSP_LCD_WriteData(0, data, data_length);
    }
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

void screen_await_transfer(void)
{
	BSP_LCD_WaitForTransferToBeDone(0);
}
