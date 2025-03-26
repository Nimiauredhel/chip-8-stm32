/*
 * gfx.c
 *
 *  Created on: Mar 23, 2025
 *      Author: mickey
 */

#include "gfx.h"
#include <stdbool.h>

//extern UART_HandleTypeDef huart3;

void gfx_rgb_to_565_nonalloc(Color565_t *dest, uint8_t red_percent, uint8_t green_percent, uint8_t blue_percent)
{
    // RGB565 2 byte format: [ggGbbbbB][rrrrRggg]
    // start with zeroed struct
	*dest[0] = 0x0;
	*dest[1] = 0x0;

    // set correctly shifted R segment in the second byte, its final location
    *dest[1] = INT_PERCENT(red_percent, R565_MAX) << 3;
    // temporarily set unshifted G segment in first byte
    *dest[0] = INT_PERCENT(green_percent, G565_MAX);
    // OR shifted upper half of G into second byte
    *dest[1] |= (*dest[0] >> 3);
    // shift first byte to only leave lower half of G
    *dest[0] <<= 5;
    // finally, OR entire B segment into first byte
    *dest[0] |= INT_PERCENT(blue_percent, B565_MAX);

    /*
    char log[64];
    sprintf(log, "R%03u|G%03u|B%03u translated to binary.\r\n", red_percent, green_percent, blue_percent);
    HAL_UART_Transmit(&huart3, (uint8_t *)log, strlen(log), 0xff);
    sprintf(log, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*dest[0]));
    HAL_UART_Transmit(&huart3, (uint8_t *)log, strlen(log), 0xff);
    sprintf(log, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*dest[1]));
    HAL_UART_Transmit(&huart3, (uint8_t *)log, strlen(log), 0xff);
    */
}

BinarySprite_t* gfx_bytes_to_binary_sprite(uint16_t height_pixels, uint8_t width_bytes, uint8_t *data)
{
    // start with zeroed struct
    uint16_t data_size = height_pixels * width_bytes;
    BinarySprite_t *sprite = malloc(sizeof(BinarySprite_t) + data_size);
    sprite->width_bytes = width_bytes;
    sprite->height_pixels = height_pixels;
    memcpy(sprite->pixel_mask, data, data_size);
    return sprite;
}

void gfx_fill_screen(Color565_t *fill_color)
{
    // write one color to the whole screen, but in chunks,
    // to avoid filling 50% of RAM for one color..
	// the buffer is allocated once since it will always be the same size
	static const uint8_t screen_fill_divisor = 32;
	static uint16_t screen_fill_buffer_size_bytes = 0;
	static uint8_t *screen_fill_buffer =  NULL;

	uint16_t idx;

	if (screen_fill_buffer == NULL)
	{
		screen_fill_buffer_size_bytes
		= (screen_get_x_size() * screen_get_y_size() * 2) / screen_fill_divisor;
		screen_fill_buffer = (uint8_t *)malloc(screen_fill_buffer_size_bytes);
	}

    for (idx = 0; idx < screen_fill_buffer_size_bytes; idx++)
    {
        memcpy(screen_fill_buffer+idx, fill_color + idx % 2, 1);
    }

    BSP_LCD_SetDisplayWindow(0, 0, 0, screen_get_x_size(), screen_get_y_size());

    for (idx = 0; idx < screen_fill_divisor; idx++)
    {
    	BSP_LCD_WriteData(0, screen_fill_buffer, screen_fill_buffer_size_bytes);
    }
/*
    while(y < screen_fill_dimension_divisor)
    {
        screen_fill_rect(screen_fill_buffer,
        		x * screen_fill_buffer_width, y * screen_fill_buffer_height,
				screen_fill_buffer_width, screen_fill_buffer_height);

        x += 1;

        if (x >= screen_fill_dimension_divisor)
        {
            x = 0;
            y += 1;
        }
    }
    */
}

void gfx_fill_rect_single_color(uint16_t x_origin, uint16_t y_origin, uint16_t width, uint16_t height, Color565_t *fill_color)
{
    uint16_t buffer_size = width * height * 2;
    uint8_t *rect_buffer = (uint8_t *)malloc(buffer_size);

    for (uint16_t idx = 0; idx < buffer_size; idx++)
    {
        memcpy(rect_buffer+idx, fill_color + idx % 2, 1);
    }

    screen_fill_rect(rect_buffer, x_origin, y_origin, width, height);

    free(rect_buffer);
}

