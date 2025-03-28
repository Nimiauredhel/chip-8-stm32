/*
 * screen.c
 *
 *  Created on: Mar 23, 2025
 *      Author: mickey
 */

#include "screen.h"

void screen_fill_rect_loop(uint8_t *data, uint32_t data_length, uint16_t x_origin, uint16_t y_origin, uint16_t width, uint16_t height)
{
	if (width * height < 1) return;

    uint32_t divisor = (width * height * 2 / data_length);

    BSP_LCD_SetDisplayWindow(0, x_origin, y_origin, width, height);

	//int32_t ret;
	//char msg_out[64];

    if(divisor <= 2)
    {
    	BSP_LCD_WriteData(0, data, data_length/2);
    	BSP_LCD_WriteData(0, data+data_length/2, data_length/2);
    }
    else
    {
		for (uint32_t idx = 0; idx < divisor; idx++)
		{
			// TODO: figure out enabling DMA
			BSP_LCD_WriteData(0, data, data_length);
		}
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
