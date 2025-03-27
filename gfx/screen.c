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
	if (width * height < 1) return;

    uint16_t divisor = ((width * height * 2) / data_length);

    if (divisor < 1) divisor = 1;

    BSP_LCD_SetDisplayWindow(0, x_origin, y_origin, width, height);

	//int32_t ret;
	//char msg_out[64];

    for (uint16_t idx = 0; idx < divisor; idx++)
    {
    	// TODO: figure out enabling DMA

    	BSP_LCD_WriteData(0, data, data_length);
    	//ret = BSP_LCD_WriteData(0, data, data_length);
		//sprintf(msg_out, "WriteData result: %ld\r\n", ret);
		//HAL_UART_Transmit(&huart3, (uint8_t *)msg_out, strlen(msg_out)+1, 0xff);
		//HAL_Delay(100);
		//HAL_DMA_PollForTransfer(hspi1.hdmatx, HAL_DMA_FULL_TRANSFER, 0xff);
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
