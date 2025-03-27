/*
 * gfx.c
 *
 *  Created on: Mar 23, 2025
 *      Author: mickey
 */

#include "gfx.h"
#include <stdbool.h>

//extern UART_HandleTypeDef huart3;

static uint8_t gfx_buffer[GFX_BUFFER_SIZE_BYTES];

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
    for (uint16_t idx = 0; idx < GFX_BUFFER_SIZE_BYTES; idx++)
    {
        memcpy(gfx_buffer+idx, fill_color + idx % 2, 1);
    }

    screen_fill_rect_loop(gfx_buffer, GFX_BUFFER_SIZE_BYTES,
    		0, 0, screen_get_x_size(), screen_get_y_size());
}

void gfx_fill_rect_single_color(uint16_t x_origin, uint16_t y_origin, uint16_t width, uint16_t height, Color565_t *fill_color)
{
	if (width * height < 1) return;

    uint16_t buffer_size = width * height * 2;
	uint8_t buffer_divisor = 1;

	if (buffer_size > GFX_BUFFER_SIZE_BYTES)
	{
		buffer_divisor = (buffer_size / GFX_BUFFER_SIZE_BYTES) + 1;
		buffer_size /= buffer_divisor;
	}

    for (uint16_t idx = 0; idx < buffer_size; idx++)
    {
        memcpy(gfx_buffer+idx, fill_color + idx % 2, 1);
    }

    screen_fill_rect_loop(gfx_buffer, buffer_size,
    		x_origin, y_origin, width, height);
}

static void gfx_draw_binary_byte(uint8_t byte, uint16_t x_origin, uint16_t y_origin, Color565_t color, uint8_t scale)
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
				gfx_fill_rect_single_color(x_origin + (scale * start_bit), y_origin, scale * (1 + (end_bit - start_bit)), scale, color);
				start_bit = -1;
				end_bit = -1;
			}
		}
    }

    if (start_bit > -1)
    {
		gfx_fill_rect_single_color(x_origin + (scale * start_bit), y_origin, scale * (8 - start_bit), scale, color);
    }
}

void gfx_draw_binary_sprite(BinarySprite_t *sprite, uint16_t x_origin, uint16_t y_origin, Color565_t color, uint8_t scale)
{
    // line by line (possibly naive approach)

    for (uint8_t xb = 0; xb < sprite->width_bytes; xb++)
    {
        for (uint16_t y = 0; y < sprite->height_pixels; y++)
        {
			gfx_draw_binary_byte(sprite->pixel_mask[y + (xb * sprite->height_pixels)], x_origin + scale * (xb * 8), y_origin + (y * scale), color, scale);
        }
    }
}

void gfx_draw_rect_sprite_565(RectSprite565_t sprite, uint16_t x_origin, uint16_t y_origin);
void gfx_draw_rect_sprite_565_scaled(RectSprite565_t sprite, uint16_t x_origin, uint16_t y_origin, float scale);