void gfx_fill_rect_color_loop(uint16_t x_origin, uint16_t y_origin, uint16_t width, uint16_t height, Color565_t *fill_color_loop, uint16_t loop_length)
{
    uint16_t buffer_size = width * height * 2;
    uint8_t *rect_buffer = (uint8_t *)malloc(buffer_size);

    for (uint16_t idx = 0; idx < buffer_size; idx++)
    {
        memcpy(rect_buffer+idx, fill_color_loop + (idx % loop_length + idx % 2), 1);
    }

    screen_fill_rect(rect_buffer, x_origin, y_origin, width, height);

    free(rect_buffer);
}

static void gfx_draw_binary_byte(uint8_t byte, uint16_t x_origin, uint16_t y_origin, Color565_t color)
{
    int8_t start_bit = -1;
    int8_t end_bit = -1;
    bool bit_on = false;

    for (uint8_t bit = 0; bit < 8; bit++)
    {
		bit_on = byte & (1 << (7 - bit));

		if (bit_on)
		{
			if (start_bit == -1)
			{
				start_bit = bit;
			}

			end_bit = bit;
		}
		else
		{
			if (start_bit > -1)
			{
				gfx_fill_rect_single_color(x_origin + start_bit, y_origin, 1 + (end_bit - start_bit), 1, color);
				start_bit = -1;
				end_bit = -1;
			}
		}
    }

    if (start_bit > -1)
    {
		gfx_fill_rect_single_color(x_origin + start_bit, y_origin, 8 - start_bit, 1, color);
    }
}

static void gfx_draw_binary_byte_scaled(uint8_t byte, uint16_t x_origin, uint16_t y_origin, Color565_t color, uint8_t scale)
{
    int8_t start_bit = -1;
    int8_t end_bit = -1;
    bool bit_on = false;

    for (uint8_t bit = 0; bit < 8; bit++)
    {
		bit_on = byte & (1 << (7 - bit));

		if (bit_on)
		{
			if (start_bit == -1)
			{
				start_bit = bit;
			}

			end_bit = bit;
		}
		else
		{
			if (start_bit > -1)
			{
				gfx_fill_rect_single_color(x_origin + (scale * start_bit), y_origin, scale * (1 + (end_bit - start_bit)), 1, color);
				start_bit = -1;
				end_bit = -1;
			}
		}
    }

    if (start_bit > -1)
    {
		gfx_fill_rect_single_color(x_origin + (scale * start_bit), y_origin, scale * (8 - start_bit), 1, color);
    }
}

void gfx_draw_binary_sprite(BinarySprite_t *sprite, uint16_t x_origin, uint16_t y_origin, Color565_t color)
{
    // line by line (possibly naive approach)

    for (uint8_t xb = 0; xb < sprite->width_bytes; xb++)
    {
        for (uint16_t y = 0; y < sprite->height_pixels; y++)
        {
            gfx_draw_binary_byte(sprite->pixel_mask[y + (xb * sprite->height_pixels)], x_origin + (xb * 8), y_origin + y, color);
        }
    }
}

void gfx_draw_binary_sprite_scaled(BinarySprite_t *sprite, uint16_t x_origin, uint16_t y_origin, Color565_t color, uint8_t scale)
{
    // line by line (possibly naive approach)

    for (uint8_t xb = 0; xb < sprite->width_bytes; xb++)
    {
        for (uint16_t y = 0; y < sprite->height_pixels; y++)
        {
        	for (uint8_t s = 0; s < scale; s++)
        	{
				gfx_draw_binary_byte_scaled(sprite->pixel_mask[y + (xb * sprite->height_pixels)], x_origin + scale * (xb * 8), y_origin + (y * scale) + s, color, scale);
        	}
        }
    }
}

void gfx_draw_rect_sprite_565(RectSprite565_t sprite, uint16_t x_origin, uint16_t y_origin);
void gfx_draw_rect_sprite_565_scaled(RectSprite565_t sprite, uint16_t x_origin, uint16_t y_origin, float scale);
