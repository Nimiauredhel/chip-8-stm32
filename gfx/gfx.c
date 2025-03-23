/*
 * gfx.c
 *
 *  Created on: Mar 23, 2025
 *      Author: mickey
 */

#include "gfx.h"

//extern UART_HandleTypeDef huart3;

void gfx_rgb_to_565_nonalloc(Color565_t *dest, uint8_t red_percent, uint8_t green_percent, uint8_t blue_percent)
{
    // RGB565 2 byte format: [gggBbbbb][RrrrrGgg]
    // start with zeroed struct
    bzero(dest, 2);

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

void gfx_fill_screen(Color565_t *fill_color)
{
    // write one color to the whole screen, but in 1/<divisor*divisor> chunks,
    // to avoid filling 50% of RAM for one color..
    static const uint8_t dimension_divisor = 8;

    uint16_t chunk_width = screen_get_x_size() / dimension_divisor;
    uint16_t chunk_height = screen_get_y_size() / dimension_divisor;
    uint16_t chunk_size_bytes = chunk_width * chunk_height * 2;
    uint8_t *chunk_data = (uint8_t *)malloc(chunk_size_bytes);

    for (uint16_t idx = 0; idx < chunk_size_bytes; idx++)
    {
        memcpy(chunk_data+idx, fill_color + idx % 2, 1);
    }

    uint16_t x = 0;
    uint16_t y = 0;

    while(y < dimension_divisor)
    {
        screen_fill_rect(chunk_data, x * chunk_width, y * chunk_height, chunk_width, chunk_height);

        x += 1;

        if (x >= dimension_divisor)
        {
            x = 0;
            y += 1;
        }
    }
}

void gfx_fill_rect_single_color(uint16_t x_origin, uint16_t y_origin, uint16_t width, uint16_t height, Color565_t fill_color);
void gfx_fill_rect_color_loop(uint16_t x_origin, uint16_t y_origin, uint16_t width, uint16_t height, Color565_t *fill_color_loop, uint16_t loop_length);

void gfx_draw_binary_sprite(BinarySprite_t sprite, uint16_t x_origin, uint16_t y_origin);
void gfx_draw_binary_sprite_scaled(BinarySprite_t sprite, uint16_t x_origin, uint16_t y_origin, float scale);

void gfx_draw_rect_sprite_565(RectSprite565_t sprite, uint16_t x_origin, uint16_t y_origin);
void gfx_draw_rect_sprite_565_scaled(RectSprite565_t sprite, uint16_t x_origin, uint16_t y_origin, float scale);
