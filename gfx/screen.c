/*
 * screen.c
 *
 *  Created on: Mar 23, 2025
 *      Author: mickey
 */

#include "screen.h"

uint32_t screen_init(uint32_t orientation)
{
	int32_t ret;
	char buffer[64];

	ret = BSP_LCD_Init(0, orientation);
	sprintf(buffer, "LCD Init result: %ld.\r\n", ret);
	HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer)+1, 0xff);

	ret = BSP_LCD_SetOrientation(0, LCD_ORIENTATION_LANDSCAPE);
	sprintf(buffer, "Set Orientation result: %ld\r\n", ret);
	HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer)+1, 0xff);

	// clear out the screen
	explicit_bzero(buffer, 64);
	screen_fill_rect_loop(buffer, 64, 0, 0, screen_get_x_size(), screen_get_y_size());

	ret = BSP_LCD_DisplayOn(0);
	sprintf(buffer, "Display On result: %ld\r\n", ret);
	HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer)+1, 0xff);
}

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

// TODO: figure out how to use the tearing callback later
/*void BSP_LCD_SignalTearingEffectEvent(uint32_t Instance, uint8_t state, uint16_t Line)
{
  if(Instance == 0)
  {
    if(state)
    {
      if(Line == 0)
      {
      }
    }
    else
    {
    }
  }
}*/
