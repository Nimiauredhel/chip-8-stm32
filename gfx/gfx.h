/*
 * gfx.h
 *
 *  Created on: Mar 23, 2025
 *      Author: mickey
 */

#ifndef GFX_H_
#define GFX_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "screen.h"

#define GFX_BUFFER_SIZE_BYTES 153600

#define R565_MAX 31
#define G565_MAX 63
#define B565_MAX 31

#define INT_PERCENT(percent, max) ((max * percent * 2) / 200)

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c\r\n"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

#define SHORT_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\r\n"
#define SHORT_TO_BINARY(byte)  \
  ((byte) & 0x8000 ? '1' : '0'), \
  ((byte) & 0x4000 ? '1' : '0'), \
  ((byte) & 0x2000 ? '1' : '0'), \
  ((byte) & 0x1000 ? '1' : '0'), \
  ((byte) & 0x0800 ? '1' : '0'), \
  ((byte) & 0x0400 ? '1' : '0'), \
  ((byte) & 0x0200 ? '1' : '0'), \
  ((byte) & 0x0100 ? '1' : '0'), \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

typedef uint8_t Color565_t[2];

typedef struct BinarySprite
{
	uint16_t height_pixels;
	uint8_t width_bytes;
	uint8_t pixel_mask[];
} BinarySprite_t;

// TODO: make a version of this that allows for negative space, like the binary sprite,
// TODO: without making every single cell bigger (like a bool would).
// TODO: a possible solution is variable length enconding for streaks of color/nothing
// TODO: VLE format idea: [VLE byte][color byte 0][color byte 1]
// TODO: VLE byte contents: (1->128) = color repeat length, (129->255) = blank length, (0) = terminator
// TODO: allow up to 127 compressed repeat color and/or repeat blank pixels, and length is encoded by terminator
// This struct represents a width*height rectangle of RGB565 pixels (NO blanks, NO transparency)
typedef struct RectSprite565
{
	uint16_t height_pixels;
	uint16_t width_pixels;
	Color565_t pixel_colors[];
} RectSprite565_t;

extern const Color565_t color_black;
extern const Color565_t color_white;

void gfx_push_to_screen(void);

/**
 * @brief takes in 0-100 percentages of RGB values and maps them to an approximate 565 representation
 * @param destination
 * @param red_percent
 * @param green_percent
 * @param blue_percent
 */
void gfx_rgb_to_565_nonalloc(Color565_t *destination, uint8_t red_percent, uint8_t green_percent, uint8_t blue_percent);

BinarySprite_t* gfx_bytes_to_binary_sprite(uint16_t height, uint8_t width, const uint8_t *data);

void gfx_fill_screen(Color565_t *fill_color);
void gfx_fill_rect_loop(uint8_t *data, uint32_t data_length, uint16_t x_origin, uint16_t y_origin, uint16_t width, uint16_t height);
void gfx_fill_rect_single_color(uint16_t x_origin, uint16_t y_origin, uint16_t width, uint16_t height, Color565_t *fill_color);

void gfx_draw_binary_sprite(BinarySprite_t *sprite, uint16_t x_origin, uint16_t y_origin, const Color565_t color, uint8_t scale);

void gfx_draw_rect_sprite_565(RectSprite565_t sprite, uint16_t x_origin, uint16_t y_origin);
void gfx_draw_rect_sprite_565_scaled(RectSprite565_t sprite, uint16_t x_origin, uint16_t y_origin, float scale);

#endif /* GFX_H_ */
